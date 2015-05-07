#include "asat.hpp"

using namespace boost::random;



void asat::remove(size_t i){
	try{
		std::cout<<"removed: "<<i<<"--->"<<POS[i]<<'\n';
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
	catch(char const* s) {std::cerr<<s<<'\n';}
	catch (...) {std::cerr << "Unknown exception caught" <<'\n';}
}


void asat::add(size_t i){

	U.push_back(i);
	POS[i] = U.size()-1;
	std::cout<<"added: "<<i<<"--->"<<U.size()-1<<'\n';
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
	
		std::cout<<"x flip back to "<<x<<'\n';
		
		for(size_t i = 0; i<R1[x].size(); ++i)
			K[R1[x][i]]++;
		for(size_t i = 0; i<R2[x].size(); ++i)
			K[R1[x][i]]--;
	}
	else{ 
	
		configuration[x] = 0;
		
		for(size_t i = 0; i<R1[x].size(); ++i)
			K[R1[x][i]]--;
		for(size_t i = 0; i<R2[x].size(); ++i)
			K[R1[x][i]]++;
		
	}
	
	for(size_t i = 0; i<B2.size();++i)	
		remove(B2[i]);
	for(size_t i = 0; i<B1.size();++i)
		add(B1[i]);
		
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



std::vector<int> solve_by_asat (sat_prob &A, unsigned int s, double p){

	size_t max_tries = 1;

	mt19937::result_type seed = s;
	mt19937 g(seed);
	
	
	asat D(A,g);
	find_clauses_for_var(A,D.R1,D.R2);
	
	
	for(size_t i = 0; i<max_tries; ++i){
		
		if(D.E == 0)
			return D.configuration;
			

		
		std::cout<<"U before: ";
		for(size_t i = 0; i<D.U.size();++i)
			std::cout<<D.U[i]<<" ";
		std::cout<<'\n';

		std::cout<<"POS before: ";
		for(size_t i = 0; i<D.POS.size();++i)
			std::cout<<D.POS[i]<<" ";
		std::cout<<'\n';
		
		std::cout<<"satisfiability before: ";
		for(size_t i = 0; i<D.K.size();++i)
			std::cout<<D.K[i]<<" ";
		std::cout<<'\n';
		
		D.flip(A,g);
		
		std::cout<<"satisfiability after: ";
		for(size_t i = 0; i<D.K.size();++i)
			std::cout<<D.K[i]<<" ";
		std::cout<<'\n';
		
		std::cout<<"B1: ";
		for(size_t i = 0; i<D.B1.size();++i)
			std::cout<<D.B1[i]<<" ";
		std::cout<<'\n';
		
		std::cout<<"B2: ";
		for(size_t i = 0; i<D.B2.size();++i)
			std::cout<<D.B2[i]<<" ";	
		std::cout<<"\n----------------------------------------------------------------------\n";
		
		//if(D.E_flip>D.E && uniform_real_distribution<>(0.0,1.0)(g)>p){
			
		std::cout<<"**************************************************************************\n";
		std::cout<<"not accepted\n";
		D.flip_back();
		
		std::cout<<"satisfiability after flipback: ";
		for(size_t i = 0; i<D.K.size();++i)
			std::cout<<D.K[i]<<" ";
		std::cout<<'\n';
			
		std::cout<<"U after flip_back: ";
		for(size_t i = 0; i<D.U.size();++i)
			std::cout<<D.U[i]<<" ";
		std::cout<<'\n';

		std::cout<<"POS U after flip_back: ";
		for(size_t i = 0; i<D.POS.size();++i)
			std::cout<<D.POS[i]<<" ";
		std::cout<<'\n';
		
		std::cout<<"B1 after flip_back: ";
		for(size_t i = 0; i<D.B1.size();++i)
			std::cout<<D.B1[i]<<" ";
		std::cout<<'\n';
		
		std::cout<<"B2 after flip_back: ";
		for(size_t i = 0; i<D.B2.size();++i)
			std::cout<<D.B2[i]<<" ";	
		std::cout<<"\n**************************************************************************\n";
		//}
	
		//else
			D.E = D.E_flip;
	}
	
	return D.configuration;
	
}
