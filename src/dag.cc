/**
 * @file dag.cc
 *
 */

#include "dag.h"

#include <deque>
#include <iostream>

Dag::Dag(const Graph &query, const CandidateSet &cs)
    : query_(query), max_label_(query.max_label_) {
  Vertex root;
  double root_criteria = 1.0 / 0.0;

  // Choose DAG root node
  // r <- argmin_{u \in V(q)}(|C(u)| / deg_q(u))
  for (Vertex i = 0; i < static_cast<long>(query.start_offset_.size()) - 1;
       ++i) {
    double temp =
        static_cast<double>(cs.GetCandidateSize(i) / query.GetDegree(i));
    if (temp < root_criteria) {
      root_criteria = temp;
      root = i;
    }
  }
  root_ = root;

  std::vector<std::vector<Vertex>> child_adj_list(query.GetNumVertices());
  std::vector<std::vector<Vertex>> parent_adj_list(query.GetNumVertices());

  std::deque<Vertex> queue;

  // Generate adjacency lists, both ways
  std::vector<bool> visited(query.GetNumVertices(), false);
  std::vector<int> vertex_order(query.GetNumVertices());
  int counter = query.GetNumVertices();
  visited[root] = true;
  vertex_order[root] = counter--;
  queue.push_back(root);
  while (!queue.empty()) {
    Vertex u = queue.front();
    queue.pop_front();
    for (size_t i = query.GetNeighborStartOffset(u);
         i < query.GetNeighborEndOffset(u); i++) {
      Vertex v = query.GetNeighbor(i);
      if (vertex_order[u] > vertex_order[v]) {
        child_adj_list[u].push_back(v);
      } else {
        parent_adj_list[u].push_back(v);
      }
      if (!visited[v]) {
        visited[v] = true;
        vertex_order[v] = counter--;
        queue.push_back(v);
      }
    }
  }

  // Transform adjacency lists into adjacency arrays
  Dag::AdjListToArray(query, child_adj_list, child_adj_array_,
                      child_start_offset_, child_start_offset_by_label_);
  Dag::AdjListToArray(query, parent_adj_list, parent_adj_array_,
                      parent_start_offset_, parent_start_offset_by_label_);
}

Dag::~Dag() {}

void Dag::Dump() {
  int i;

  std::cout << "query_.adj_array_: [";
  for (const Vertex v : query_.adj_array_) {
    std::cout << v << ", ";
  }
  std::cout << "]\n";

  std::cout << "query_.start_offset_: [";
  for (const size_t x : query_.start_offset_) {
    std::cout << x << ", ";
  }
  std::cout << "]\n";

  std::cout << "query_.start_offset_by_label_: [\n";
  i = 0;
  for (const auto p : query_.start_offset_by_label_) {
    std::cout << '(' << p.first << ", " << p.second << "), ";
    if ((i + 1) % (max_label_ + 1) == 0) {
      std::cout << '\n';
    }
    ++i;
  }
  std::cout << "]\n\n";

  std::cout << "child_adj_array_: [";
  for (const Vertex v : child_adj_array_) {
    std::cout << v << ", ";
  }
  std::cout << "]\n";

  std::cout << "child_start_offset_: [";
  for (const size_t x : child_start_offset_) {
    std::cout << x << ", ";
  }
  std::cout << "]\n";

  std::cout << "child_start_offset_by_label_: [\n";
  i = 0;
  for (const auto p : child_start_offset_by_label_) {
    std::cout << '(' << p.first << ", " << p.second << "), ";
    if ((i + 1) % (max_label_ + 1) == 0) {
      std::cout << '\n';
    }
    ++i;
  }
  std::cout << "]\n\n";

  std::cout << "parent_adj_array_: [";
  for (const Vertex v : parent_adj_array_) {
    std::cout << v << ", ";
  }
  std::cout << "]\n";

  std::cout << "parent_start_offset_: [";
  for (const size_t x : parent_start_offset_) {
    std::cout << x << ", ";
  }
  std::cout << "]\n";

  std::cout << "parent_start_offset_by_label_: [\n";
  i = 0;
  for (const auto p : parent_start_offset_by_label_) {
    std::cout << '(' << p.first << ", " << p.second << "), ";
    if ((i + 1) % (max_label_ + 1) == 0) {
      std::cout << '\n';
    }
    ++i;
  }
  std::cout << "]\n\n";
}

void Dag::AdjListToArray(
    const Graph &query, std::vector<std::vector<Vertex>> &adj_list,
    std::vector<Vertex> &adj_array, std::vector<size_t> &start_offset,
    std::vector<std::pair<size_t, size_t>> &start_offset_by_label) {
  size_t labels = query.max_label_ + 1;
  adj_array.resize(query.GetNumEdges());
  start_offset.resize(query.GetNumVertices() + 1);
  start_offset_by_label.resize(query.GetNumVertices() * labels);

  start_offset[0] = 0;
  for (size_t i = 0; i < adj_list.size(); ++i) {
    start_offset[i + 1] = start_offset[i] + adj_list[i].size();
  }

  for (size_t i = 0; i < adj_list.size(); ++i) {
    std::vector<Vertex> &neighbors = adj_list[i];
    if (neighbors.size() == 0) {
      continue;
    }

    // sort neighbors by ascending order of label first, and descending order of
    // degree second
    std::sort(neighbors.begin(), neighbors.end(), [query](Vertex u, Vertex v) {
      if (query.GetLabel(u) != query.GetLabel(v))
        return query.GetLabel(u) < query.GetLabel(v);
      else if (query.GetDegree(u) != query.GetDegree(v))
        return query.GetDegree(u) > query.GetDegree(v);
      else
        return u < v;
    });

    Vertex v = neighbors[0];
    Label l = query.GetLabel(v);

    start_offset_by_label[i * labels + l].first = start_offset[i];

    for (size_t j = 1; j < neighbors.size(); ++j) {
      v = neighbors[j];
      Label next_l = query.GetLabel(v);

      if (l != next_l) {
        start_offset_by_label[i * labels + l].second = start_offset[i] + j;
        start_offset_by_label[i * labels + next_l].first = start_offset[i] + j;
        l = next_l;
      }
    }

    start_offset_by_label[i * labels + l].second = start_offset[i + 1];

    std::copy(adj_list[i].begin(), adj_list[i].end(),
              adj_array.begin() + start_offset[i]);
  }
}