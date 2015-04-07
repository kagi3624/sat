#include <ilcplex/ilocplex.h>
#include <string>
#include <time.h>
#include <stdlib.h>
#include "sat_prob.hpp"

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

	srand48(time(NULL));
	
	sat_prob A;
	clause a,b,c,d;
	a.add_literal(1);
	a.add_literal(2);
	b.add_literal(1);
	b.add_literal(-2);
	c.add_literal(-1);
	c.add_literal(2);
	d.add_literal(-1);
	d.add_literal(-2);
	
	A.add_clause(a);
	A.add_clause(b);
	A.add_clause(c);
	A.add_clause(d);
	
	A.set_num_variables(2);
	
	IloEnv   env;
	try{
	
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
		}

	}

	catch (IloException& e) {
		env.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	}
	catch (...) {
      cerr << "Unknown exception caught" <<'\n';
	}

	env.end(); 
	
	A.print_problem();

}
