/*
    TODO:
*/

#include <iostream>
#include <cmath>
#include <cpp_gala/df/streamdf.h>

using namespace gala::utils;
using namespace gala::df;

double get_jacobi_radius(gala::potential::BasePotential &host_potential,
                         gala::simulation::ParticleCollection &progenitor,
                         double t) {
    // TODO: need to enforce that sat potential is not composite
    auto *sat_potential = reinterpret_cast<gala::potential::BaseBuiltinPotential *>(progenitor.potential);

    // TODO: should add more robust (but slower) ways of computing the Jacobi radius
    if (sat_potential->parameters.find("M") == sat_potential->parameters.end())
        throw std::invalid_argument("NotImplementedError: Currently, stream DFs only work for "
                                    "progenitors with mass parameters");

    auto mass_param = sat_potential->parameters.find("M")->second;
    auto m = mass_param->get_value(t);

    auto Menc = host_potential.mass_enclosed(progenitor.w);
    double R = norm(progenitor.w[0]);

    return pow(m / Menc, 1. / 3.) * R; // TODO: could multiple by f - see B&T
}

vector_1d get_unit_vector(vector_1d &vec) {
    vector_1d uvec;
    double R = norm(vec);
    for (int i=0; i < vec.size(); i++)
        uvec.push_back(vec[i] / R);
    return uvec;
}

/*
    Base class
*/
BaseStreamDF::BaseStreamDF(bool lead, bool trail, int seed) {
    this->lead = lead;
    this->trail = trail;
    this->seed = seed;
    // TODO: set up rng

    if ((this->lead == false) && (this->lead == false))
        throw std::invalid_argument("lead and trail cannot both be false");
}

vector_2d BaseStreamDF::sample(gala::potential::BasePotential &potential,
                               gala::simulation::ParticleCollection &progenitor,
                               uint32_t n_particles,
                               double t) { }


/*
    Streakline
*/
StreaklineStreamDF::StreaklineStreamDF(bool lead, bool trail, int seed)
: BaseStreamDF(lead, trail, seed) {}

vector_2d StreaklineStreamDF::sample(gala::potential::BasePotential &potential,
                                     gala::simulation::ParticleCollection &progenitor,
                                     uint32_t n_particles,
                                     double t) {
    vector_2d particles_w;
    vector_1d tmp_w;

    auto Rjac = get_jacobi_radius(potential, progenitor, t);
    auto r_uvec = get_unit_vector(progenitor.get_x()[0]);
    auto v_uvec = get_unit_vector(progenitor.get_v()[0]);
    auto L = cross(progenitor.w[0]);

    double Omega = norm(L) / pow(norm(progenitor.w[0]), 2);
    double dv = Omega * Rjac;

    for (int n=0; n < n_particles; n++) {
        if (this->lead) {
            tmp_w[n] = progenitor.w[0][n] - Rjac * r_uvec[n];
            tmp_w[n+3] = progenitor.w[0][n+3] + dv * v_uvec[n];
            particles_w.push_back(tmp_w);
        }
        if (this->trail) {
            tmp_w[n] = progenitor.w[0][n] + Rjac * r_uvec[n];
            tmp_w[n+3] = progenitor.w[0][n+3] - dv * v_uvec[n];
            particles_w.push_back(tmp_w);
        }
    }

}