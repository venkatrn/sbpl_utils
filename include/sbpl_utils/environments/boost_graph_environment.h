#pragma once

#include <sbpl/headers.h>

#include <limits>
#include <map>
#include <vector>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

// Macros are evil, but.....
#define GRAPH_TEMPLATE template<class Graph>
#define GRAPH_CLASS BGEnvironment<Graph>

namespace bo = boost;

namespace sbpl_utils {

// An environment compatible with SBPL planners designed to work with
// boost graphs. The template parameter "Graph" should be a boost graph type (e.g.
// boost::adjacency list) with bundled vertex and edge properties. The bundled
// properties should include *at least* these two properties: "heuristic" for
// vertex bundles, and "cost" for edge bundles. An example is provided below:
//
// #include <sbpl_utils/environments/boost_graph_environment.h>
// #include <sbpl/headers.h>
// using namespace sbpl_utils;
// SimpleGraph g(5);
// // (from, to, cost)
// vector<vector<int>> edges = {
//   {0, 1, 10},
//   {1, 2, 10},
//   {2, 3, 10},
//   {0, 3, 5},
// };
// vector<vector<int>> heuristics = {
//   {0, 12},
//   {1, 11},
//   {2, 10},
//   {3, 9},
//   {4, 8},
// };
// auto edge_cost_map = get(&EdgeWithCost::cost, g);
// for (const auto &edge : edges) {
//   edge_cost_map[add_edge(edge[0], edge[1], g).first] = edge[2];
// }
// for (const auto &heuristic : heuristics) {
//   g[heuristic[0]].heuristic = heuristic[1];
// }
// BGEnvironment<SimpleGraph> bg_env(g);
// std::unique_ptr<SBPLPlanner> planner(new ARAPlanner(&bg));
// planner->(..do stuff..)
// Look at boost_environment_test.cpp for more usage examples.

GRAPH_TEMPLATE
class BGEnvironment : public virtual DiscreteSpaceInformation {

 public:
  typedef typename bo::property_map<Graph, bo::vertex_index_t>::type IndexMap;
  typedef typename bo::graph_traits<Graph> GraphTraits;
  typedef typename GraphTraits::vertex_descriptor Vertex;
  typedef typename bo::graph_traits<Graph>::edge_descriptor Edge;
  typedef typename GraphTraits::out_edge_iterator OutEdgeIterator;
  typedef typename bo::vertex_bundle_type<Graph>::type VertexType;
  typedef typename bo::edge_bundle_type<Graph>::type EdgeType;


  BGEnvironment(const Graph &g);
  ~BGEnvironment();
  virtual void GetSuccs(int parent_id, std::vector<int> *succ_ids,
                        std::vector<int> *costs) override;
  virtual void GetLazySuccs(int parent_id, std::vector<int> *succ_ids,
                            std::vector<int> *costs, std::vector<bool> *true_costs) override;
  virtual int GetGoalHeuristic(int state_id) override;

  // Unused methods, need dummy definitions to make them non-abstract.
  virtual bool InitializeEnv(const char *) override {};
  virtual bool InitializeMDPCfg(MDPConfig *) override {};
  virtual int GetFromToHeuristic(int, int) override {
    return 0;
  }
  virtual int GetStartHeuristic(int) override {
    return 0;
  }
  virtual void GetPreds(int, std::vector<int> *, std::vector<int> *) override {}
  virtual void SetAllActionsandAllOutcomes(CMDPSTATE *) override {}
  virtual void SetAllPreds(CMDPSTATE *) override {}
  virtual int SizeofCreatedEnv() override {
    return 0;
  }
  virtual void PrintState(int, bool, FILE *) override {};
  virtual void PrintEnv_Config(FILE *) override {};

  // Allow derived classes to access these.
 protected:
  Graph graph_;
  decltype(get(&VertexType::heuristic, graph_)) heuristic_map_;
  decltype(get(&EdgeType::cost, graph_)) edge_cost_map_;
};

///////////////////////////////////////////////////////////////////////////
//i////////////////// Template Implementation /////////////////////////////
///////////////////////////////////////////////////////////////////////////

GRAPH_TEMPLATE
GRAPH_CLASS::BGEnvironment(const Graph &graph) : graph_(graph) {
  heuristic_map_ = get(&VertexType::heuristic, graph_);
  edge_cost_map_ = get(&EdgeType::cost, graph_);

  // Sadly, this is needed for backward compatibility with old SBPL planners.
  for (int vertex_id = 0; vertex_id < num_vertices(graph_); ++vertex_id) {
    int *entry = new int[NUMOFINDICES_STATEID2IND];
    StateID2IndexMapping.push_back(entry);

    for (int ii = 0; ii < NUMOFINDICES_STATEID2IND; ++ii) {
      StateID2IndexMapping[vertex_id][ii] = -1;
    }
  }

}

GRAPH_TEMPLATE
GRAPH_CLASS::~BGEnvironment() {
  // More sadness.
  for (auto it = StateID2IndexMapping.begin(); it != StateID2IndexMapping.end();) {
    if (*it == nullptr) {
      it = StateID2IndexMapping.erase(it);
    } else {
      ++it;
    }
  }
}


GRAPH_TEMPLATE
void GRAPH_CLASS::GetSuccs(int parent_id, std::vector<int> *succ_ids,
                           std::vector<int> *costs) {
  succ_ids->clear();
  costs->clear();
  IndexMap index_map = get(bo::vertex_index, graph_);
  auto parent_vertex = bo::vertex(parent_id, graph_);
  OutEdgeIterator out_i, out_end;
  Edge e;

  for (std::tie(out_i, out_end) = out_edges(parent_vertex, graph_);
       out_i != out_end; ++out_i) {
    e = *out_i;
    // Vertex src = bo::source(e, graph_);
    Vertex targ = bo::target(e, graph_);
    succ_ids->push_back(index_map[targ]);
    costs->push_back(edge_cost_map_[e]);
  }
}

GRAPH_TEMPLATE
void GRAPH_CLASS::GetLazySuccs(int parent_id, std::vector<int> *succ_ids,
                               std::vector<int> *costs, std::vector<bool> *true_costs) {
  GetSuccs(parent_id, succ_ids, costs);
  true_costs->resize(succ_ids->size(), true);
}


GRAPH_TEMPLATE
int GRAPH_CLASS::GetGoalHeuristic(int state_id) {
  return heuristic_map_[bo::vertex(state_id, graph_)];
}

///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

// Some common vertex and edge bundles.
struct VertexWithHeuristic {
  int heuristic = 0;
};

struct EdgeWithCost {
  int cost = std::numeric_limits<int>::max();
};

struct EdgeWithCostAndProbability {
  int cost = std::numeric_limits<int>::max();
  double probability = 1.0;
  double evaluation_time = 0.0;
};

// Typedef for some common graph types.
// Vertex has heuristic, edges have cost.
typedef bo::adjacency_list<bo::vecS, bo::vecS, bo::undirectedS, VertexWithHeuristic, EdgeWithCost>
SimpleGraph;
typedef bo::adjacency_list<bo::vecS, bo::vecS, bo::bidirectionalS, VertexWithHeuristic, EdgeWithCost>
SimpleDiGraph;

// Vertex has heuristic, edges have cost, probability of existence and
// evaluation time.
typedef bo::adjacency_list<bo::vecS, bo::vecS, bo::undirectedS, VertexWithHeuristic, EdgeWithCostAndProbability>
StochasticGraph;
typedef bo::adjacency_list<bo::vecS, bo::vecS, bo::bidirectionalS, VertexWithHeuristic, EdgeWithCostAndProbability>
StochasticDiGraph;

} // namespace sbpl_utils
