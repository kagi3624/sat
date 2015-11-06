#include "cplex.hpp"

ILOSTLBEGIN


static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, const sat_prob &A,  const bool lp = true){
	
	IloEnv env = model.getEnv();
     
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
	
	/*IloConstraint cons1(x[0] == 1);
	cons1.setName("c9");
  IloConstraint cons2(x[3] == 1);
  cons2.setName("c10");
	
	model.add(cons1);
	model.add(cons2);*/
	//model.add(obj);
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
		cplex.exportModel("LP.lp");
		
		if (!i){
			lp.out() << "Solution status = " << cplex.getStatus() << endl;
		}


		else{
			IloNumArray vals(lp);
			IloNumArray slaks(lp);
			
			cplex.getValues(vals, var);
  		cplex.getSlacks(slaks, con);
  		
			lp.out() << "Solution status = " << cplex.getStatus() << endl;
			//lp.out() << "Slacks = " << slaks << '\n';
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
		
		//cplex.setOut(ilp.getNullStream());
		cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
		//cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1.0);
		
		//cplex.setParam(IloCplex::Param::MIP::Cuts::Cliques, 3);
		//cplex.setParam(IloCplex::Param::MIP::Cuts::ZeroHalfCut, -1);
		//cplex.setParam(IloCplex::Param::MIP::Limits::Nodes,0);
		
		int j = cplex.solve(); 
		cplex.exportModel("ILP.lp");
		
		if (!j){
			ilp.out() << "Solution status = " << cplex.getStatus() << endl;
		}


		else{
			IloNumArray vals(ilp);
			cplex.getValues(vals, var);
		
  			
			ilp.out() << "Solution status = " << cplex.getStatus() << endl;
			ilp.out() << "IP values        = " << vals << '\n';
			
			/*ilp.out()<<"Cliques: " <<cplex.getNcuts(IloCplex::CutClique)<<'\n';
			ilp.out()<<"CutCover: " <<cplex.getNcuts(IloCplex::CutCover)<<'\n';
			ilp.out()<<"CutGubCover: " <<cplex.getNcuts(IloCplex::CutGubCover)<<'\n';
			ilp.out()<<"CutFlowCover: " <<cplex.getNcuts(IloCplex::CutFlowCover)<<'\n';
			ilp.out()<<"CutFrac: " <<cplex.getNcuts(IloCplex::CutFrac)<<'\n';
			ilp.out()<<"CutMCF: " <<cplex.getNcuts(IloCplex::CutMCF)<<'\n';
			ilp.out()<<"CutMir: " <<cplex.getNcuts(IloCplex::CutMir)<<'\n';
			ilp.out()<<"CutFlowPath: " <<cplex.getNcuts(IloCplex::CutFlowPath)<<'\n';
			ilp.out()<<"CutDisj: " <<cplex.getNcuts(IloCplex::CutDisj)<<'\n';
			ilp.out()<<"CutImplBd: " <<cplex.getNcuts(IloCplex::CutImplBd)<<'\n';
			ilp.out()<<"CutZeroHalf: " <<cplex.getNcuts(IloCplex::CutZeroHalf)<<'\n';
			ilp.out()<<"CutLocalCover: " <<cplex.getNcuts(IloCplex::CutLocalCover)<<'\n';
			ilp.out()<<"CutTighten: " <<cplex.getNcuts(IloCplex::CutTighten)<<'\n';
			ilp.out()<<"CutObjDisj: " <<cplex.getNcuts(IloCplex::CutObjDisj)<<'\n';*/
		}
		
	}
	catch (IloException &e) {
		ilp.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	} 
	catch (...) {cerr << "Unknown exception caught" <<'\n';}
	
	ilp.end(); 
}
