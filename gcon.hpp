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


int gcon(const sat_prob &A);

#endif
