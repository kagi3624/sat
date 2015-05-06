#include <ilcplex/ilocplex.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "randomize_prob.hpp"
#include "asat.hpp"

#include<boost/program_options.hpp>

ILOSTLBEGIN

static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, sat_prob &A){
   IloEnv env = model.getEnv();

	for(long long unsigned int i = 0; i<A.get_num_variables();++i){
		x.add(IloNumVar(env, 0, 1, ILOINT));
		string S = "x" + to_string(i+1);
		x[i].setName(S.c_str());
	}
   
	for(long long unsigned int i = 0; i<A.get_num_clauses(); ++i){
		c.add(IloRange(env, A.range_value(i), IloInfinity));
		string S = "c" + to_string(i+1);
		c[i].setName(S.c_str());
	}

	for(unsigned int i = 0; i<A.get_num_clauses();++i){
		for(unsigned int k = 0; k<A.get_clause(i).get_num_literals();++k){
		
				if(A.get_clause(i).get_literal(k)<0)
					c[i].setLinearCoef(x[-A.get_clause(i).get_literal(k)-1],-1.0);
				else
					c[i].setLinearCoef(x[A.get_clause(i).get_literal(k)-1],  1.0);
		
		}
	}
	
   model.add(c);
}


int main (int argc, char **argv){

	
	
	//IloEnv   env;
	try{
	
		namespace po = boost::program_options;
	
		int num_variables, num_literals, num_clauses, exact, seed;
		
		po::options_description desc("Allowed options");
		desc.add_options()
    ("help", "produce help message")
    ("variables,v", po::value<int>(&num_variables), "set number of variables")
    ("literals,l", po::value<int>(&num_literals)->default_value(3), "set number of maximum literals")
    ("clauses,c", po::value<int>(&num_clauses), "set number of clauses")
    ("seed,s", po::value<int>(&seed)->default_value(1717), "set number of the seed")
    ("exact,e", po::value<int>(&exact)->default_value(1), "set every clause to exact length of given literals");	
		
		po::variables_map vm;
		po::store(po::parse_command_line(argc, argv, desc), vm);
		po::notify(vm); 
		
		
		if (vm.count("help") || argc == 1) {
		  std::cout << desc << "\n";
		  return 1;
		}
		
		if (!vm.count("clauses") || !vm.count("variables")) throw "Error: Give the number of variables and clauses!";


		
		if(num_literals>num_variables) throw "Error: Number of variables can't be lower than the number of literals!";
		
		sat_prob A(num_variables,num_clauses);
	
		randomize_prob(A,seed,num_literals,exact);
		
		A.print_problem();

		std::vector<int> t = solve_by_asat(A,seed,0.21);
		for(size_t i = 0; i<t.size();++i)
			std::cout<<t[i]<<" ";
		std::cout<<'\n';
		
		/*
		IloModel model(env);

		IloNumVarArray var(env);
		IloRangeArray con(env);

		populatebynonzero (model, var, con, A);
		
		IloCplex cplex(model);
		int i = cplex.solve(); 
		cplex.exportModel("lpex1.lp");
		
		
			
		

		
		if (!i){
			env.out() << "Solution status = " << cplex.getStatus() << endl;
		}


		else{
			IloNumArray vals(env);
			cplex.getValues(vals, var);
			env.out() << "Solution status = " << cplex.getStatus() << endl;
			env.out() << "Values        = " << vals << endl;
		}*/
		
	}
	/*
	catch (IloException &e) {
		env.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	} */
	
	catch (std::exception &e){
		cerr <<e.what()<<'\n';
	}	
	
	catch(char const* s){
		std::cerr<<s<<'\n';
	}
	catch (...) {
      cerr << "Unknown exception caught" <<'\n';
	}
	//env.end(); 

}
