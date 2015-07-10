#include "gcon.hpp"

using namespace lemon;

typedef ListBpGraph::RedNode C_Node;
typedef ListBpGraph::BlueNode V_Node;
typedef ListBpGraph Graph;

int gcon(const sat_prob &A){

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
