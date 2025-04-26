#ifndef BIMSIM_CELL_HPP
#define BIMSIM_CELL_HPP

// Standard
#include <cmath>
#include <random>

// Other
#include <nlohmann/json.hpp>

// Cadmium V2
#include <cadmium/modeling/celldevs/grid/cell.hpp>
#include <cadmium/modeling/celldevs/grid/config.hpp>

// Models
#include "bimsimState.hpp"

using namespace cadmium::celldevs;


/**
 * Atomic Model Cell Definition
 */
class bimsim : public GridCell<BimSimState, double> {

    private:

    /**
     * Constants
     */

    // Delay time units
    static constexpr double DEFAULT_DELAY_TIME = 1.00;
    // External temperature (can't be colder than this!)
    static constexpr double EXTERNAL_TEMP = 10.00;
    // Target temperatures
    static constexpr double MIN_TARGET_TEMP = 21.00;
    static constexpr double MAX_TARGET_TEMP = 24.00;
    static constexpr double TARGET_TEMP = (MAX_TARGET_TEMP + MIN_TARGET_TEMP) / 2;
    // Temperature step for state categorization
    static constexpr double TEMP_STEP = 1.00; // Step for up and down the state scale
    // Heater temperature contribution
    static constexpr double HEATER_TEMP_INCREASE = 100.00;
    // Dissipation bounds - may relocate to cell properties for easier configuration
    static constexpr double DISSIPATION_MIN = 0.10;
    static constexpr double DISSIPATION_MAX = 0.20;

    public:

    /**
     * Constructor
     */
    bimsim(const std::vector<int>& id, 
            const std::shared_ptr<const GridCellConfig<BimSimState, double>>& config
          ): GridCell<BimSimState, double>(id, config) { }

    /**
     * Local Computation Function (tau)
     */
    [[nodiscard]] BimSimState localComputation(BimSimState state,
        const std::unordered_map<std::vector<int>, NeighborData<BimSimState, double>>& neighborhood) const override {

        /* Local Variables */

        // Accumulators 
        int neighbours = 0;
        int occupant_neighbours = 0;
        double neighbourhood_temperature = 0.0;

        /* Canvas the Neighborhood */

        // Canvas this cell's neighborhood to categorize neighbor types
        // ie. tally neighbor cells by type
        for (const auto& [neighborId, neighborData]: neighborhood) {
            // State of the neighbor cell for this iteration
            auto nState = neighborData.state;

            // DEBUG
            //std::cout << "\n\tNeighbour " << neighbours << ": [" << neighborId << "]";

            // Accumulate neighbours for temperature averaging calculation
            neighbours++;
            // Accumulate occupant neighbours for sensor use in controlling heaters
            if (nState->type == BimSimStateName::OCCUPANT) {
                occupant_neighbours++;
            }
            neighbourhood_temperature += nState->temperature;

        }

        /* Mutate State Based on Rules and Return */

        // Case: EMPTY cell [0, 7]
        if(state.type >= BimSimStateName::EMPTY_COLD_0 && state.type <= BimSimStateName::EMPTY_HOT_6) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Update the state based on new temperature (for visualization)
            updateEmptyCellStateByTemperature(state);
        } 
        // Case: HEATER cell [9, 10]
        else if(state.type == BimSimStateName::HEATER_ON || state.type == BimSimStateName::HEATER_OFF) {

            // Take the average neighbourhood temperature as the new cell temperature.
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();

            // DEBUG
            //std::cout << state.type << ":" 
            //          << "\n\tCurrent Temperature: " << state.temperature
            //          << "\n\tTarget Temperature:  " << TARGET_TEMP
            //          << std::endl;
            //
            // Case: HEATER_ON, so supply heat
            if(state.type == BimSimStateName::HEATER_ON) {
                // Supply heat from heater
                state.temperature += HEATER_TEMP_INCREASE;
                // Rather than incrementing it, use thermostat temp
                //state.temperature = TARGET_TEMP;
            }


            // Turn the heater ON or OFF as required
            updateHeaterCellState(state);
        } 
        // Case: WALL cell 
        else if(state.type == BimSimStateName::WALL) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Apply external wall heat dissipation
            state.temperature -= dissipate(1.00, 2.00);
        }
        // Case: WINDOW cell 
        else if(state.type == BimSimStateName::WINDOW) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Apply window heat dissipation 
            state.temperature -= dissipate(2.00, 3.00);
        }

        // Return the (possibly) mutated state, with its temperature retained
        return state;

    }

    /**
     * Delay function (D)
     */
    [[nodiscard]] double outputDelay(const BimSimState& state) const override {
        return DEFAULT_DELAY_TIME;
    }

    /**
     * Get a random probability in [0, 1]
     */
    double randomProbability() const {     
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    /**
     * Dissipate some heat within a range
     */
    double dissipate(double min = DISSIPATION_MIN, double max = DISSIPATION_MAX) const {     
        return randomDouble(min, max);
    }

    /**
     * Get a random double in limits
     */
    double randomDouble(double min, double max) const {     
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    /**
     * Update (mutate) state of EMPTY cells based on temperature.
     */
    void updateEmptyCellStateByTemperature(BimSimState& state) const {     

        // < 19
        if (state.temperature < (MIN_TARGET_TEMP - 2.00))  {
            state.type = BimSimStateName::EMPTY_COLD_0;
        }
        // [19, 20)
        else if (state.temperature < (MIN_TARGET_TEMP - 1.00))  {
            state.type = BimSimStateName::EMPTY_COLD_1;
        }
        // [20, 21)
        else if (state.temperature < MIN_TARGET_TEMP)  {
            state.type = BimSimStateName::EMPTY_COLD_2;
        }
        // [21, 24]
        else if (state.temperature >= MIN_TARGET_TEMP && state.temperature <= MAX_TARGET_TEMP) {
            state.type = BimSimStateName::EMPTY_OK_3;
        }
        // (24, 25)
        else if (state.temperature < MAX_TARGET_TEMP + 1.00) {
            state.type = BimSimStateName::EMPTY_HOT_4;
        } 
        // [25, 26)
        else if (state.temperature < MAX_TARGET_TEMP + 2.00) {
            state.type = BimSimStateName::EMPTY_HOT_5;
        }
        // > 26
        else {
            state.type = BimSimStateName::EMPTY_HOT_6;
        }

    }

    /**
     * Update (mutate) state of HEATER cells based on temperature targets.
     */
    void updateHeaterCellState(BimSimState& state) const {     

        // If strictly less than target temperature, heater ON
        if (state.temperature < TARGET_TEMP)  {
            state.type = BimSimStateName::HEATER_ON;
            //std::cout << "HEATER ON" << std::endl;
        }
        // If greater than target temperature, heater OFF
        else {
            state.type = BimSimStateName::HEATER_OFF;
            //std::cout << "HEATER ON" << std::endl;
        }
    }

};

#endif // BIMSIM_CELL_HPP
