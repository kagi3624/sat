#include <ilcplex/ilocplex.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "randomize_prob.hpp"
#include "asat.hpp"

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
	IloEnv   env;
	try{
		int num_variables, num_literals, num_clauses, exact = 0;
		num_variables = atoi(argv[1]);
		num_clauses   = atoi(argv[2]);
		num_literals  = atoi(argv[3]);
		
		if(num_literals>num_variables) throw "Error: Number of variables can't be lower than the number of literals!";
		
		if(argc>4)
			exact     = atoi(argv[4]);
	
		sat_prob A;
	
		randomize_prob(A,num_variables,num_clauses,num_literals,exact);
		
		IloModel model(env);

		IloNumVarArray var(env);
		IloRangeArray con(env);

		populatebynonzero (model, var, con, A);
		
		IloCplex cplex(model);
		int i = cplex.solve();
		cplex.exportModel("lpex1.lp");
		
		
			
		A.print_problem();
		std::vector<int> t =solve_asat(A,0.21);
		for(std::size_t i = 0; i<t.size();++i)
			std::cout<<t[i]<<" ";
		std::cout<<'\n';
		
		if (!i){
			env.out() << "Solution status = " << cplex.getStatus() << endl;
		}


		else{
			IloNumArray vals(env);
			cplex.getValues(vals, var);
			env.out() << "Solution status = " << cplex.getStatus() << endl;
			env.out() << "Values        = " << vals << endl;
		}
		
	}

	catch (IloException& e) {
		env.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	}
	catch(char const* s){
		std::cerr<<s<<'\n';
	}
	catch (...) {
      cerr << "Unknown exception caught" <<'\n';
	}
	env.end(); 

}
