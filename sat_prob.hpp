#ifndef sat_prob_HPP
#define sat_prob_HPP

#include <vector>
#include <iostream>

//literals and clauses indexing starts at 1!

struct clause{

	
	void add_literal (int const &x);
	int get_literal (std::size_t i) const;

	void print_clause();
	void set_num_literals(unsigned int const &x);
	
	private:
	std::vector <int> v;
	unsigned int num_literals;
};

class sat_prob{
  std::vector<clause> problem;
	unsigned int num_variables, num_clauses;
	double probability; 
   
public:

 	
	void add_clause(clause const& f);
	void remove_clause (std::size_t i);
	clause get_clause(std::size_t i) const;
	void print_problem();
	unsigned int get_num_clauses() const;
	unsigned int get_num_variables() const;
	
	sat_prob(unsigned int x = 0, unsigned int y = 0, double p = 0.5);
	
};
#endif
