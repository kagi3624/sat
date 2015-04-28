#ifndef randomize_prob_HPP
#define randomize_prob_HPP


#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <math.h> 
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include "sat_prob.hpp"


//if no k is passed as argument the number of literals in each clause is random
void randomize_prob(sat_prob &A, unsigned int num_lit=0, int exact = 0);

#endif
