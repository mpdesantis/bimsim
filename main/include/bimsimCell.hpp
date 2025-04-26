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
    static constexpr double MIN_TARGET_TEMP = 21.00;
    static constexpr double MAX_TARGET_TEMP = 24.00;
    static constexpr double TEMP_STEP = 1.00; // Step for up and down the state scale
    // TODO: randomize within interval
    static constexpr double HEATER_TEMP_INCREASE = 1.00;

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
        double neighbourhood_temperature = 0.0;

        /* Canvas the Neighborhood */

        // Canvas this cell's neighborhood to categorize neighbor types
        // ie. tally neighbor cells by type type
        for (const auto& [neighborId, neighborData]: neighborhood) {
            // State of the neighbor cell for this iteration
            auto nState = neighborData.state;

            // Accumulate neighbours
            // N.B. We may not have a full neighbourhood at borders, so can't assume.
            neighbours++;
            // Accumulate temperature
            neighbourhood_temperature += nState->temperature;

        }

        /* Mutate State Based on Rules and Return */

        // Case: EMPTY cell [0, 7]
        if(state.type >= BimSimStateName::EMPTY_COLD_0 && state.type <= BimSimStateName::EMPTY_HOT_6) {
            // Take the average neighbourhood temperature as the new cell temperature
            std::cout << state.type << " cell temp updated: " << state.temperature;
            state.temperature = neighbourhood_temperature / neighbours;
            std::cout << " --> " << state.temperature << std::endl;
            state.temperature -= randomDouble();
            // Update the state based on new temperature for visualization
            updateEmptyCellStateByTemperature(state);
        } 
        // Case: HEATER cell
        else if(state.type == BimSimStateName::HEATER) {
            // Take the average neighbourhood temperature as the new cell temperature,
            // but add some heat from the heater.
            std::cout << state.type << " cell temp updated: " << state.temperature;
            state.temperature = neighbourhood_temperature / neighbours;
            std::cout << " --> " << state.temperature;
            state.temperature += HEATER_TEMP_INCREASE;
            std::cout << " --> " << state.temperature << std::endl;
            // Don't update state, since a HEATER is a HEATER no matter how hot!
        } 
        // Else: TODO: Remove
        else {
            state.temperature = neighbourhood_temperature / neighbours;
        }

        // Return the (possibly) mutated state
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
     * Get a random double in limits
     */
    double randomDouble() const {     
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 2.0);
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
};

#endif // BIMSIM_CELL_HPP
