#include "randomize_prob.hpp"

using namespace boost::random;

bool is_valid(sat_prob &A);

bool vec_not_stored(sat_prob &A, std::vector<int> &T){
	if(A.problem_size()==0)
		return true;
	for(unsigned int i = 0; i<A.get_num_clauses();++i){
		if(A.get_clause(i).v.size() ==T.size() && A.get_clause(i).v == T)
			return false;
	}
	return true;

}


void fill_vec(std::vector<int> &v, unsigned int N){

	v.resize(N);
	
	for(unsigned int i = 0; i<N;++i)
		v[i]=i+1;
		
}


template <typename URNG>
void random_flip(std::vector<int> &v, const double p, URNG &g){
	uniform_real_distribution<> rand_p(0,1);
	for(std::size_t i = 0; i<v.size();++i)
		if(rand_p(g)>p)
			v[i]=-v[i];
}


void randomize_prob(sat_prob &A, boost::random::mt19937 &gen, unsigned int num_lit, bool exact){

	try{
		if(A.get_num_variables() == 0 || A.get_num_clauses() == 0) throw "By definition an empty SAT problem (no clauses) is satisfied!";
		
		unsigned int num_var = A.get_num_variables();
		unsigned int num_cl  = A.get_num_clauses();
		
		std::vector<int> T;
		fill_vec(T, num_var);



		if(num_lit==0){
			uniform_int_distribution<int> random_num_literals(1,num_var);
			for(unsigned int n = 0; n<num_cl;++n){
				unsigned int k = random_num_literals(gen);
				clause a(k);
				partial_shuffle(T.begin(), T.begin()+k, T.end(), gen);
				a.v.assign(T.begin(),T.begin()+k); 
				A.add_clause(a);
			}
		}

		else if(num_lit==num_var){
			if(num_cl>pow(2,num_var)) throw "Error: There are less possible clauses than the number of choosen clauses!";
			clause a(num_lit);
			for(unsigned int n = 0; n<num_cl;++n){
				bool store = false;
				while(store == false){
					random_flip(T,A.get_probability(),gen);	
					if(vec_not_stored(A,T)){
						a.v = T;
						A.add_clause(a);
						T.clear();
						fill_vec(T,num_var);
						store = true;
					}
				}
			}
		}
		else if(exact == false){
			uniform_int_distribution<int> random_num_literals(1,num_lit);
			for(unsigned int n = 0; n<num_cl;++n){
				unsigned int k = random_num_literals(gen);
				clause a(k);				
				partial_shuffle(T.begin(), T.begin()+k, T.end(), gen);
				a.v.assign(T.begin(),T.begin()+k); 
				random_flip(a.v,A.get_probability(),gen);				
				A.add_clause(a);
			}
		}
		else{
			int count = 0;
		//	do{
				//A.clear();
				for(unsigned int n = 0; n<num_cl;++n){
					clause a(num_lit);		
					partial_shuffle(T.begin(), T.begin()+num_lit, T.end(), gen);
					a.v.assign(T.begin(),T.begin()+num_lit); 
					random_flip(a.v,A.get_probability(),gen);	
					A.add_clause(a);
				//	++count;
					//std::cout<<count<<'\n';
				}
		//	} while (!is_valid(A));	
		}
	}
	catch(char const* s){
		std::cerr<<s<<'\n';
	}
}

bool is_valid(sat_prob &A){
	
	std::map <int,int> M;
	
	for(size_t i = 0; i<A.get_num_clauses(); ++i)
	for(size_t j = 0; j<A.get_clause(i).get_num_literals();++j){
		
		M[abs(A.get_clause(i).get_literal(j))]++;
	}
	if(M.size() == A.get_num_variables())
		return true;
	return false;
}
