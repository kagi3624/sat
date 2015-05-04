#include "asat.hpp"

using namespace boost::random;

F::F(sat_prob &A, const std::vector<int> &configuration){

	N = 0;
	vec.resize(A.get_num_clauses());
	for(size_t i = 0; i<A.get_num_clauses();++i){
		size_t sum = 0;
		for(size_t j = 0; j<A.get_clause(i).v.size();++j){		
			if((A.get_clause(i).v[j] < 0 && configuration[-A.get_clause(i).v[j]-1]) || (A.get_clause(i).v[j] > 0 && configuration[A.get_clause(i).v[j]-1]))
				sum++;
		}
		vec[i] = sum;
		if(sum == 0){
			N++;
			U.push_back(i);
		}	
	}
}

//fills respectivly the vectors with indices of clauses where x_i or its conjugate is present
void find_clauses_for_var(const sat_prob &A, std::vector<std::vector<int> > &R1, std::vector<std::vector<int> > &R2){

	for(std::size_t j = 0;j<A.get_num_clauses();++j){
		for(std::size_t k = 0; k<A.get_clause(j).v.size();++k){
			if(A.get_clause(j).v[k]<0)
				R2[-A.get_clause(j).v[k]-1].push_back(j);
			else
				R2[A.get_clause(j).v[k]-1].push_back(j);
		}
	}
}




std::vector<int> solve_by_asat (sat_prob &A, unsigned int s, double p){

	mt19937::result_type seed = s;
	mt19937 g(seed);
	
	std::vector<int> configuration (A.get_num_variables());
	std::vector<std::vector<int> > R1, R2;
	
	for(size_t i = 0; i<A.get_num_variables();++i)
		configuration[i] = uniform_int_distribution<>(0, 1)(g);

	F t(A,configuration);
	find_clauses_for_var(A,R1,R2);
	std::cout<<t.N<<'\n';
	
	return t.U;
	//return configuration;
}






/*void find_unsatisfied_clauses(const sat_prob &A, const std::vector<int> &conf, std::vector<int> &R){


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

void find_unsatisfied_clauses(const sat_prob &A, const std::vector<int> &conf, std::vector<int> &R, const std::vector<int> &U, const std::vector<int> &W){

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
	std::vector<int> configuration (A.get_num_variables());
	
	std::vector<std::vector<int> > CFx = find_clauses_for_var(A);

	for(unsigned int i = 0; i<A.get_num_variables();++i)
		configuration.push_back(uniform_int_distribution<>(0, 1)(g));
	
		
	find_unsatisfied_clauses(A,configuration,U);

	unsigned int t=0, t_max = 1000000000;

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
			std::cout<<"a  "<<R.size()<<" "<<U.size()<<"  "<<t<<'\n';
		}

		else{
			
			std::cout<<"b  "<<R.size()<<" "<<U.size()<<"  "<<t<<'\n';
			//U = R;
			std::swap(U,R);
		}
		t++;
	}
	std::cout<<"t_max reached\n";
	return configuration;
}*/

