#include "asat.hpp"

using namespace boost::random;

std::pair<std::vector<int>, int> find_unsatisfied_clauses(sat_prob &A, std::vector<int> &conf){
	
	//the number of unsatisfied clauses
	unsigned int N = 0; 
	
	//indices of unsatisfied clauses
	std::vector<int> uns_cl;

	for(unsigned int i = 0; i<A.get_num_clauses();++i){
		int l_value = 0;
		int r_value = 1;
		for(unsigned int j = 0; j<A.get_clause(i).v.size();++j){
			if(A.get_clause(i).get_literal(j)<0){
				l_value -= conf[-A.get_clause(i).get_literal(j)-1];
				r_value--;
			}
			else
				l_value += conf[A.get_clause(i).get_literal(j)-1];
		}
		if(l_value<r_value){
			N++;
			uns_cl.push_back(i);
		}
	}
	std::pair<std::vector<int>, int> K;
	K = std::make_pair(uns_cl,N);
	return K;
}


std::vector<int> solve_asat (sat_prob &A, double p){

	mt19937::result_type seed = 6;
	mt19937 g(seed);
	
	//initial configuration
	std::vector<int> init_conf;
	for(unsigned int i = 0; i<A.get_num_variables();++i)
		init_conf.push_back(uniform_int_distribution<>(0, 1)(g));
	
	for(unsigned int i = 0; i<init_conf.size();++i)
		std::cout<<init_conf[i]<<" ";
	std::cout<<'\n';
	
	std::pair<std::vector<int>, int> P = find_unsatisfied_clauses(A,init_conf);
	std::vector<int> configuration;
	std::cout<<P.first.size()<<" "<<P.second<<'\n';
	for(int i = 0;i<P.first.size();++i)
		std::cout<<P.first[i]<<" ";
	std::cout<<'\n';
	return configuration;
}
