#include "asat.hpp"

using namespace boost::random;


void asat::remove(size_t i){
	try{
		if((unsigned int)POS[i] == U.size()-1){
			POS[i] = -1;
			U.pop_back();
		}
 
		else{
			POS[U[U.size()-1]] = POS[i]; 
			U[POS[i]] = U[U.size()-1];
			POS[i] = -1;
			U.pop_back();
		}
		if(POS[i] != - 1) throw "something went wrong, clause not removed!";
	}
	catch(char const* s){
		std::cerr<<s<<'\n';
	}
	catch (...) {
		std::cerr << "Unknown exception caught" <<'\n';
	}
}


void asat::add(size_t i){

	U.push_back(i);
	POS[i] = U[U.size()-1];

}


void asat::run_through_increasing(const std::vector<int> &R){

	for(size_t i = 0; i<R.size(); ++i){
		K[R[i]]++; 
		if(K[R[i]] == 1){
			E_flip--;
			remove(R[i]);
			B2.push_back(R[i]);
		}		
	}
}

void asat::run_through_decreasing(const std::vector<int> &R){

	for(size_t i = 0; i<R.size(); ++i){
		K[R[i]]--; 
		if(K[R[i]] == 0){
			E_flip++;
			add(R[i]);
			B1.push_back(R[i]);
		}		
	}
}




// R1 stores indices of clauses with x, R2 stores ̄x respectively
void find_clauses_for_var(const sat_prob &A, std::vector<std::vector<int> > &R1, std::vector<std::vector<int> > &R2){

	for(std::size_t j = 0;j<A.get_num_clauses();++j){
		for(std::size_t k = 0; k<A.get_clause(j).v.size();++k){
			if(A.get_clause(j).v[k]<0)
				R2[-A.get_clause(j).v[k]-1].push_back(j);
			else
				R1[A.get_clause(j).v[k]-1].push_back(j);
		}
	}
}



std::vector<int> solve_by_asat (sat_prob &A, unsigned int s, double p){

	size_t max_tries = 100000;

	mt19937::result_type seed = s;
	mt19937 g(seed);
	
	
	asat D(A,g);
	find_clauses_for_var(A,D.R1,D.R2);
	
	for(size_t i = 0; i<max_tries; ++i{
		
		if(D.E == 0)
			return D.configuration;
		
		D.flip(A,g);
	
	
	}
	
	
	return D.configuration;
	
}
