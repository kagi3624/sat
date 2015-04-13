#include "asat.hpp"

using namespace boost::random;

std::pair<std::vector<int>, int> find_unsatisfied_clauses(sat_prob A){
	
//TO DO

}


std::vector<int> solve_asat (sat_prob &A, double p){

	mt19937::result_type seed = 6;
	mt19937 g(seed);
	
	std::vector<int> configuration;
	for (unsigned int i = 0; i<A.get_num_variables();++i){
		configuration.push_back(uniform_int_distribution<>(0, 1)(g));
	}
	
	std::pair<std::vector<int>, int> P = find_unsatisfied_clauses(A);

	return configuration;
}
