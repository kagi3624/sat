#include "wcuts.hpp"

ILOSTLBEGIN

void solve_wcuts(const sat_prob &A){

	IloEnv   env;

	try{
	
		IloModel model(env);

		IloNumVarArray var(env);
		IloRangeArray con(env);

		
		IloCplex cplex(model);
		
		cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
		
		int i = cplex.solve(); 
		cplex.exportModel("lpex1.lp");
		
		if (!i){
			env.out() << "Solution status = " << cplex.getStatus() << endl;
		}


		else{
			IloNumArray vals(env);
			cplex.getValues(vals, var);
			
			
			int s = vals.getSize();
			std::vector<double> sol_out(s);
			for(int i=0; i<s; ++i)
  			sol_out[i] = vals[i];
  			
  			
			env.out() << "Solution status = " << cplex.getStatus() << endl;
			env.out() << "Values        = " << vals << endl;
		}
		
	}
	catch (IloException &e) {
		env.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	} 
	catch (...) {cerr << "Unknown exception caught" <<'\n';}
	
	env.end(); 


}
