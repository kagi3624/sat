#include "asat.hpp"

using namespace boost::random;

//returns the indices of unsatisfied clauses
void find_unsatisfied_clauses(const sat_prob &A, const std::vector<int> &conf, std::vector<int> &R){

	R.clear();

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
			R.push_back(i);
	}
}

//returns indices of all clauses where variable x is presentet
std::vector<std::vector<int> > find_clauses_for_var(const sat_prob &A){

	std::vector<std::vector<int> > V(A.get_num_variables());	
	
	
		for(std::size_t j = 0;j<A.get_num_clauses();++j){
			for(std::size_t k = 0; k<A.get_clause(j).v.size();++k){
				V[abs(A.get_clause(j).v[k])-1].push_back(j);
			}
		}
	return V;
}

std::vector<int> solve_asat (sat_prob &A, double p){

	mt19937::result_type seed = 6;
	mt19937 g(seed);
	
	//roll initial configuration
	std::vector<int> R,U;
	std::vector<int> configuration;
	
	std::vector<std::vector<int> > Cfx = find_clauses_for_var(A);

	for(unsigned int i = 0; i<A.get_num_variables();++i)
		configuration.push_back(uniform_int_distribution<>(0, 1)(g));
		
	std::vector<int> changed_configuration = configuration;
	find_unsatisfied_clauses(A,configuration,U);

	unsigned int t_max = 1000000000;

	for(size_t t = 0; t<t_max;++t){
	
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
			
		find_unsatisfied_clauses(A,changed_configuration,R);
		if(R.size()<=U.size()){
			configuration[abs(A.get_clause(w).v[z])-1]=changed_configuration[abs(A.get_clause(w).v[z])-1];
			//std::cout<<"a"<<"    "<<R.size()<<" "<<U.size()<<"  "<<t<<'\n'<<std::flush;
			U = R;
		}
		else if(uniform_real_distribution<>(0.0,1.0)(g)<=p){
			configuration[abs(A.get_clause(w).v[z])-1]=changed_configuration[abs(A.get_clause(w).v[z])-1];
			//std::cout<<"b"<<"    "<<R.size()<<" "<<U.size()<<"  "<<t<<'\n'<<std::flush;
			U = R;
		}
		/*else{
			std::cout<<"c    ------"<<"  "<<t<<'\n'<<std::flush;
		}*/
	}
	std::cout<<"t_max reached\n";
	return configuration;
}

