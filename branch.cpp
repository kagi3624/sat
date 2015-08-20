
#include "branch.hpp"

ILOSTLBEGIN



static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, sat_prob A){
	IloEnv env = model.getEnv();
   
	IloObjective obj = IloMaximize(env);  


	for(unsigned int i = 0; i<A.get_num_variables();++i){
		x.add(IloNumVar(env, 0, 1, ILOFLOAT));
		string S = "x" + to_string(i+1);
		x[i].setName(S.c_str());
	}

	for(unsigned int i = 0; i<A.get_num_clauses(); ++i){
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
	
	
	IloExpr e(env);
	
	for(size_t i = 0; i<A.get_num_cuts();++i)
		for(size_t j = 0; j<A.get_cut(i).left.size();++j){
		
		}

	model.add(obj);
  model.add(c);
}



void solve_by_branching(const sat_prob &A){

	std::queue<sat_prob> problem_stack;
	problem_stack.push(A);
	
	for(auto i = 0; i<2;++i){
	
	
	
	}

}
