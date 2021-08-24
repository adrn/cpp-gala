namespace gala {
namespace potential {

class BasePotential {

    public:
        int m_ndim;

        BasePotential(int ndim=3);
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
