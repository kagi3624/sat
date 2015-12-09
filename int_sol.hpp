///////////////////////////////////////////////////////////////////////////////////
// INT_SOL: For given number of variables, clauses and literals the SAT problem //
//          is tested via linear programming for a possible integer solution   //
////////////////////////////////////////////////////////////////////////////////


#ifndef int_sol_HPP
#define int_sol_HPP

#include <math.h> 
#include <ilcplex/ilocplex.h>
#include <iostream>
#include <boost/random/mersenne_twister.hpp>
#include "cliq.hpp"
#include "randomize_prob.hpp"
#include <chrono>
#include <ctime>



void test_for_int(const int v,const int c,const int k,const int r,const int s, const bool e);
void test_w_cuts(const int v,const int c,const int k,const int r,const int s, const bool e);

#endif 
