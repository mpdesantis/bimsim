#ifndef MAPGEN_STATE_HPP
#define MAPGEN_STATE_HPP

#include <iostream>
#include <nlohmann/json.hpp>

/**
 * Enum for easy state representation.
 */
enum MapgenStateName {
    WATER,  // 0
    LAND,   // 1
    FOREST, // 2
    SAND    // 3
};


/**
 * Atomic model cell state variables.
 */
struct MapgenState {
    /**
     * Members
     */
    int terrain;
    int land_birth_limit;
    int land_death_limit;
    int forest_death_limit;
    int sand_death_limit;
    double forest_base_rate;
    double forest_multiplier;
    double sand_base_rate;
    double sand_multiplier;

    /**
     * Constructor
     */
    MapgenState() : terrain(MapgenStateName::WATER)
        , land_birth_limit(6)
        , land_death_limit(4)
        , forest_death_limit(3)
        , sand_death_limit(2)
        , forest_base_rate(0.10)
        , forest_multiplier(0.12)
        , sand_base_rate(0.15)
        , sand_multiplier(0.05)
    {}
};

/**
 * operator<< overload definition.
 *
 * Defines the output format of the MapgenState enum class in order to
 * print the cell state.
 *  <0> : WATER
 *  <1> : LAND
 *  <2> : FOREST
 *  <3> : SAND
 */
std::ostream& operator<<(std::ostream& os, const MapgenState& s) {
    os << "<" << s.terrain << ">";
    return os;
}

/**
 * operator!= overload definition.
 *
 * Enables simulator to check for equality between two state objects.
 */
bool operator!=(const MapgenState& x, const MapgenState& y) {
    return x.terrain != y.terrain;
}

/**
 * Parse a JSON file to populate a cell's state.
 */
void from_json(const nlohmann::json& j, MapgenState& s) {
    // terrain
    j.at("terrain").get_to(s.terrain);
    j.at("land_birth_limit").get_to(s.land_birth_limit);
    j.at("land_death_limit").get_to(s.land_death_limit);
    j.at("forest_death_limit").get_to(s.forest_death_limit);
    j.at("sand_death_limit").get_to(s.sand_death_limit);
    j.at("forest_base_rate").get_to(s.forest_base_rate);
    j.at("forest_multiplier").get_to(s.forest_multiplier);
    j.at("sand_base_rate").get_to(s.sand_base_rate);
    j.at("sand_multiplier").get_to(s.sand_multiplier);
}

#endif // MAPGEN_STATE_HPP
