////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RANDOMIZE PROB:: Generates given number of random clauses with given number of literals for a class A.				 //                                                                                                         
//                  If exact is zero or not given than the length of the clause is random between 1 and num_lit //                                                                                                                       
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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




void randomize_prob(sat_prob &A, unsigned int s = 1717, unsigned int num_lit=0, bool exact = true);

#endif
