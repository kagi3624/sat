#include "cliq.hpp"

ILOSTLBEGIN


std::vector<std::vector<int> > find_clique(const sat_prob &A, int f, int g){

	std::vector<std::vector<int> > results;

		IloEnv lp;
		
		try{
			IloModel model(lp);
			
			IloNumVarArray var(lp);
      IloRangeArray con(lp);
      
			for(long long unsigned int i = 0; i<A.get_num_variables();++i){
				var.add(IloNumVar(lp, 0, 1));
				string S = "x" + to_string(i+1);
				var[i].setName(S.c_str());
			}
      
      IloCplex cplex(model);
      int solution = cplex.solve(); 
		
		}
		
		catch (IloException &e) {
			lp.error() << "Failed to optimize LP" << endl;
			cerr << "Concert exception caught: " << e <<'\n';
		} 
		catch (...) {cerr << "Unknown exception caught" <<'\n';}
		
		lp.end();
		
	
	return results;
}
