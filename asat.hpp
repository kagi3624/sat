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

//stores the number of satisfied variables for each clause
struct F{
	// Number of unsatisfied clauses
	size_t N;
	size_t N_temp;
	// stores the number of literals which satisfy the clause
	std::vector<int> vec;
	// stores indices of unsatisfied clauses
	std::vector<int> U;
	std::vector<int> POS;
	
	F(sat_prob &A, const std::vector<int> &configuration);
	
	void rm_satisf (sat_prob &A, int x);
};



std::vector<int> solve_by_asat (sat_prob &A, unsigned int s = 1717, double p=0.3);

#endif 
