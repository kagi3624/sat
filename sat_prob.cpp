#include "sat_prob.hpp"

clause::clause(unsigned int x):num_literals(x){}

void clause::print_clause(){
	
	for(size_t i=0; i<v.size();++i){
			if(v[i]<0 && i == v.size()-1)
				std::cout<<"\u0304x"<<-v[i]-1<<'\n';
			else if (v[i]>0 && i == v.size()-1)
				std::cout<<'x'<<v[i]-1<<'\n';
			else if (v[i]<0)
				std::cout<<"\u0304x"<<-v[i]-1<<" \u2228 ";
			else 
				std::cout<<'x'<<v[i]-1<<" \u2228 ";
	}
};

void cut::print_cut(){

	try{
		if(left.size()==0) throw "Error: left-hand side empty!";

		
		for(size_t i = 0; i<left.size();++i){
			if(i == 0 && left[0]>0){
				std::cout<<'x'<<left[0]-1;
				continue;
			}
			if(left[i]<0)
				std::cout<<"-x"<<-left[i]-1;
			else
				std::cout<<"+x"<<left[i]-1;
		}
		if(relation==eq) 
			std::cout<<"=";
		else if(relation==lesser)
			std::cout<<"<";
		else if(relation==greater)
			std::cout<<">";
		else throw "Error: relation not set!";
		
		std::cout<<right<<'\n';
	}
	catch(char const* s){
		std::cerr<<s<<'\n';
	}
}


void clause::add_literal (int const &x){
	if(x==0) std::cerr<<"literal indexing beginns with 1!\n";
	else{
		v.push_back(x);
		num_literals+=1;
	}
}


int clause::get_literal (std::size_t i)const{
	return v[i];
}
void clause::set_num_literals(unsigned int const &x){num_literals=x;};
unsigned int clause::get_num_literals(){
	
	if(num_literals!=v.size()){
		std::cerr<<"Number of stored literals doesn't match the given number of literals!\n";
		return 0;
	}
	return num_literals;

}



void sat_prob::add_clause(clause const& f){
	problem.push_back(f);
}

void sat_prob::add_cut(cut const& c){
	cut_pool.push_back(c);
	num_cuts = cut_pool.size();
}

void sat_prob::print_problem(){
	for (size_t i = 0; i<problem.size();++i){
		std::cout<<"c"<<i<<": ";
		problem[i].print_clause();		
	}
	for(size_t i = 0; i<cut_pool.size();++i){
		std::cout<<"cut"<<i<<": ";
		cut_pool[i].print_cut();
	}
};


clause sat_prob::get_clause(size_t i) const {return problem[i];}
cut sat_prob::get_cut(size_t i) const {return cut_pool[i];}
unsigned int sat_prob::problem_size(){return problem.size();}

void sat_prob::set_num_variables(unsigned int x){num_variables=x;}
unsigned int sat_prob::get_num_clauses() const {return num_clauses;}
unsigned int sat_prob::get_num_cuts() const {return cut_pool.size();}
unsigned int sat_prob::get_num_variables() const {return num_variables;}
double sat_prob::get_probability() const {return probability;}

int sat_prob::range_value(std::size_t i){
	int z = 0;
	for(unsigned int t = 0; t<problem[i].get_num_literals();++t)
		if(problem[i].get_literal(t)<0)
			z++;
	return(1-z);
}

sat_prob::sat_prob(unsigned int x, unsigned int y, double p): num_variables(x), num_clauses(y), probability(p){

	problem.reserve(num_clauses);

}
