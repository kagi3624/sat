#ifndef sat_prob_HPP
#define sat_prob_HPP

#include <vector>
#include <iostream>

enum compare {eq, greater, lesser};


struct clause{
	std::vector <int> v;
	
	void add_literal (int const &x);
	int get_literal (std::size_t i) const;

	void print_clause();
	void set_num_literals(unsigned int const &x);
	
	unsigned int get_num_literals();
	
	clause(unsigned int x = 0);
	
	
	private:
	unsigned int num_literals;
};

struct cut{
	
	std::vector <int> left;
	compare relation;
	int right;
	
	void print_cut();
};

class sat_prob{
  std::vector<clause> problem;
  std::vector<cut> cut_pool;
	unsigned int num_variables, num_clauses, num_cuts = 0;
	double probability; 
   
public:
 	
	void add_clause(clause const& f);
	void add_cut(cut const& c);
	clause get_clause(size_t i) const;
	cut get_cut(size_t i) const;
	unsigned int problem_size();
	void print_problem();
	
	void set_num_variables (unsigned int x);
	unsigned int get_num_clauses() const;
	unsigned int get_num_variables() const;
	unsigned int get_num_cuts() const;
	double get_probability() const;
	
	int range_value(std::size_t i);
	
	sat_prob(unsigned int x = 0, unsigned int y = 0, double p = 0.5);
	
};
#endif
