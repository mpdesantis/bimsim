#ifndef BIMSIM_STATE_HPP
#define BIMSIM_STATE_HPP

#include <iostream>
#include <nlohmann/json.hpp>

/**
 * Enum for easy state representation.
 */
enum BimSimStateName {
    EMPTY_COLD_0,  // 0
    EMPTY_COLD_1,  // 1
    EMPTY_COLD_2,  // 2
    EMPTY_OK_3,    // 3 // DEFAULT
    EMPTY_HOT_4,   // 4
    EMPTY_HOT_5,   // 5
    EMPTY_HOT_6,   // 6
    WALL,          // 7     
    OCCUPANT,      // 8
    HEATER_ON,     // 9
    HEATER_OFF,    // 10
    WINDOW         // 11
};


/**
 * Atomic model cell state variables.
 */
struct BimSimState {
   
    /**
     * Constants
     */
    static constexpr double DEFAULT_TEMP = 22.777;

    /**
     * Members
     */
    int type;
    double temperature;

    /**
     * Constructor
     */
    BimSimState() : type(BimSimStateName::EMPTY_OK_3)
        , temperature(DEFAULT_TEMP)
    {}
};

/**
 * operator<< overload definition.
 *
 * Defines the output format of the BimSimState enum class in order to
 * print the cell state.
 *  <0>  : EMPTY_COLD_0
 *  <1>  : EMPTY_COLD_1
 *  <2>  : EMPTY_COLD_2
 *  <3>  : EMPTY_OK_3
 *  <4>  : EMPTY_HOT_4
 *  <5>  : EMPTY_HOT_5
 *  <6>  : EMPTY_HOT_6
 *  <7>  : WALL
 *  <8>  : OCCUPANT
 *  <9>  : HEATER_ON
 *  <10> : HEATER_OFF
 *  <11> : WINDOW
 */
std::ostream& operator<<(std::ostream& os, const BimSimState& s) {
    os << "<" << s.type << ">";
    return os;
}

/**
 * operator!= overload definition.
 *
 * Enables simulator to check for equality between two state objects.
 */
bool operator!=(const BimSimState& x, const BimSimState& y) {
    // N.B. Explicitly check EVERYTHING you are interested in!!!!!!!
    // This is a sneaky bug hotspot.
    double tolerance = 0.0001;
    return x.type != y.type || std::abs(x.temperature - y.temperature) > tolerance;
}

/**
 * Parse a JSON file to populate a cell's state.
 */
void from_json(const nlohmann::json& j, BimSimState& s) {
    j.at("type").get_to(s.type);
    j.at("temperature").get_to(s.temperature);
}

#endif // BIMSIM_STATE_HPP
