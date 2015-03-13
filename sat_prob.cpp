#include "sat_prob.hpp"

void clause::print_clause(){
	
	for(std::size_t i=0; i<length();++i){
			if(get_literal(i)<0 && i == length()-1)
				std::cout<<"\u0304x"<<-get_literal(i)<<'\n';
			else if (get_literal(i)>0 && i == length()-1)
				std::cout<<'x'<<get_literal(i)<<'\n';
			else if (get_literal(i)<0)
				std::cout<<"\u0304x"<<-get_literal(i)<<" \u2228 ";
			else 
				std::cout<<'x'<<get_literal(i)<<" \u2228 ";
	}
};

unsigned int clause::length() const {return v.size();}
void clause::add_literal (int const &x){v.push_back(x);}
int clause::get_literal (std::size_t i)const{return v[i-1];}

unsigned int sat_prob::length() const {return problem.size();}


void sat_prob::add_clause(clause const& f){
	num_clauses += 1;
	problem.push_back(f);
	if(num_clauses!=length())
		std::cerr<<"Number of stored clauses doesn't match the given number of clauses!\n";
}



	
clause sat_prob::get_clause(std::size_t i) const {return problem[i-1];}
