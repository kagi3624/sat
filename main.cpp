#include "int_sol.hpp"
#include "cplex.hpp"
#include "cliq.hpp"
#include "old_asat.hpp"
#include "rsf3.hpp"

#include <boost/program_options.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <ctime>
#include <unistd.h>


int main (int argc, char **argv){

	try{
	
		namespace po = boost::program_options;
	
		int num_variables, num_literals, num_clauses,  seed, runs;
		
		po::options_description desc("Allowed options");
		desc.add_options()
    ("help", "produce help message")
    ("variables,v", po::value<int>(&num_variables), "set number of variables")
    ("literals,k", po::value<int>(&num_literals)->default_value(3), "set number of maximum literals")
    ("clauses,c", po::value<int>(&num_clauses), "set number of clauses")
    ("seed,s", po::value<int>(&seed)->default_value(0), "set number of the seed (default is random)")
    ("exact,e", "set every clause to exact length of given literals")
    ("grsf3,g", "generate a guaranteed satisfiable 3SAT problem")
    ("cplex,x", "solve by cplex")
    ("clique,q", "find clique cuts")
    ("asat,a", "solve by asat")
    ("oldasat,o", "solve by old asat")
    ("print,p", "print problen")
    ("runs,r", po::value<int>(&runs), "number of runs for simulation");


		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm); 
		
		
		if (vm.count("help") || argc == 1){
		  std::cout << desc <<'\n';
		  return 1;
		}
		
		if (!vm.count("clauses") || !vm.count("variables")) throw "Error: Give the number of variables and clauses!";
		
		if(num_literals>num_variables) throw "Error: Number of variables can't be lower than the number of literals!";
		
		if(num_literals !=3 && vm.count("grsf3")) throw "grsf3 is for 3SAT only!";
		
		if(!vm.count("cplex")&&!vm.count("asat")&&!vm.count("oldasat")&&!vm.count("clique")) throw "select at least one solver!";

		if(vm.count("cplex")&& vm.count("runs"))
			test_for_int(num_variables, num_clauses, num_literals, runs, seed, !vm.count("exact"));
		else if(vm.count("clique")&& vm.count("runs"))
			test_w_cuts(num_variables, num_clauses, num_literals, runs, seed, !vm.count("exact"));
		else if(vm.count("clique")||vm.count("cplex")||vm.count("asat")||vm.count("oldasat")){
			sat_prob A(num_variables,num_clauses);
			
			if(vm.count("grsf3"))
				randomize_k3(A,num_literals,seed);
		 	else{
		 		boost::random::mt19937 gen(seed);
		 		randomize_prob(A,gen,num_literals,!vm.count("exact"));
			}
			if(vm.count("cplex")) solve_by_cplex(A);
			if(vm.count("clique")){
			
				auto v = find_clique(A);
				solve_w_cuts(A,v);
				write(v);

			}
			if(vm.count("print")) A.print_problem();
			if(vm.count("oldasat")) std::vector<int> t = old_asat(A,seed,0.21);
			if(vm.count("asat")) std::vector<int> t = solve_by_asat(A,seed,0.21);

		}
	}
	
	catch (std::exception &e){std::cerr <<e.what()<<'\n';}	
	catch(char const* s){std::cerr<<s<<'\n';}
	catch (...) {std::cerr << "Unknown exception caught" <<'\n';}
}
