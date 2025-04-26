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
    SENSOR,        // 8
    HEATER,        // 9
    WINDOW         // 10
};


/**
 * Atomic model cell state variables.
 */
struct BimSimState {
    
    /**
     * Constants
     */
    static constexpr double DEFAULT_TEMP = 22.5;

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
 *  <0> : EMPTY_COLD_3
 *  <1> : TODO
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
    return x.type != y.type;
}

/**
 * Parse a JSON file to populate a cell's state.
 */
void from_json(const nlohmann::json& j, BimSimState& s) {
    j.at("type").get_to(s.type);
    j.at("temperature").get_to(s.temperature);
}

#endif // BIMSIM_STATE_HPP
