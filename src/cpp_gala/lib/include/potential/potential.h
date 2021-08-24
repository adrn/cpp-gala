#include <string>

namespace gala {
namespace potential {


class PotentialParameter {

    public:
        // Attributes:
        std::string const m_name;

        // Constructors:
        PotentialParameter(std::string name);

        // Methods:
        double get_value(double t);

};


class BasePotential {

    public:
        // Attributes:
        int m_ndim;

        // Constructors:
        BasePotential(int ndim=3);

        // Methods::
        int get_ndim() const;

        double _density(double *q, double t);
        double _energy(double *q, double t);
        void _gradient(double *q, double t, double *grad);
        void _hessian(double *q, double t, double *hess);

};

class KeplerPotential : public BasePotential {
    virtual double _density(double *q, double t);
    virtual double _energy(double *q, double t);
    virtual void _gradient(double *q, double t, double *grad);
    virtual void _hessian(double *q, double t, double *hess);
};

}} // namespace: gala::potential
