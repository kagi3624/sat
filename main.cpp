#include "int_sol.hpp"
#include "cplex.hpp"
#include "asat.hpp"
#include "old_asat.hpp"
#include "rsf3.hpp"

#include<boost/program_options.hpp>
#include <ctime>
#include <unistd.h>


int main (int argc, char **argv){

	try{
	
		namespace po = boost::program_options;
	
		int num_variables, num_literals, num_clauses,  seed, runs;
		bool cpl, exact, asat, oldasat, grsf3;
		
		po::options_description desc("Allowed options");
		desc.add_options()
    ("help", "produce help message")
    ("variables,v", po::value<int>(&num_variables), "set number of variables")
    ("literals,k", po::value<int>(&num_literals)->default_value(3), "set number of maximum literals")
    ("clauses,c", po::value<int>(&num_clauses), "set number of clauses")
    ("seed,s", po::value<int>(&seed)->default_value(time(NULL)+getpid()), "set number of the seed (default is random)")
    ("exact,e", po::value<bool>(&exact)->default_value(true), "set every clause to exact length of given literals")
    ("grsf3,g", po::value<bool>(&grsf3)->default_value(false), "generate a guaranteed satisfiable 3SAT problem")
    ("cplex,x", po::value<bool>(&cpl)->default_value(false), "solve by cplex")
    ("asat,a", po::value<bool>(&asat)->default_value(false), "solve by asat")
    ("oldasat,o", po::value<bool>(&oldasat)->default_value(false), "solve by old asat")
    ("intsol,i", po::value<int>(&runs), "test for an integer solution within the given number of runs");
		
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm); 
		
		
		if (vm.count("help") || argc == 1){
		  std::cout << desc <<'\n';
		  return 1;
		}
		
		if (!vm.count("clauses") || !vm.count("variables")) throw "Error: Give the number of variables and clauses!";
		
		if(num_literals>num_variables) throw "Error: Number of variables can't be lower than the number of literals!";
		
		if(num_literals !=3 && grsf3==true) throw "grsf3 is for 3SAT only!";
		
		if(!vm.count("cplex")&&!vm.count("asat")&&!vm.count("oldsat")&&!vm.count("intsol")) throw "select at least one solver!";
		
		if(vm.count("intsol")){
			if(grsf3) throw "grsf3 shouldn't be used with intsol!";
			test_for_int(num_variables, num_clauses, num_literals, runs, seed, exact);
		}
		if(vm.count("cplix")||vm.count("asat")||vm.count("oldasat")){
			sat_prob A(num_variables,num_clauses);
			grsf3 ? randomize_k3(A,num_literals,seed) : randomize_prob(A,seed,num_literals,exact);
	
			if(cpl == true) solve_by_cplex(A);
			if(oldasat == true) std::vector<int> t = old_asat(A,seed,0.21);
			if(asat == true) std::vector<int> t = solve_by_asat(A,seed,0.21);
			
			//A.print_problem();
		}
		
		

	}
	
	catch (std::exception &e){std::cerr <<e.what()<<'\n';}	
	catch(char const* s){std::cerr<<s<<'\n';}
	catch (...) {std::cerr << "Unknown exception caught" <<'\n';}

}
