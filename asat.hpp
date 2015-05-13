#ifndef asat_HPP
#define asat_HPP

#include <fstream>
#include <cmath>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include "sat_prob.hpp"


class asat{

public:

	size_t E;																// the current number of unsatisfied clauses
	size_t E_flip;													// the number of unsatisfied clauses after a flip
	int x;																	// index of current flipped variable
	
	std::vector<int> configuration;
	std::vector<int> K;  										// stores the multiplicity of satisfaction for each clause
	std::vector<int> U;											// stores the indices of current unsatisfied clauses
	std::vector<unsigned int> POS;										// stores the position of the unsatisfied clauses in U
	std::vector<int> B1;										// stores the indices of clauses which have been removed from U
	std::vector<int> B2;										// stores the indices of clauses which have been added to U
	std::vector<std::vector<int> > R1, R2;	// respectively sotres the indices of clauses in which x_i/̄x_i is present 
	
	template <typename URNG> 
	asat(sat_prob &A, URNG &g){

		for(size_t i = 0; i<A.get_num_variables();++i){
			configuration.resize(A.get_num_variables());
			configuration[i] = boost::random::uniform_int_distribution<>(0, 1)(g);
		}
		
		E = 0;
		K.resize(A.get_num_clauses());
		U.reserve(A.get_num_clauses());
		POS.resize(A.get_num_clauses(), -1);
		R1.resize(A.get_num_variables());
		R2.resize(A.get_num_variables());
		B1.reserve(A.get_num_clauses());
		B2.reserve(A.get_num_clauses());
	
		try{
			for(size_t i = 0; i<A.get_num_clauses();++i){
				for(size_t j = 0; j<A.get_clause(i).v.size();++j)	
					if((A.get_clause(i).v[j] < 0 && configuration[-A.get_clause(i).v[j]-1] == 0) || (A.get_clause(i).v[j] > 0 && configuration[A.get_clause(i).v[j]-1] ==1))
						K[i]++;
	
				if(K[i] == 0){			
					U.push_back(i);
					POS[i] = E;
					E++;
				}	
			}
		}
		catch(char const* s) {std::cerr<<s<<'\n';}
		catch (...) {std::cerr << "Unknown exception caught" <<'\n';}

	}	
	
	template <typename URNG> 
	void flip(sat_prob &A, URNG &g){
	
		E_flip = E;
		
		B1.clear();
		B2.clear();
		
		size_t w = boost::random::uniform_int_distribution<>(0,U.size()-1)(g);
		size_t z = boost::random::uniform_int_distribution<>(0,A.get_clause(w).v.size()-1)(g);
		
		x = abs(A.get_clause(w).v[z])-1;
		
		if(configuration[x] == 0){	
			configuration[x] = 1;
			
			run_increasing_satisfiability(R1[x]);		
			run_decreasing_satisfiability(R2[x]);
		
		}
		else{
			configuration[x] = 0;
			
			run_increasing_satisfiability(R2[x]);
			run_decreasing_satisfiability(R1[x]);
			
		}
	}
	
	void remove(size_t i);
	void add(size_t i);
	void run_increasing_satisfiability(const std::vector<int> &R);
	void run_decreasing_satisfiability(const std::vector<int> &R);
	void flip_back();
	
};



std::vector<int> solve_by_asat (sat_prob A, const unsigned int s = 1717, const double p=0.21, const bool b = false);

#endif 
