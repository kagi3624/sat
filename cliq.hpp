#ifndef cliq_HPP
#define cliq_HPP

#include <iostream>
#include <fstream>
#include "cplex.hpp"

std::vector<std::tuple<int,int,int,int> > find_clique(const sat_prob &A);


void write(const std::vector<std::tuple<int,int,int,int> > &v);


std::vector<std::tuple<int,int,int,int> > find_clique_v2(const sat_prob &A);

#endif
