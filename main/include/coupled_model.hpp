#ifndef CmName_HPP
#define CmName_HPP

// Cadmium V2
#include "cadmium/modeling/devs/coupled.hpp"

// Project atopmic models
#include "atomic_model.hpp"

using namespace cadmium;

struct CmName : public Coupled {

    /**
     * Constructor for top coupled model.
     *
     * @param id ID of the model.
     */
    CmName(const std::string& id) : Coupled(id) {
        // Model declaration
        auto model1 = addComponent<Model1>("Model1");
        auto model2 = addComponent<Model2>("Model2");

        // Port coupling
        addCoupling(model1->out, model2->in);
    }

};

#endif
