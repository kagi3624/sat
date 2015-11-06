#ifndef cliq_HPP
#define cliq_HPP



#include "cplex.hpp"

void write(const std::vector<std::vector<int> > &v);
std::vector<std::vector<int> > find_clique(const sat_prob &A, int f, int g);


#endif
