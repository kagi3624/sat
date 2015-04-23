#ifndef asat_HPP
#define asat_HPP

#include <math.h> 
#include <utility>
#include <algorithm>
#include <iterator>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "sat_prob.hpp"

std::vector<int> solve_asat (sat_prob &A, double p=0.3);

#endif 
