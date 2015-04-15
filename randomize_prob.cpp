#include "randomize_prob.hpp"

using namespace boost::random;

bool vec_not_stored(sat_prob &A, std::vector<int> &T){
	if(A.get_num_clauses()==0)
		return true;
	for(unsigned int i = 0; i<A.get_num_clauses();++i){
		if(A.get_clause(i).v.size() ==T.size() && A.get_clause(i).v == T)
			return false;
	}
	return true;

}

template <typename URNG>
void fill_vec(std::vector<int> &v, unsigned int N, double p, URNG& g){
	uniform_real_distribution<> dis(0, 1);
	for(unsigned int i = 0; i<N;++i){
		if(dis(g)<p)
			v.push_back(-(i+1));
		else
			v.push_back(i+1);
		}
}

template <typename RandomIt, typename URNG>
void partial_shuffle(RandomIt first, RandomIt mid, RandomIt last, URNG& g) {
  auto n = last - first;
  auto k = mid - first;
  for (decltype(n) i{}; i < k; ++i) {
    auto j = uniform_int_distribution<decltype(i)>(i, n - 1)(g);
    using std::swap;
    swap(first[i], first[j]);
  }
}

void randomize_prob(sat_prob &A, unsigned int num_var, unsigned int num_cl, unsigned int num_lit, int exact){

	try{
		if(A.get_num_variables() != 0 || A.get_num_clauses() != 0) throw "Error: SAT problem should be empty. Use the default constructor for creating one!";
		
		mt19937::result_type seed = 70;//time(0);
		mt19937 gen(seed);

		A.set_num_variables(num_var);
		if(num_lit==0){
			uniform_int_distribution<int> random_num_literals(1,num_var);
			for(unsigned int n = 0; n<num_cl;++n){
				unsigned int k = random_num_literals(gen);
				clause a(k);
				std::vector<int> T;
				fill_vec(T, num_var, A.get_probability(), gen);
				partial_shuffle(T.begin(), T.begin()+k, T.end(), gen);
				a.v.assign(T.begin(),T.begin()+k); 
				A.add_clause(a);
			}
		}

		else if(num_lit==num_var){
			if(num_cl>pow(2,num_var)) throw "Error: There are less possible clauses than the number choosen clauses!";
			for(unsigned int n = 0; n<num_cl;++n){
				bool store = false;
				while(store == false){
					clause a(num_lit);
					fill_vec(a.v, num_var, A.get_probability(), gen);
					if(vec_not_stored(A,a.v)){
						A.add_clause(a);
						store = true;
					}
				}
			}
		}
		else if(exact == 0){
			uniform_int_distribution<int> random_num_literals(1,num_lit);
			for(unsigned int n = 0; n<num_cl;++n){
				unsigned int k = random_num_literals(gen);
				clause a(k);
				std::vector<int> T;
				fill_vec(T, num_var, A.get_probability(), gen);
				partial_shuffle(T.begin(), T.begin()+k, T.end(), gen);
				a.v.assign(T.begin(),T.begin()+k); 
				A.add_clause(a);
			}
		}
		else{
			for(unsigned int n = 0; n<num_cl;++n){
				clause a(num_lit);
				std::vector<int> T;
				fill_vec(T, num_var, A.get_probability(), gen);
				partial_shuffle(T.begin(), T.begin()+num_lit, T.end(), gen);
				a.v.assign(T.begin(),T.begin()+num_lit); 
				A.add_clause(a);
			}
		}
	}
	catch(char const* s){
		std::cerr<<s<<'\n';
	}
}
