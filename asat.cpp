#include "asat.hpp"

using namespace boost::random;



void asat::remove(size_t i){

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
}


void asat::add(size_t i){

	U.push_back(i);
	POS[i] = U.size()-1;
}


void asat::run_increasing_satisfiability(const std::vector<int> &R){

	for(size_t i = 0; i<R.size(); ++i){
		K[R[i]]++; 
		if(K[R[i]] == 1){
			E_flip--;
			remove(R[i]);
			B1.push_back(R[i]);
		}		
	}
}

void asat::run_decreasing_satisfiability(const std::vector<int> &R){

	for(size_t i = 0; i<R.size(); ++i){
		K[R[i]]--; 
		if(K[R[i]] == 0){
			E_flip++;
			add(R[i]);
			B2.push_back(R[i]);
		}		
	}
}

void asat::flip_back(){
	
	if(configuration[x] == 0){
	
		configuration[x] = 1;
	
		
		for(size_t i = 0; i<R1[x].size(); ++i)
			K[R1[x][i]]++;
		for(size_t i = 0; i<R2[x].size(); ++i)
			K[R2[x][i]]--;
	}
	else{ 
	
		configuration[x] = 0;

		
		for(size_t i = 0; i<R1[x].size(); ++i)
			K[R1[x][i]]--;
		for(size_t i = 0; i<R2[x].size(); ++i)
			K[R2[x][i]]++;
		
	}
	try{
		for(size_t i = 0; i<B2.size();++i)	
			remove(B2[i]);
		for(size_t i = 0; i<B1.size();++i)
			add(B1[i]);
		if(U.size()!=E) throw "Error at the flip back stage!";
	}
	catch(char const* s) {std::cerr<<s<<'\n';}
			B1.clear();
			B2.clear();
	
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



std::vector<int> solve_by_asat (sat_prob A, const unsigned int s, const double p, const bool b){

	unsigned long long int  max_flips = 1000000000000LL;

	mt19937::result_type seed = s;
	mt19937 g(seed);
	
	
	asat D(A,g);
	find_clauses_for_var(A,D.R1,D.R2);
	
	
	
	if(b == true){
	
		std::ofstream file;
  	file.open ("asat.dat");
		
		for(unsigned long long int i = 0; i<max_flips; ++i){
		
			if(D.E == 0){
				std::cout<<"\nconfiguration:";
				for(size_t i = 0; i<D.configuration.size();++i)
					std::cout<<D.configuration[i]<<" ";
				return D.configuration;
			}
				
			D.flip(A,g);
		
			if(D.E_flip>D.E && uniform_real_distribution<>(0.0,1.0)(g)>p)
				D.flip_back();
			else
				D.E = D.E_flip;	
				
			file<<(double)i/A.get_num_clauses()<<" "<<D.E<<'\n';
		}
	file<<'\n'<<max_flips<<"reached\n";
	file.close();
	}
	else{	
		for(unsigned long long int i = 0; i<max_flips; ++i){
		
			if(D.E == 0)
				return D.configuration;
				
			D.flip(A,g);
		
			if(D.E_flip>D.E && uniform_real_distribution<>(0.0,1.0)(g)>p)
				D.flip_back();
			else
				D.E = D.E_flip;
			std::cout<<i<<" "<<D.E<<'\n';
		}
	}	
	
	return D.configuration;
	
}
