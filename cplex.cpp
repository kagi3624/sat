#include "cplex.hpp"

ILOSTLBEGIN


static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, sat_prob A,  const bool lp = true){
	IloEnv env = model.getEnv();
   
	IloObjective obj = IloMaximize(env);  


	for(long long unsigned int i = 0; i<A.get_num_variables();++i){
		x.add(IloNumVar(env, 0, 1, lp ? ILOFLOAT : ILOINT));
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
	model.add(obj);
  model.add(c);
}

void solve_by_cplex(const sat_prob &A){

	IloEnv   lp;

	try{
	
		IloModel model(lp);

		IloNumVarArray var(lp);
		IloRangeArray con(lp);

		populatebynonzero (model, var, con, A);
		
		IloCplex cplex(model);
		
		cplex.setOut(lp.getNullStream());
		cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
		//cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1.0);
		//cplex.setParam(IloCplex::Param::MIP::Limits::Nodes,0);
		
		int i = cplex.solve(); 
		//cplex.exportModel("lpex1.lp");
		
		if (!i){
			lp.out() << "Solution status = " << cplex.getStatus() << endl;
		}


		else{
			IloNumArray vals(lp);
			IloNumArray slaks(lp);
			
			cplex.getValues(vals, var);
  		cplex.getSlacks(slaks, con);
  		
			lp.out() << "Solution status = " << cplex.getStatus() << endl;
			lp.out() << "Slacks = " << slaks << '\n';
			lp.out() << "LP values        = " << vals << "\n\n";
		}
		
	}
	catch (IloException &e) {
		lp.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	} 
	catch (...) {cerr << "Unknown exception caught" <<'\n';}
	
	lp.end(); 
	
	
	IloEnv   ilp;

	try{
	
		IloModel model(ilp);

		IloNumVarArray var(ilp);
		IloRangeArray con(ilp);

		populatebynonzero (model, var, con, A, false);
		
		IloCplex cplex(model);
		
		cplex.setOut(ilp.getNullStream());
		cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
		cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1.0);
		//cplex.setParam(IloCplex::Param::MIP::Limits::Nodes,0);
		
		int i = cplex.solve(); 
		//cplex.exportModel("lpex1.lp");
		
		if (!i){
			lp.out() << "Solution status = " << cplex.getStatus() << endl;
		}


		else{
			IloNumArray vals(ilp);
			cplex.getValues(vals, var);
  			
			ilp.out() << "Solution status = " << cplex.getStatus() << endl;
			ilp.out() << "IP values        = " << vals << '\n';
		}
		
	}
	catch (IloException &e) {
		ilp.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	} 
	catch (...) {cerr << "Unknown exception caught" <<'\n';}
	
	ilp.end(); 
}
