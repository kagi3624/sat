#include "asat.hpp"

using namespace boost::random;

//returns the indices of unsatisfied clauses
std::vector<int> find_unsatisfied_clauses(sat_prob &A, std::vector<int> &conf){

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
		if(l_value<r_value)
			uns_cl.push_back(i);
	}

	return uns_cl;
}

unsigned int find_num_ucl(sat_prob &A, std::vector<int> &conf){

	unsigned int N = 0;

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
		if(l_value<r_value)
			N++;
	}

	return N;
}


std::vector<int> solve_asat (sat_prob &A, double p){

	mt19937::result_type seed = time(0);
	mt19937 g(seed);
	
	//roll initial configuration
	std::vector<int> configuration;
	unsigned int t=0, t_max = 1000000;
	for(unsigned int i = 0; i<A.get_num_variables();++i)
		configuration.push_back(uniform_int_distribution<>(0, 1)(g));
	std::vector<int> changed_configuration = configuration;

	while(t<t_max){
		std::vector<int> U = find_unsatisfied_clauses(A,configuration);
		
		if(U.size() == 0){
			std::cout<<"t="<<t<<'\n';
			return configuration;	
		}
						
		std::size_t w = uniform_int_distribution<>(0,U.size()-1)(g);
		std::size_t z = uniform_int_distribution<>(0,A.get_clause(w).v.size()-1)(g);
		
		if(configuration[abs(A.get_clause(w).v[z])-1] == 0)	
			changed_configuration[abs(A.get_clause(w).v[z])-1] = 1;
		else
			changed_configuration[abs(A.get_clause(w).v[z])-1] = 0;
			
		unsigned int N = find_num_ucl(A,changed_configuration);
		if(N<=U.size()){
			configuration[abs(A.get_clause(w).v[z])-1]=changed_configuration[abs(A.get_clause(w).v[z])-1];
			std::cout<<"a"<<"    "<<N<<" "<<U.size()<<"  "<<t<<'\n'<<std::flush;
		}
		else if(uniform_real_distribution<>(0.0,1.0)(g)<=p){
			configuration[abs(A.get_clause(w).v[z])-1]=changed_configuration[abs(A.get_clause(w).v[z])-1];
			std::cout<<"b"<<"    "<<N<<" "<<U.size()<<"  "<<t<<'\n'<<std::flush;
		}
		else{
			std::cout<<"c    ------"<<"  "<<t<<'\n'<<std::flush;
		}
		
		t++;
	}
	std::cout<<"t_max reached\n";
	return configuration;
}


//./test 500 10000 450
