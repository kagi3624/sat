#include "cliq.hpp"



ILOSTLBEGIN


std::vector<std::tuple<int,int,int,int> > find_clique(const sat_prob &A){

	std::vector<std::tuple<int,int,int,int> > results;

		IloEnv lp;
		
		try{
			IloModel model(lp);
			
			IloNumVarArray var(lp);
      IloRangeArray con(lp);
      
      long long unsigned int t;
			for(t = 0; t<A.get_num_variables();++t){
				var.add(IloNumVar(lp, 0, 1));
				string S = "x" + to_string(t+1);
				var[t].setName(S.c_str());
			}
			
			for(long long unsigned int i = 0; i<A.get_num_clauses(); ++i){
				con.add(IloRange(lp, A.range_value(i), IloInfinity));
				string S = "c" + to_string(i+1);
				con[i].setName(S.c_str());
			}
			
			for(unsigned int i = 0; i<A.get_num_clauses();++i)
			for(unsigned int k = 0; k<A.get_clause(i).get_num_literals();++k){
				if(A.get_clause(i).get_literal(k)<0)
					con[i].setLinearCoef(var[-A.get_clause(i).get_literal(k)-1],-1.0);
				else
					con[i].setLinearCoef(var[A.get_clause(i).get_literal(k)-1],  1.0);
			}
      
      model.add(con);
       
      for(unsigned int i = 0; i<A.get_num_variables()-1;++i)
      for(unsigned int j = i+1; j<A.get_num_variables();++j){
      	
      	//for 00
      	IloConstraint cons1(var[i] == 0);
      	IloConstraint cons2(var[j] == 0);

      	cons1.setName("cons1");
      	cons2.setName("cons2");
      	
      	model.add(cons1);
				model.add(cons2);
				
				IloCplex cplex(model);
				cplex.setOut(lp.getNullStream());
				cplex.setParam(IloCplex::RootAlg, IloCplex::Dual);
				cplex.setParam(IloCplex::Param::MIP::Tolerances::MIPGap, 1.0);
				
				int solution = cplex.solve(); 
				
				if(!solution){
					
					results.push_back(std::make_tuple(0,0,i,j));
					//string S3 = "lps/00_lp"+to_string(i)+to_string(j)+".lp";
					//cplex.exportModel(S3.c_str());
				}
				

				
				model.remove(cons1);
				model.remove(cons2);
				
				//for 01
      	IloConstraint cons3(var[i] == 0);
      	IloConstraint cons4(var[j] == 1);

      	cons3.setName("cons3");
      	cons4.setName("cons4");
      	
      	model.add(cons3);
				model.add(cons4);
				
				
				solution = cplex.solve(); 
				
				if(!solution){
					
					results.push_back(std::make_tuple(0,1,i,j));
					//string S3 = "lps/01_lp"+to_string(i)+to_string(j)+".lp";
					//cplex.exportModel(S3.c_str());
				}
				

				
				model.remove(cons3);
				model.remove(cons4);
				
				//for 10
      	IloConstraint cons5(var[i] == 1);
      	IloConstraint cons6(var[j] == 0);

      	cons5.setName("cons5");
      	cons6.setName("cons6");
      	
      	model.add(cons5);
				model.add(cons6);
				
				
				solution = cplex.solve(); 
				
				if(!solution){
					
					results.push_back(std::make_tuple(1,0,i,j));
					//string S3 = "lps/10_lp"+to_string(i)+to_string(j)+".lp";
					//cplex.exportModel(S3.c_str());
				}
				

				
				model.remove(cons5);
				model.remove(cons6);
				
				//for 11
      	IloConstraint cons7(var[i] == 1);
      	IloConstraint cons8(var[j] == 1);

      	cons7.setName("cons7");
      	cons8.setName("cons8");
      	
      	model.add(cons7);
				model.add(cons8);
				
				
				solution = cplex.solve(); 
				
				if(!solution){
					
					results.push_back(std::make_tuple(1,1,i,j));
					//string S3 = "lps/01_lp"+to_string(i)+to_string(j)+".lp";
					//cplex.exportModel(S3.c_str());
				}
				

				
				model.remove(cons7);
				model.remove(cons8);
				
				//std::cout<<i<<" "<<j<<'\n';
			}
			
			
		}
		
		catch (IloException &e) {
			lp.error() << "Failed to optimize LP" << endl;
			cerr << "Concert exception caught: " << e <<'\n';
		} 
		catch (...) {cerr << "Unknown exception caught" <<'\n';}
		
		lp.end();
		
	
	return results;
}


void write(const std::vector<std::tuple<int,int,int,int> > &v){

	for(size_t i = 0; i<v.size();++i){
		std::cout<<"["<<std::get<0>(v[i])<<","<<std::get<1>(v[i])<<"] -> "
						 <<std::get<2>(v[i])<<" "<<std::get<3>(v[i])<<'\n';
	}
}
