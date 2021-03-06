#include "old_asat.hpp"

using namespace boost::random;

void find_unsatisfied_clauses(const sat_prob &A, const std::vector<int> &conf, std::vector<int> &R){


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

void find_unsatisfied_clauses(const sat_prob A, const std::vector<int> &conf, std::vector<int> &R, const std::vector<int> &U, const std::vector<int> &W){

	R.clear();

	std::vector<int> merged;
	std::set_union(U.begin(), U.end(), W.begin(), W.end(), std::back_inserter(merged));
	
	

	for(unsigned int i = 0; i<merged.size();++i){
		int l_value = 0;
		int r_value = 1;
		for(unsigned int j = 0; j<A.get_clause(merged[i]).v.size();++j){
			if(A.get_clause(merged[i]).get_literal(j)<0){
				l_value -= conf[-A.get_clause(merged[i]).get_literal(j)-1];
				r_value--;
			}
			else
				l_value += conf[A.get_clause(merged[i]).get_literal(j)-1];
		}
		if(l_value<r_value)
			R.push_back(merged[i]);
	}
}

//returns indices of all clauses where variable x is present
std::vector<std::vector<int> > old_find_clauses_for_var(const sat_prob &A){

	std::vector<std::vector<int> > V(A.get_num_variables());	
	
	
		for(std::size_t j = 0;j<A.get_num_clauses();++j){
			for(std::size_t k = 0; k<A.get_clause(j).v.size();++k){
				V[abs(A.get_clause(j).v[k])-1].push_back(j);
			}
		}
	return V;
}

std::vector<int> old_asat (sat_prob &A, const unsigned int s, double p){

	mt19937::result_type seed = s;
	mt19937 g(seed);
	
	//roll initial configuration
	std::vector<int> R,U;
	std::vector<int> configuration (A.get_num_variables());
	
	std::vector<std::vector<int> > CFx = old_find_clauses_for_var(A);

	
	for(unsigned int i = 0; i<A.get_num_variables();++i)
		configuration[i]=uniform_int_distribution<>(0, 1)(g);
		
	
	find_unsatisfied_clauses(A,configuration,U);
	
	std::cout<<U.size()<<'\n';

	unsigned long long int t=0, t_max = 1000000000000LL;

	while(t<t_max){
	
		if(U.size() == 0){
			//std::cout<<"t="<<t<<'\n';
			//find_unsatisfied_clauses(A,configuration,U);
			return configuration;	
		}
						
		std::size_t w = uniform_int_distribution<>(0,U.size()-1)(g);
		std::size_t z = uniform_int_distribution<>(0,A.get_clause(w).v.size()-1)(g);
		
		unsigned int x = abs(A.get_clause(w).v[z])-1;
		
		if(configuration[x] == 0)	
			configuration[x] = 1;
		else
			configuration[x] = 0;
			
			
		find_unsatisfied_clauses(A,configuration,R,U,CFx[x]);
		if(R.size()>U.size() && uniform_real_distribution<>(0.0,1.0)(g)>p){
			if(configuration[x] == 0)	
				configuration[x] = 1;
			else
				configuration[x] = 0;
			std::cout<<"NO   "<<R.size()<<" "<<U.size()<<"  "<<t<<'\n';
		}

		else{
			
			std::cout<<"YES  "<<R.size()<<" "<<U.size()<<"  "<<t<<'\n';
			//U = R;
			std::swap(U,R);
		}
		t++;
	}
	std::cout<<"t_max reached\n";
	return configuration;
}

