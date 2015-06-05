#pragma once

#include <ilcplex/ilocplex.h>
#include <stdlib.h>
#include "sat_prob.hpp"

void solve_wcuts(const sat_prob &A);
