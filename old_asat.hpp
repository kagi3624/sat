#ifndef old_asat_HPP
#define old_asat_HPP

#include <cmath>
#include <algorithm> 
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "sat_prob.hpp"

std::vector<int> old_asat(sat_prob &A, const unsigned int s = 1717, double p = 0.21); 


#endif
