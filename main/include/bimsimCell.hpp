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
    // Temperature limits and targets
    static constexpr double MIN_TEMP = 19.00;
    static constexpr double MAX_TEMP = 26.00;
    static constexpr double MIN_TARGET_TEMP = 21.00;
    static constexpr double MAX_TARGET_TEMP = 24.00;
    static constexpr double TARGET_TEMP = (MAX_TARGET_TEMP + MIN_TARGET_TEMP) / 2;
    // Dissipation constants
    static constexpr double DEFAULT_DISSIPATION_MIN = 0.15;
    static constexpr double DEFAULT_DISSIPATION_MAX = 0.25;
    static constexpr double WALL_DISSIPATION_MIN = 1.25;
    static constexpr double WALL_DISSIPATION_MAX = 1.50;
    static constexpr double WINDOW_DISSIPATION_MIN = 1.75;
    static constexpr double WINDOW_DISSIPATION_MAX = 2.75;
    // Generation constants
    static constexpr double OCCUPANT_GENERATION_MIN = 0.20;
    static constexpr double OCCUPANT_GENERATION_MAX = 0.40;
    static constexpr double HEATER_GENERATION_MIN = 90.00;
    static constexpr double HEATER_GENERATION_MAX = 115.00;
    static constexpr double WINDOW_GENERATION_MIN = 0.10;
    static constexpr double WINDOW_GENERATION_MAX = 0.20;

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

        // Canvas this cell's neighborhood and record required values for determining
        // temperature and occupancy.
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
        // Take the average neighbourhood temperature as the new cell temperature
        state.temperature = neighbourhood_temperature / neighbours;

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
            // Dissipate and/or generate heat as required
            if (state.temperature >= MIN_TEMP) {
                state.dissipate(DEFAULT_DISSIPATION_MIN, DEFAULT_DISSIPATION_MAX);
            }
            // Update the state based on new temperature (for visualization)
            updateEmptyCellStateByTemperature(state);
        } 
        // Case: Occupants
        // 7  OCCUPANT_COMFORTABLE     #ffff66    [255, 255, 102]
        // 8  OCCUPANT_UNCOMFORTABLE   #cc33ff    [204,  51, 255]
        else if(state.type == BimSimStateName::OCCUPANT_COMFORTABLE || state.type == BimSimStateName::OCCUPANT_UNCOMFORTABLE) {
            // Dissipate and/or generate heat as required
            if (state.temperature >= MIN_TEMP) {
                state.dissipate(DEFAULT_DISSIPATION_MIN, DEFAULT_DISSIPATION_MAX);
            }
            state.generate(OCCUPANT_GENERATION_MIN, OCCUPANT_GENERATION_MAX);
            // Update the occupant's state based on climate comfort level
            updateOccupantCellState(state);
        }
        // Case: Active Heater
        // 9  HEATER_ON                #ff0002    [255,   0,   0]
        else if(state.type == BimSimStateName::HEATER_ON) {
            // Dissipate and/or generate heat as required
            if (state.temperature >= MIN_TEMP) {
                state.dissipate(DEFAULT_DISSIPATION_MIN, DEFAULT_DISSIPATION_MAX);
            }
            state.generate(HEATER_GENERATION_MIN, HEATER_GENERATION_MAX);
            // Turn the heater ON or OFF as required
            updateHeaterCellState(state, occupant_neighbours);
        } 
        // Case: Unactive Heater
        // 9  HEATER_OFF               #ff0002    [255,   0,   0]
        else if(state.type == BimSimStateName::HEATER_OFF) {
            // Dissipate and/or generate heat as required
            if (state.temperature >= MIN_TEMP) {
                state.dissipate(DEFAULT_DISSIPATION_MIN, DEFAULT_DISSIPATION_MAX);
            }
            // Turn the heater ON or OFF as required
            updateHeaterCellState(state, occupant_neighbours);
        } 
        // Case: Wall
        // 11 WALL                     #000000    [  0,   0,   0]
        else if(state.type == BimSimStateName::WALL) {
            // Dissipate and/or generate heat as required
            if (state.temperature >= MIN_TEMP) {
                state.dissipate(WALL_DISSIPATION_MIN, WALL_DISSIPATION_MAX);
            }
        } 
        // Case: Window
        // 12 WINDOW                   #ced4db    [206, 212, 219]
        else {
            // Dissipate and/or generate heat as required
            if (state.temperature >= MIN_TEMP) {
                state.dissipate(WINDOW_DISSIPATION_MIN, WINDOW_DISSIPATION_MAX);
            }
            state.generate(WINDOW_GENERATION_MIN, WINDOW_GENERATION_MAX);
        }

        // Return the (possibly) mutated state, with its updated temperature.
        return state;

    }

    /**
     * Delay function (D)
     */
    [[nodiscard]] double outputDelay(const BimSimState& state) const override {
        return DEFAULT_DELAY_TIME;
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
