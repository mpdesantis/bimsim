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

        // Accumulators for all neighbor types (differentiated by terrain)
        int water_neighbors = 0;
        int land_neighbors = 0;
        int forest_neighbors = 0;
        int sand_neighbors = 0;
        int non_water_neighbors = 0;

        /* Canvas the Neighborhood */

        // Canvas this cell's neighborhood to categorize neighbor types
        // ie. tally neighbor cells by terrain type
        for (const auto& [neighborId, neighborData]: neighborhood) {
            // State of the neighbor cell for this iteration
            auto nState = neighborData.state;

            // WATER neighbors
            if(nState->terrain == BimSimStateName::WATER) {
                water_neighbors++;
            }
            // LAND neighbors
            if(nState->terrain == BimSimStateName::LAND) {
                land_neighbors++;
            }
            // FOREST neighbors
            if(nState->terrain == BimSimStateName::FOREST) {
                forest_neighbors++;
            }
            // SAND neighbors
            if(nState->terrain == BimSimStateName::SAND) {
                sand_neighbors++;
            }
        }

        /* Mutate State Based on Rules and Return */

        // Case: WATER cell
        if(state.terrain == BimSimStateName::WATER) {
            // Uncount this cell from its own neighborhood tally
            water_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + sand_neighbors;

            // Case: WATER --> LAND
            if(non_water_neighbors > state.land_birth_limit) {
                // A LAND cell is born
                state.terrain = BimSimStateName::LAND; 
            }
        } 

        // Case: LAND cell
        else if (state.terrain == BimSimStateName::LAND) {
            // Uncount this cell from its own neighborhood tally
            land_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + sand_neighbors;

            // Case: LAND --> WATER
            if(non_water_neighbors < state.land_death_limit) {
                // A LAND cell dies
                state.terrain = BimSimStateName::WATER;
            }

            // Case: LAND --> SAND | LAND
            // SAND can only form near WATER (ie. at least one WATER neighbor)
            else if (water_neighbors) {
                // Get random number in [0, 1] to test against SAND rules
                double r = randomProbability();
                // Get base threshold for becoming SAND
                double sand_threshold = state.sand_base_rate;
                // SAND is more likely to form near WATER.
                // Apply multiplier to increase chance of becoming SAND
                // based on number of WATER neighbours
                sand_threshold += state.sand_multiplier * water_neighbors;
                // Case: This cell is below threshold, so it becomes SAND
                if (r <= sand_threshold) {
                    // A SAND cell is born
                    state.terrain = BimSimStateName::SAND;
                }
                // Case: This cell remains LAND
            }

            // Case: LAND --> FOREST | LAND
            // Forests only grow in interior (ie. no WATER neighbors)
            else {
                // Get random number in [0, 1] to test against FOREST rules
                double r = randomProbability();
                // Get base threshold for becoming FOREST
                double forest_threshold = state.forest_base_rate;
                // FOREST is more likely to form near FOREST.
                // Apply multiplier to increase chance of becoming FOREST
                // based on number of FOREST neighbors.
                forest_threshold += state.forest_multiplier * forest_neighbors;
                // Case: This cell is below threshold, so it becomes FOREST
                if (r <= forest_threshold) {
                    // A FOREST cell is born
                    state.terrain = BimSimStateName::FOREST;
                }
                // Case: This cell remains LAND
            }

        }

        // Case: FOREST cell
        // FOREST can only revert to LAND
        else if (state.terrain == BimSimStateName::FOREST) {
            // Uncount this cell from its own neighborhood tally
            forest_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + sand_neighbors;

            // Case: FOREST --> LAND
            // If too much WATER nearby, revert to LAND
            if(water_neighbors > state.forest_death_limit) {
                // A FOREST cell reverts to LAND
                state.terrain = BimSimStateName::LAND;
            }
        }

        // Case: SAND cell
        // SAND can only revert to LAND
        else if (state.terrain == BimSimStateName::SAND) {
            // Uncount this cell from its own neighborhood tally
            sand_neighbors--; 
            // Tally the alive (ie. non-WATER neighbors) for rule use
            non_water_neighbors = land_neighbors + forest_neighbors + sand_neighbors;

            // Case: SAND --> LAND
            // If not enough WATER nearby, revert to LAND
            if(water_neighbors < state.sand_death_limit) {
                // A SAND cell reverts to LAND
                state.terrain = BimSimStateName::LAND;
            }
        }

        // Case: LAND --> LAND

        return state;
    }

    /**
     * Delay function (D)
     */
    [[nodiscard]] double outputDelay(const BimSimState& state) const override {
        return DEFAULT_DELAY_TIME;
    }

    /**
     * Get a random double within defined limits
     */
    double randomProbability() const {     
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        return dis(gen);
    }

    /**
     * Update temperature
     */
    double updateTemperature(const BimSimState& state) const {     
        double newTemperature;
        return 1.0; 
    }
};

#endif // BIMSIM_CELL_HPP
