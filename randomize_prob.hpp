#ifndef randomize_prob_HPP
#define randomize_prob_HPP


#include <random> 
#include <math.h> 
#include "sat_prob.hpp"

bool vec_stored(sat_prob &A, std::vector<int> &T);
//if no k is passed as argument the number of literals in each clause is random
void randomize_prob(sat_prob &A, unsigned int num_var, unsigned int num_cl, unsigned int num_lit=0);

#endif
