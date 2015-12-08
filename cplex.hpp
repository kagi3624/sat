#ifndef cplex_HPP
#define cplex_HPP


#include <ilcplex/ilocplex.h>
#include <stdlib.h>
#include <tuple>

#include "asat.hpp"

void solve_by_cplex(const sat_prob &A);
void solve_w_cuts(const sat_prob &A, const std::vector<std::tuple<int,int,int,int> > &v);

#endif
