#ifndef asat_HPP
#define asat_HPP

#include <utility>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "sat_prob.hpp"

std::vector<int> solve_asat (sat_prob &A, double p);

#endif 
