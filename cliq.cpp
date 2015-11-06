#include "cliq.hpp"

ILOSTLBEGIN


static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, const sat_prob &A, int f, int g, int u, int h){

	IloEnv env = model.getEnv();
   
	for(long long unsigned int i = 0; i<A.get_num_variables();++i){
		x.add(IloNumVar(env, 0, 1, ILOFLOAT));
		string S = "x" + to_string(i+1);
		x[i].setName(S.c_str());
	}

	long long unsigned int j;
	for(j = 0; j<A.get_num_clauses(); ++j){
		c.add(IloRange(env, A.range_value(j), IloInfinity));
		string S = "c" + to_string(j+1);
		c[j].setName(S.c_str());
	}

	for(unsigned int i = 0; i<A.get_num_clauses();++i){
		for(unsigned int k = 0; k<A.get_clause(i).get_num_literals();++k){
		
				if(A.get_clause(i).get_literal(k)<0)
					c[i].setLinearCoef(x[-A.get_clause(i).get_literal(k)-1],-1.0);
				else
					c[i].setLinearCoef(x[A.get_clause(i).get_literal(k)-1],  1.0);
		
		}
	}
	
	IloConstraint cons1(x[u] == f);  
	IloConstraint cons2(x[h] == g);
	
	string S = "c" + to_string(j+2);
	cons1.setName(S.c_str());

	S = "c" + to_string(j+3);
	cons1.setName(S.c_str());
	
	model.add(cons1);
	model.remove(cons1);
	model.add(cons2);
	//model.add(obj);
  model.add(c);
}

std::vector<std::vector<int> > find_clique(const sat_prob &A, int f, int g){

	std::vector<std::vector<int> > results;

	for(unsigned int h = 0; h<A.get_num_variables()-1; ++h)
	for(unsigned int u = h+1; u<A.get_num_variables(); ++u){
		IloEnv   lp;

		try{
		
			IloModel model(lp);
			
			IloNumVarArray var(lp);
			IloRangeArray con(lp);
	
			populatebynonzero (model, var, con, A, f, g, u, h);
			
			IloCplex cplex(model);

			cplex.setOut(lp.getNullStream());
			cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
			//cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1.0);

			//cplex.setParam(IloCplex::Param::MIP::Limits::Nodes,0);

			int i = cplex.solve(); 
			//cplex.exportModel("LP.lp");

			results.push_back({i,(int)u,(int)h});

		}
		catch (IloException &e) {
			lp.error() << "Failed to optimize LP" << endl;
			cerr << "Concert exception caught: " << e <<'\n';
		} 
		catch (...) {cerr << "Unknown exception caught" <<'\n';}
		
		lp.end();
		
		return {};
	}
	return results;
}

void write(const std::vector<std::vector<int> > &v){

	for(unsigned int i = 0; i<v.size();++i)
	for(unsigned int j = 0; j<v[i].size();++j){
		if(j==v[i].size()-1)
			std::cout<<v[i][j]<<'\n';
		std::cout<<v[i][j]<<" ";
		
	}


}
