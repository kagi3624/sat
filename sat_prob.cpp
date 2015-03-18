#include "sat_prob.hpp"

void clause::print_clause(){
	
	for(std::size_t i=1; i<=v.size();++i){
			if(get_literal(i)<0 && i == v.size())
				std::cout<<"\u0304x"<<-get_literal(i)<<'\n';
			else if (get_literal(i)>0 && i == v.size())
				std::cout<<'x'<<get_literal(i)<<'\n';
			else if (get_literal(i)<0)
				std::cout<<"\u0304x"<<-get_literal(i)<<" \u2228 ";
			else 
				std::cout<<'x'<<get_literal(i)<<" \u2228 ";
	}
};


void clause::add_literal (int const &x){
	if(x==0) std::cerr<<"The indexing of literals beginns at 1. Don't try to add zeros to the clause!\n";
	else
		v.push_back(x);
}
int clause::get_literal (std::size_t i)const{return v[i-1];}
void clause::set_num_literals(unsigned int const &x){num_literals=x;};



void sat_prob::add_clause(clause const& f){
	num_clauses += 1;
	problem.push_back(f);
	if(num_clauses!=problem.size())
		std::cerr<<"Number of stored clauses doesn't match the given number of clauses!\n";
}

void sat_prob::print_problem(){
	for (unsigned int i = 0; i<problem.size();++i)
		problem[i].print_clause();
};


clause sat_prob::get_clause(std::size_t i) const {return problem[i-1];}

unsigned int sat_prob::get_num_clauses() const {return num_clauses;}
unsigned int sat_prob::get_num_variables() const {return num_variables;}

sat_prob::sat_prob(unsigned int x, unsigned int y, double p): num_variables(x), num_clauses(y), probability(p){}
