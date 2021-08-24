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

        virtual double _density(double *q, double t);
        virtual double _energy(double *q, double t);
        // void _gradient(double *q, double t, double *grad);
        // void _hessian(double *q, double t, double *hess);

};

class KeplerPotential : public BasePotential {

    public:
        using BasePotential::BasePotential;
        double _density(double *q, double t) override;
        double _energy(double *q, double t) override;
        // void _gradient(double *q, double t, double *grad) override;
        // void _hessian(double *q, double t, double *hess) override;
};

}} // namespace: gala::potential
