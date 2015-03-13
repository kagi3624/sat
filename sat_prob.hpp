#ifndef sat_prob_HPP
#define sat_prob_HPP

#include <vector>
#include <iostream>

//literals and clauses numbering starts at 1!

struct clause{

	
	void add_literal (int const &x);
	int get_literal (std::size_t i) const;
	unsigned int length() const;
	void print_clause();
	
	private:
	std::vector <int> v;
	unsigned int num_literals;
};

class sat_prob{
  std::vector<clause> problem;

   
public:
	unsigned int num_clauses, num_variables; 
 	double p;
 	
	unsigned int length() const;
	void add_clause(clause const& f);
	void remove_clause (std::size_t i);
	clause get_clause(std::size_t i) const;
	void print_problem();
	
};


	
#endif
