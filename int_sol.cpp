#include "int_sol.hpp"

ILOSTLBEGIN

static bool solve_cuts(const sat_prob &A, const std::vector<std::tuple<int,int,int,int> > &v);
static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, const sat_prob &A);
static bool integer_test(const std::vector<double> &v);
static bool integer_solution(sat_prob &A);


void test_for_int(const int v,const int c,const int k,const int r,const int s, const bool e){

	boost::random::mt19937 gen;
	
	std::cout<<"#clauses: "<<c<<'\n';
	std::cout<<"#variables: "<<v<<'\n';
	std::cout<<"#runs: "<<r<<'\n';
		for(auto i = 0; i<r;++i){
			gen.seed(s+i);
			sat_prob A(v,c);
			randomize_prob(A, gen, k);
			
			auto start = std::chrono::high_resolution_clock::now();
			bool test = integer_solution(A);
			auto end = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> diff = end-start;
			
			std::cout<<s+i<<" "<<test<<" "<<diff.count()<<" "<<'\n';
		}

}

void test_w_cuts(const int v,const int c,const int k,const int r,const int s, const bool e){
	
	boost::random::mt19937 gen;
	
	std::cout<<"#clauses: "<<c<<'\n';
	std::cout<<"#variables: "<<v<<'\n';
	std::cout<<"#runs: "<<r<<'\n';
	
	for(auto i = 0; i<r;++i){
	
		gen.seed(s+i);
		sat_prob A(v,c);
		randomize_prob(A, gen, k);

		auto start = std::chrono::high_resolution_clock::now();
		bool test = integer_solution(A);
		auto end = std::chrono::high_resolution_clock::now();		
		std::chrono::duration<double> diff = end-start;
		
		if(test)
			std::cout<<i<<" "<<test<<" "<<diff.count()<<" "<<s+i<<'\n';			
		else{
			start = std::chrono::high_resolution_clock::now();
			bool t = solve_cuts(A,find_clique(A));
			end = std::chrono::high_resolution_clock::now();	
			diff = end-start;
			std::cout<<i<<" "<<t<<" "<<diff.count()<<" "<<s+i<<'\n';	
		}
	}

}

static void populatebynonzero (IloModel model, IloNumVarArray x, IloRangeArray c, const sat_prob &A){
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

static bool integer_solution(sat_prob &A){

	IloEnv   env;
	IloNumArray vals(env);
	try{
	
		IloModel model(env);

		IloNumVarArray var(env);
		IloRangeArray con(env);

		populatebynonzero (model, var, con, A);
		
		IloCplex cplex(model);
		
		cplex.setOut(env.getNullStream());
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


static bool solve_cuts(const sat_prob &A, const std::vector<std::tuple<int,int,int,int> > &v){

	IloEnv   lp;
	IloNumArray vals(lp);
	try{
	
		IloModel model(lp);

		IloNumVarArray var(lp);
		IloRangeArray con(lp);

		populatebynonzero (model, var, con, A);
		
		for(size_t n = 0; n<v.size(); ++n){
			
			if(std::get<0>(v[n]) == 1 && std::get<1>(v[n]) == 1){		
				IloConstraint cons(var[std::get<2>(v[n])]+var[std::get<3>(v[n])] <= 1);
				model.add(cons); 
			}
			else if(std::get<0>(v[n]) == 1 && std::get<1>(v[n]) == 0){		
				IloConstraint cons(var[std::get<2>(v[n])]-var[std::get<3>(v[n])] <= 0);
				model.add(cons); 
			}		
			else if(std::get<0>(v[n]) == 0 && std::get<1>(v[n]) == 1){		
				IloConstraint cons(var[std::get<3>(v[n])]-var[std::get<2>(v[n])] <= 0);
				model.add(cons); 
			}
			else{
				IloConstraint cons(-var[std::get<2>(v[n])]-var[std::get<3>(v[n])] <= -1);
				model.add(cons); 				
			}
		}
		
		IloCplex cplex(model);	
		cplex.setOut(lp.getNullStream());
		cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
		cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1.0);
		
		//cplex.setParam(IloCplex::Param::MIP::Limits::Nodes,0);
		
		cplex.solve(); 
		cplex.getValues(vals, var);
	}
	catch (IloException &e) {
		lp.error() << "Failed to optimize LP" << endl;
 		cerr << "Concert exception caught: " << e <<'\n';
	} 
	catch (...) {cerr << "Unknown exception caught" <<'\n';}
	
	std::vector<double> out(vals.getSize());
	for(size_t i=0; i<out.size(); ++i)
  	out[i] = vals[i];

	lp.end(); 
	
	return integer_test(out);
}


