#ifndef BIMSIM_STATE_HPP
#define BIMSIM_STATE_HPP

#include <iostream>
#include <nlohmann/json.hpp>

/**
 * Enum for easy state representation.
 * State names are commented with their colour encodings used in visualization.
 */
enum BimSimStateName {
    EMPTY_COLD_0,             // 0     #b3b3ff    [179, 179, 255]
    EMPTY_COLD_1,             // 1     #ccccff    [204, 204, 255]
    EMPTY_COLD_2,             // 2     #e6e6ff    [230, 230, 255]
    EMPTY_OK_3,               // 3     #ffffff    [255, 255, 255]    
    EMPTY_HOT_4,              // 4     #ffe6e6    [255, 230, 230]
    EMPTY_HOT_5,              // 5     #ffcccc    [255, 204, 204]
    EMPTY_HOT_6,              // 6     #ffb3b3    [255, 179, 179]
    OCCUPANT_COMFORTABLE,     // 7     #ffff66    [255, 255, 102]
    OCCUPANT_UNCOMFORTABLE,   // 8     #cc33ff    [204,  51, 255]
    HEATER_ON,                // 9     #ff0000    [255,   0,   0]
    HEATER_OFF,               // 10    #ffad33    [255, 173,  51]
    WALL,                     // 11    #000000    [  0,   0,   0]
    WINDOW                    // 11    #ced4db    [206, 212, 219]
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
    bool sensor;

    /**
     * Constructor
     */
    BimSimState() : type(BimSimStateName::EMPTY_OK_3)
        , temperature(DEFAULT_TEMP)
        , sensor(false)
    {}

    /**
     * Methods
     */

    /**
     * Passively dissipate heat
     */
    void dissipate(double min, double max) {     
        temperature -= randomDouble(min, max);
    }
    
    /**
     * Actively generate heat
     */
    void generate(double min, double max) {     
        temperature += randomDouble(min, max);
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

};

/**
 * operator<< overload definition.
 *
 * Defines the output format of the BimSimState enum class in order to
 * print the cell state.
 *
 *  <0>  : EMPTY_COLD_0             #b3b3ff    [179, 179, 255]
 *  <1>  : EMPTY_COLD_1             #ccccff    [204, 204, 255]
 *  <2>  : EMPTY_COLD_2             #e6e6ff    [230, 230, 255]
 *  <3>  : EMPTY_OK_3               #ffffff    [255, 255, 255]    
 *  <4>  : EMPTY_HOT_4              #ffe6e6    [255, 230, 230]
 *  <5>  : EMPTY_HOT_5              #ffcccc    [255, 204, 204]
 *  <6>  : EMPTY_HOT_6              #ffb3b3    [255, 179, 179]
 *  <7>  : OCCUPANT_COMFORTABLE     #ffff66    [255, 255, 102]
 *  <8>  : OCCUPANT_UNCOMFORTABLE   #cc33ff    [204,  51, 255]
 *  <9>  : HEATER_ON                #ff0000    [255,   0,   0]
 *  <10> : HEATER_OFF               #ffad33    [255, 173,  51]
 *  <11> : WALL                     #000000    [  0,   0,   0]
 *  <12> : WINDOW                   #ced4db    [206, 212, 219]
 */
std::ostream& operator<<(std::ostream& os, const BimSimState& s) {
    os << "<" << s.type << ">";
    return os;
}

/**
 * operator!= overload definition.
 *
 * Enables simulator to check for equality between two state objects.
 * Equality is determined by state and temperature.
 */
bool operator!=(const BimSimState& x, const BimSimState& y) {
    double tolerance = 0.0001;
    return x.type != y.type || std::abs(x.temperature - y.temperature) > tolerance;
}

/**
 * Parse a JSON file to populate a cell's initial state.
 */
void from_json(const nlohmann::json& j, BimSimState& s) {
    j.at("type").get_to(s.type);
    j.at("temperature").get_to(s.temperature);
    j.at("sensor").get_to(s.sensor);
}

#endif // BIMSIM_STATE_HPP
