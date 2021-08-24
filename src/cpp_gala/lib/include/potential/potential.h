#include <string>
#include <map>

namespace gala {
namespace potential {

/*
    Parameters
*/

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


/*
    Potentials
*/

class BasePotential {

    public:
        // Attributes:
        int m_ndim;
        std::map<std::string, BasePotentialParameter*> parameters;

        // Constructors:
        BasePotential(int ndim=3);

        // Methods::
        int get_ndim() const;

        virtual double _density(double *q, double t);
        virtual double _energy(double *q, double t);
        // void _gradient(double *q, double t, double *grad);
        // void _hessian(double *q, double t, double *hess);

};

class KeplerPotential : public BasePotential {

    public:
        // Constructors:
        KeplerPotential(BasePotentialParameter *GM, int ndim=3);

        // Methods::
        double _density(double *q, double t) override;
        double _energy(double *q, double t) override;
        // void _gradient(double *q, double t, double *grad) override;
        // void _hessian(double *q, double t, double *hess) override;
};

}} // namespace: gala::potential
