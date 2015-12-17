#ifndef sat_prob_HPP
#define sat_prob_HPP

#include <vector>
#include <set>
#include <iostream>
#include <boost/iterator/counting_iterator.hpp>

struct clause{
	std::vector <int> v;
	
	void add_literal (int const &x);
	int get_literal (std::size_t i) const;

	void print_clause();
	void set_num_literals(unsigned int const &x);
	
	unsigned int get_num_literals() const;
	
	clause(unsigned int x = 0);
	
	
	private:
	unsigned int num_literals;
};

class sat_prob{
  std::vector<clause> problem;
  std::set<unsigned int> unbound_vars;
	unsigned int num_variables, num_clauses;
	double probability; 
   
public:
 	
	void add_clause(clause const& f);
	clause get_clause(size_t i) const;
	unsigned int problem_size();
	void print_problem();
	
	void find_unbound_vars();
	std::set<unsigned int> get_unbound_vars() const;
	
	void set_num_variables (unsigned int x);
	unsigned int get_num_clauses() const;
	unsigned int get_num_variables() const;
	double get_probability() const;
	
	int range_value(std::size_t i) const;
	
	void clear();
	
	sat_prob(unsigned int x = 0, unsigned int y = 0, double p = 0.5);
	
};
#endif
