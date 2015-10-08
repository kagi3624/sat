//////////////////////////////////////////////////////////////////////////////////////////////////
// GCON: Transforms a SAT Problem into a factor graph and calculates the gratest connectivity  //
////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef gcon_HPP
#define gcon_HPP

#include "sat_prob.hpp"
#include <cmath>
#include <lemon/connectivity.h>
#include <lemon/concepts/graph_components.h>
#include <lemon/list_graph.h>


//where x_i and ̄x_i are same node
int gcon1(const sat_prob &A);

//where x_i and ̄x_i are distinct node
int gcon2(const sat_prob &A);

void gcon_test(const sat_prob &A);

#endif
