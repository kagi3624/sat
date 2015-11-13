#ifndef cliq_HPP
#define cliq_HPP


#include <tuple>
#include "cplex.hpp"

std::vector<std::tuple<int,int,int,int> > find_clique(const sat_prob &A);

void write(const std::vector<std::tuple<int,int,int,int> > &v);


#endif
