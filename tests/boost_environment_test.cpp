#include <sbpl_utils/environments/boost_graph_environment.h>
#include <sbpl/headers.h>

#include <stdexcept>

#include <gtest/gtest.h>

using namespace sbpl_utils;
using namespace std;

void TEST_SIMPLE_GRAPH() {
  SimpleGraph g(6);

  // (from, to, cost)
  vector<vector<int>> edges = {
    {0, 1, 10},
    {1, 2, 10},
    {2, 3, 10},
    {3, 4, 10},
    {0, 5, 20},
    {5, 4, 10},
  };

  vector<vector<int>> heuristics = {
    {0, 12},
    {1, 11},
    {2, 10},
    {3, 9},
    {4, 8},
  };

  auto edge_cost_map = get(&EdgeWithCost::cost, g);

  for (const auto &edge : edges) {
    edge_cost_map[add_edge(edge[0], edge[1], g).first] = edge[2];
  }

  for (const auto &heuristic : heuristics) {
    g[heuristic[0]].heuristic = heuristic[1];
  }

  BGEnvironment<SimpleGraph> bg_env(g);
  vector<int> succ_ids, costs;
  int parent_id = 0;
  bg_env.GetSuccs(parent_id, &succ_ids, &costs);

  printf("Successors for %d\n", parent_id);

  for (size_t ii = 0; ii < succ_ids.size(); ++ii) {
    printf("(%d,%d)  : %d\n", parent_id, succ_ids[ii], costs[ii]);
  }

  printf("\n");
  cout << "Heuristic: " << bg_env.GetGoalHeuristic(parent_id) << endl;
}

void TEST_SIMPLE_GRAPH_WITH_PLANNER() {
  SimpleGraph g(6);

  // (from, to, cost)
  vector<vector<int>> edges = {
    {0, 1, 10},
    {1, 2, 10},
    {2, 3, 10},
    {3, 4, 10},
    {4, 5, 10},
    {0, 5, 200},
  };

  vector<vector<int>> heuristics = {
    {0, 0},
    {1, 1},
    {2, 0},
    {3, 0},
    {4, 0},
    {5, 0},
  };

  auto edge_cost_map = get(&EdgeWithCost::cost, g);

  for (const auto &edge : edges) {
    edge_cost_map[add_edge(edge[0], edge[1], g).first] = edge[2];
  }

  for (const auto &heuristic : heuristics) {
    g[heuristic[0]].heuristic = heuristic[1];
  }

  BGEnvironment<SimpleGraph> bg_env(g);

  // Construct and test the planner.
  unique_ptr<SBPLPlanner> planner(new ARAPlanner(&bg_env, true)); 
  // unique_ptr<SBPLPlanner> planner(new LazyARAPlanner(&bg_env, true)); 
  planner->set_start(0);
  planner->set_goal(5);
  ReplanParams params(1.0);
  params.initial_eps = 1.0;
  params.final_eps = 1.0;
  params.return_first_solution = true;
  vector<int> solution_ids;
  int solution_cost = 0;
  planner->replan(&solution_ids, params, &solution_cost);
  printf("Found solution with cost %d:\n", solution_cost);
  for (size_t ii = 0; ii < solution_ids.size(); ++ii) {
    cout << solution_ids[ii] << " ";
  }
  cout << endl;
}

void TEST_STOCHASTIC_GRAPH() {
  StochasticGraph g(6);

  // (from, to, cost, probability of existence, evaluation time)
  vector<vector<double>> edges = {
    {0, 1, 10, 0.1, 1.3},
    {1, 2, 10, 1.0, 1.5},
    {2, 3, 10, 1.0, 2.5},
    {3, 4, 10, 0.5, 3.5},
    {0, 5, 20, 1.0, 1.0},
    {5, 4, 10, 0.6, 0.1},
  };

  vector<vector<int>> heuristics = {
    {0, 12},
    {1, 11},
    {2, 10},
    {3, 9},
    {4, 8},
  };

  auto edge_bundle_map = get(bo::edge_bundle, g);

  for (const auto &edge : edges) {
    auto& edge_properties = edge_bundle_map[add_edge(edge[0], edge[1], g).first];
    edge_properties.cost = static_cast<int>(edge[2]);
    edge_properties.probability = edge[3];
    edge_properties.evaluation_time = edge[4];
  }

  for (const auto &heuristic : heuristics) {
    g[heuristic[0]].heuristic = heuristic[1];
  }

  BGEnvironment<StochasticGraph> bg_env(g);
  vector<int> succ_ids, costs;
  int parent_id = 0;
  bg_env.GetSuccs(parent_id, &succ_ids, &costs);

  printf("Successors for %d\n", parent_id);

  for (size_t ii = 0; ii < succ_ids.size(); ++ii) {
    printf("(%d,%d)  : %d\n", parent_id, succ_ids[ii], costs[ii]);
  }

  printf("\n");
  cout << "Heuristic: " << bg_env.GetGoalHeuristic(parent_id) << endl;
}

int main() {
  // TODO: convert to gtest.
  TEST_SIMPLE_GRAPH();
  TEST_STOCHASTIC_GRAPH();
  TEST_SIMPLE_GRAPH_WITH_PLANNER();
}
