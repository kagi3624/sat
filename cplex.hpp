#ifndef cplex_HPP
#define cplex_HPP


#include <ilcplex/ilocplex.h>
#include <stdlib.h>

#include "asat.hpp"

void solve_by_cplex(const sat_prob &A);

#endif
