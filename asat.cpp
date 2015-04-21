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

	return std::make_pair(uns_cl,N);
}


std::vector<int> solve_asat (sat_prob &A, double p){

	mt19937::result_type seed = 6;
	mt19937 g(seed);
	
	//roll initial configuration
	std::vector<int> configuration;
	unsigned int t=0, t_max = 1000;
	for(unsigned int i = 0; i<A.get_num_variables();++i)
		configuration.push_back(uniform_int_distribution<>(0, 1)(g));
	std::vector<int> changed_configuration = configuration;

	while(t<t_max){
		std::pair<std::vector<int>, int> U = find_unsatisfied_clauses(A,configuration);
		
		if(U.second == 0)
			return configuration;
		
		/*for(std::size_t i=0;i<U.first.size();++i)
			std::cout<<U.first[i]<<" ";
		std::cout<<'\n';*/
		
			
		std::size_t w = uniform_int_distribution<>(0,U.first.size()-1)(g);
		std::size_t z = uniform_int_distribution<>(0,A.get_clause(w).v.size()-1)(g);
		
		if(configuration[abs(A.get_clause(w).v[z])-1] == 0)	
			changed_configuration[abs(A.get_clause(w).v[z])-1] = 1;
		else
			changed_configuration[abs(A.get_clause(w).v[z])-1] = 0;
			
		std::pair<std::vector<int>, int> R = find_unsatisfied_clauses(A,changed_configuration);
		if(R.second<=U.second)
			configuration[abs(A.get_clause(w).v[z])-1]=changed_configuration[abs(A.get_clause(w).v[z])-1];
		else if(uniform_real_distribution<>(0.0,1.0)(g)<=p)
			configuration[abs(A.get_clause(w).v[z])-1]=changed_configuration[abs(A.get_clause(w).v[z])-1];
		
		t++;
	}
	if(t==t_max)
		std::cout<<"t_max reached\n";
	std::cout<<"t="<<t<<'\n';
	return configuration;
}


//./test 500 10000 450
