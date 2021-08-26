#ifndef _GALA_POTENTIALPARAMETER_H
#define _GALA_POTENTIALPARAMETER_H

#include <string>

namespace gala { namespace potential {

class BasePotentialParameter {

    public:
        // Attributes:
        std::string const m_name;

        // Methods:
        virtual double get_value(double t);

};

class StaticPotentialParameter : public BasePotentialParameter {

    public:
        // Attributes:
        double m_val;

        // Constructors:
        // TODO: implement int, long, double, array constructors!
        StaticPotentialParameter(double val);

        // Methods:
        double get_value(double t) override;

};

}} // namespace: gala::potential

#endif
