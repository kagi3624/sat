#include "a_test.hpp"

void tt(const int v,const int c,const int k,const int r,const int s, const bool e){

	boost::random::mt19937 gen;

	for(auto i = 0; i<r;++i){
	
		auto start = std::chrono::high_resolution_clock::now();
		gen.seed(s+i);
		sat_prob A(v,c);
		randomize_prob(A, gen, k);

		auto N = find_clique(A);
		auto end = std::chrono::high_resolution_clock::now();
		
		std::chrono::duration<double> diff = end-start;
		
		std::cout<<s+i<<" "<<N.size()<<" "<<diff.count()<<" "<<'\n';
	}
}
