namespace gala {
namespace potential {

class BasePotential {

    public:
        int m_ndim;

        BasePotential(int ndim=3);
        int get_ndim() const;

};

}} // namespace: gala::potential
