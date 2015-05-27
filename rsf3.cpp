#include "rsf3.hpp"



void set_clause_values(clause &a, int num_values, int num_literals, const std::vector<int> &configuration, const std::vector<int> &set_indices){

	for(auto i = 0; i<num_values;++i){
		if(configuration[set_indices[i]]==0)
			a.v.push_back(-(set_indices[i]+1));
		else
			a.v.push_back(set_indices[i]+1);
		}
	for(auto i = num_values; i<num_literals;++i){
		if(configuration[set_indices[i]]==0)
			a.v.push_back(set_indices[i]+1);
		else
			a.v.push_back(-(set_indices[i]+1));
	}
}







void randomize_k3(sat_prob &A, int num_literals, int s){

			/*for(size_t i = 0; i<set_indices.size(); ++i){
			std::cout<<set_indices[i]<<" ";
			}
			std::cout<<'\n'; */


	try{
	
		using namespace boost::random;
	
		if(num_literals !=3) throw "this function is for k = 3 only!";
		
		mt19937::result_type seed = s;
		mt19937 gen(seed);
		uniform_real_distribution<> rand_p(0,1);
		
		std::vector<int> configuration;
		configuration.resize(A.get_num_variables());
		
		for(size_t i = 0; i<configuration.size(); ++i){
			if(rand_p(gen)<0.5) configuration[i]=1;
		}
		
		std::vector<int> set_indices(A.get_num_variables());
		
		for(size_t i = 0; i<set_indices.size();++i)
			set_indices[i]=i;
			
		clause a(num_literals);
		a.v.reserve(num_literals);
		
		for(size_t m = 0; m<A.get_num_clauses();++m){
			
			partial_shuffle(set_indices.begin(),set_indices.begin()+num_literals, set_indices.end(), gen);
			
			double p = rand_p(gen);
			
			if(p<=0.073){
				set_clause_values(a, 3, 3, configuration, set_indices);
				A.add_clause(a);
				a.v.clear();		
			}
			else if(p<=0.345 && p>0.073){
				set_clause_values(a, 2, 3, configuration, set_indices);
				A.add_clause(a);
				a.v.clear();
			}
			else{
				set_clause_values(a, 1, 3, configuration, set_indices);
				A.add_clause(a);
				a.v.clear();
			}
		}
		
		
		
	}
		
	
	catch(char const* s){std::cerr<<s<<'\n';}
}

