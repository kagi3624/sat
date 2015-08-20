#include "int_sol.hpp"

ILOSTLBEGIN

static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, sat_prob A);
static bool integer_test(const std::vector<double> &v);
static bool integer_solution(const sat_prob &A);


void test_for_int(const int v,const int c,const int k,const int r,const int s, const bool e){

	boost::random::mt19937 gen(s);
	boost::random::uniform_int_distribution<int> random_seed(1,2000000000);
	
	std::cout<<"#clauses: "<<c<<'\n';
	std::cout<<"#variables: "<<v<<'\n';
	std::cout<<"#runs: "<<r<<'\n';
		for(auto i = 0; i<r;++i){
			sat_prob A(v,c);
			randomize_prob(A, random_seed(gen), k);
			
			auto start = std::chrono::high_resolution_clock::now();
			bool test = integer_solution(A);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end-start;
			
			auto greatest_connectivity = gcon(A);
			
			std::cout<<i<<" "<<test<<" "<<greatest_connectivity<<" "<<diff.count()<<'\n';
		}

}



static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, sat_prob A){
	IloEnv env = model.getEnv();
   
	IloObjective obj = IloMaximize(env);   

	for(size_t i = 0; i<A.get_num_variables();++i){
		x.add(IloNumVar(env, 0, 1, ILOFLOAT));
		string S = "x" + to_string(i+1);
		x[i].setName(S.c_str());
	}
   
	for(size_t i = 0; i<A.get_num_clauses(); ++i){
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

static bool integer_test(const std::vector<double> &v){
	double dummy;
	for(size_t i = 0; i<v.size();++i){
			if(modf (v[i] , &dummy) != 0.0) 
				return false;
		}
  return true;  
}

static bool integer_solution(const sat_prob &A){

	IloEnv   env;
	IloNumArray vals(env);
	try{
	
		IloModel model(env);

		IloNumVarArray var(env);
		IloRangeArray con(env);

		populatebynonzero (model, var, con, A);
		
		IloCplex cplex(model);
		
		//cplex.setOut(env.getNullStream());
		cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
		
		cplex.solve(); 

		
		cplex.getValues(vals, var);
		//env.out() << "Values:\n" << vals << endl;

			
	}
	catch (IloException &e) {
		env.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	} 
	catch (...) {cerr << "Unknown exception caught" <<'\n';}
	
	std::vector<double> out(vals.getSize());
	for(size_t i=0; i<out.size(); ++i)
  	out[i] = vals[i];
	env.end(); 
	
	return integer_test(out);
}
