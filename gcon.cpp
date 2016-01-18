#include "gcon.hpp"

using namespace lemon;

typedef ListBpGraph::RedNode C_Node;
typedef ListBpGraph::BlueNode V_Node;
typedef ListBpGraph Graph;

int gcon1(const sat_prob &A){

	Graph g;
	std::vector<C_Node> C;
  std::vector<V_Node> V; 
  Graph::NodeMap<int> Map(g);
  
  for(size_t i = 0; i<A.get_num_variables();++i)
  	V.push_back(g.addBlueNode());
  
  for(size_t i = 0; i<A.get_num_clauses();++i)
  	C.push_back(g.addRedNode());
  	
  for(size_t i = 0;i<A.get_num_clauses();++i)
  	for(size_t j = 0;j<A.get_clause(i).get_num_literals();++j)
  		g.addEdge(C[i],V[abs(A.get_clause(i).get_literal(j))-1]);
  
  
  std::vector<int> compsize(connectedComponents(g,Map));
  
  for(Graph::NodeIt it(g); it!=INVALID;++it)
		compsize[Map[it]]++;
	
	return *max_element(compsize.begin(),compsize.end());
}


int gcon2(const sat_prob &A){

	Graph g;
	Graph::NodeMap<int> Map(g);
	std::vector<C_Node> C;
	
	for(size_t i = 0; i<A.get_num_clauses();++i)
  	C.push_back(g.addRedNode());



	std::vector<V_Node> V1(A.get_num_variables());
	std::vector<V_Node> V2(A.get_num_variables());  
	
	for(size_t i = 0;i<A.get_num_clauses();++i)
  for(size_t j = 0;j<A.get_clause(i).get_num_literals();++j){
  	if(A.get_clause(i).get_literal(j)<0)
  		V1[abs(A.get_clause(i).get_literal(j))-1] = g.addBlueNode();
		else
			V2[abs(A.get_clause(i).get_literal(j))-1] = g.addBlueNode();
	}
	
	for(size_t i = 0;i<A.get_num_clauses();++i)
  for(size_t j = 0;j<A.get_clause(i).get_num_literals();++j){
  		if(A.get_clause(i).get_literal(j)<0)
  			g.addEdge(C[i],V1[abs(A.get_clause(i).get_literal(j))-1]);
  		else
  			g.addEdge(C[i],V2[abs(A.get_clause(i).get_literal(j))-1]);
  }

  std::vector<int> compsize(connectedComponents(g,Map));
  
  for(Graph::NodeIt it(g); it!=INVALID;++it)
		compsize[Map[it]]++;
	
	return *max_element(compsize.begin(),compsize.end());
}

void gcon_test(const sat_prob &A){

	int g1 = gcon1(A);
	int g2 = gcon2(A);
	
	//std::cout<<"#clauses: "<<A.get_num_clauses()<<'\n';
	//std::cout<<"#variables: "<<A.get_num_variables()<<'\n';
	std::cout<<std::setprecision(8)
					 <<(double)A.get_num_clauses()/A.get_num_variables()<<" "
					 <<(double)g1/(A.get_num_clauses()+A.get_num_variables())<<" "
					 <<(double)g2/(A.get_num_clauses()+2*A.get_num_variables())<<'\n';
}
