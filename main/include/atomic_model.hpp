#ifndef ATOMIC_MODEL_HPP
#define ATOMIC_MODEL_HPP

// Standard includes
#include <random>
#include <iostream>

// Cadmium V2 includes
#include "cadmium/modeling/devs/atomic.hpp"

// Namespaces
using namespace cadmium;

/**
 * Atomic model state variables
 */
struct atomic_modelState {
    /**
     * Members
     */
    double sigma;

    /**
     * Constructor
     */
    explicit atomic_modelState(): sigma(1){
    }
};

/**
 * operator<< overload definition.
 *
 * Defines the output format to a given file stream uwsing this model's state variables.
 */
std::ostream& operator<<(std::ostream &out, const atomic_modelState& state) {
    out  << "{" << state.sigma << "}";
    return out;
}

/**
 * Atomic model class definition.
 *
 * Extends the Cadmium V2 `Atomic` class.
 */
class atomic_model : public Atomic<atomic_modelState> {

    public:

    /**
     * Member ports
     */

    /**
     * Constructor
     */
    atomic_model(const std::string id) : Atomic<atomic_modelState>(id, atomic_modelState()) {
        // Initialize ports

    }

    /**
     * Internal transition function (delta_int)
     */
    void internalTransition(atomic_modelState& state) const override {

        // Advance sigma
        state.sigma += 1;
    }

    /**
     * External transition function (delta_ext)
     */
    void externalTransition(atomic_modelState& state, double e) const override {

        // Update sigma based on elapsed duration
        state.sigma -= e; 
    }
    
    
    /**
     * Output function (lambda)
     */
    void output(const atomic_modelState& state) const override {

    }

    /**
     * Time advance function (ta)
     */
    [[nodiscard]] double timeAdvance(const atomic_modelState& state) const override {     
            return state.sigma;
    }

};

#endif
