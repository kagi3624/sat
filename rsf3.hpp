////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// RSF3:: Generates for given number of clauses and literals k = 3 a formula that is guaranteed satisfiable.      //                                                                                                                       
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef rsf3_HPP
#define rsf3_HPP


#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <math.h> 
#include <algorithm>
#include <iterator>
#include <unordered_set>
#include "sat_prob.hpp"
#include "randomize_prob.hpp"




void randomize_k3(sat_prob &A, int num_literals=3, int s = 1717);



#endif
