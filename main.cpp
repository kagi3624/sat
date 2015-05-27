#include "randomize_prob.hpp"
#include "cplex.hpp"
#include "asat.hpp"
#include "old_asat.hpp"
#include "rsf3.hpp"

#include<boost/program_options.hpp>


int main (int argc, char **argv){

	try{
	
		namespace po = boost::program_options;
	
		int num_variables, num_literals, num_clauses,  seed;
		bool cpl, exact, asat, oldasat, rsf3;
		
		po::options_description desc("Allowed options");
		desc.add_options()
    ("help", "produce help message")
    ("variables,v", po::value<int>(&num_variables), "set number of variables")
    ("literals,k", po::value<int>(&num_literals)->default_value(3), "set number of maximum literals")
    ("clauses,c", po::value<int>(&num_clauses), "set number of clauses")
    ("seed,s", po::value<int>(&seed)->default_value(1000), "set number of the seed")
    ("exact,e", po::value<bool>(&exact)->default_value(true), "set every clause to exact length of given literals")
    ("cplex,x", po::value<bool>(&cpl), "solve by cplex")
    ("asat,a", po::value<bool>(&asat), "solve by asat")
    ("oldasat,o", po::value<bool>(&oldasat), "solve by old asat")
    ("rsf3,r", po::value<bool>(&rsf3)->default_value(false), "generate a guaranteed satisfiable 3SAT problem");
		
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm); 
		
		
		if (vm.count("help") || argc == 1) {
		  std::cout << desc << "\n";
		  return 1;
		}
		
		if (!vm.count("clauses") || !vm.count("variables")) throw "Error: Give the number of variables and clauses!";
		
		if(num_literals>num_variables) throw "Error: Number of variables can't be lower than the number of literals!";
		
		if(vm.count("rsf3") && num_literals !=3) throw "rsf3 is for 3SAT only!";
		
		if(!vm.count("cplex")&&!vm.count("asat")&&!vm.count("oldsat")) throw "select at least one solver!";
		
		sat_prob A(num_variables,num_clauses);
	
		if(rsf3==true)
			randomize_k3(A,num_literals,seed);
		else
			randomize_prob(A,seed,num_literals,exact);
		if(cpl == true) solve_by_cplex(A);
		if(oldasat == true) std::vector<int> t = old_asat(A,seed,0.21);
		if(asat == true) std::vector<int> t = solve_by_asat(A,seed,0.21);
	}
	
	catch (std::exception &e){std::cerr <<e.what()<<'\n';}	
	catch(char const* s){std::cerr<<s<<'\n';}
	catch (...) {std::cerr << "Unknown exception caught" <<'\n';}

}
