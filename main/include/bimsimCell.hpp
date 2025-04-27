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
    // N.B. This looks like a lot, but it diffuses rapidly
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
            // Accumulate neighbours for temperature averaging calculation
            neighbours++;
            // Accumulate occupant neighbours for sensor use in controlling heaters
            if (nState->type == BimSimStateName::OCCUPANT_COMFORTABLE || 
                nState->type == BimSimStateName::OCCUPANT_UNCOMFORTABLE) {
                occupant_neighbours++;
            }
            // Accumulate neighbour temperatures
            neighbourhood_temperature += nState->temperature;
        }

        /* Mutate State Based on Rules and Return */

        // Case: Empty cells
        // 0  EMPTY_COLD_0             #b3b3ff    [179, 179, 255]
        // 1  EMPTY_COLD_1             #ccccff    [204, 204, 255]
        // 2  EMPTY_COLD_2             #e6e6ff    [230, 230, 255]
        // 3  EMPTY_OK_3               #ffffff    [255, 255, 255]    
        // 4  EMPTY_HOT_4              #ffe6e6    [255, 230, 230]
        // 5  EMPTY_HOT_5              #ffcccc    [255, 204, 204]
        // 6  EMPTY_HOT_6              #ffb3b3    [255, 179, 179]
        if(state.type >= BimSimStateName::EMPTY_COLD_0 && state.type <= BimSimStateName::EMPTY_HOT_6) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Update the state based on new temperature (for visualization)
            updateEmptyCellStateByTemperature(state);
        } 
        // Case: Occupants
        // 7  OCCUPANT_COMFORTABLE     #ffff66    [255, 255, 102]
        // 8  OCCUPANT_UNCOMFORTABLE   #cc33ff    [204,  51, 255]
        else if(state.type == BimSimStateName::OCCUPANT_COMFORTABLE || state.type == BimSimStateName::OCCUPANT_UNCOMFORTABLE) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Update the state based on climate comfort level
            updateOccupantCellState(state);

        }
        // Case: Heaters
        // 9  HEATER_ON                #ff0002    [255,   0,   0]
        // 10 HEATER_OFF               #ffad33    [255, 173,  51]
        // N.B. Remember that HEATER cells have extended Moore neighbourhoods (r=4)
        else if(state.type == BimSimStateName::HEATER_ON || state.type == BimSimStateName::HEATER_OFF) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Case: HEATER_ON, so supply heat
            if(state.type == BimSimStateName::HEATER_ON) {
                // Supply heat from heater
                state.temperature += HEATER_TEMP_INCREASE;
                // Rather than incrementing it, use thermostat temp
                //state.temperature = TARGET_TEMP;
            }
            // Turn the heater ON or OFF as required
            updateHeaterCellState(state, occupant_neighbours);
        } 
        // Case: Wall
        // 11 WALL                     #000000    [  0,   0,   0]
        else if(state.type == BimSimStateName::WALL) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Apply external wall heat dissipation
            state.temperature -= dissipate(0.50, 1.00); // TODO: constants
        }
        // Case: Window
        // 12 WINDOW                   #ced4db    [206, 212, 219]
        else if(state.type == BimSimStateName::WINDOW) {
            // Take the average neighbourhood temperature as the new cell temperature
            state.temperature = neighbourhood_temperature / neighbours;
            // Apply heat dissipation
            state.temperature -= dissipate();
            // Apply window heat dissipation 
            state.temperature -= dissipate(1.00, 1.50); // TODO: constants
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
     * Update (mutate) state of HEATER cells based on occupancy and temperature targets.
     */
    void updateHeaterCellState(BimSimState& state, int occupants) const {     
        // If zone occupied and strictly less than target temperature, heater ON
        if (state.temperature < TARGET_TEMP && occupants)  {
            state.type = BimSimStateName::HEATER_ON;
        }
        // If greater than target temperature, heater OFF
        else {
            state.type = BimSimStateName::HEATER_OFF;
        }
    }

    /**
     * Update (mutate) state of OCCUPANT cells based on temperature targets.
     */
    void updateOccupantCellState(BimSimState& state) const {     
        // Occupant is within climate comfort zone
        if (state.temperature >= MIN_TARGET_TEMP && state.temperature <= MAX_TARGET_TEMP) {
            state.type = BimSimStateName::OCCUPANT_COMFORTABLE;
        }
        // Occupant is not within climate comfort zone
        else {
            state.type = BimSimStateName::OCCUPANT_UNCOMFORTABLE;
        }
    }

};

#endif // BIMSIM_CELL_HPP
