/**
 * @file checkergraph.cc
 *
 */

#include "checkergraph.h"

namespace {
std::vector<Label> transferred_label;
void TransferLabel(const std::string &filename) {
  std::ifstream fin(filename);

  if (!fin.is_open()) {
    std::cout << "Graph file " << filename << " not found!\n";
    exit(EXIT_FAILURE);
  }

  char type;
  int32_t graph_id_;
  size_t num_vertices_;

  std::set<Label> label_set;

  fin >> type >> graph_id_ >> num_vertices_;

  // preprocessing
  while (fin >> type) {
    if (type == 'v') {
      Vertex id;
      Label l;
      fin >> id >> l;

      label_set.insert(l);
    } else if (type == 'e') {
      Vertex v1, v2;
      Label l;
      fin >> v1 >> v2 >> l;
    }
  }

  fin.close();

  transferred_label.resize(
      *std::max_element(label_set.begin(), label_set.end()) + 1, -1);

  Label new_label = 0;
  for (Label l : label_set) {
    transferred_label[l] = new_label;
    new_label += 1;
  }
}
}  // namespace

// add - new constructor
Checkergraph::Checkergraph(const std::string &filename, bool is_query) {
  if (!is_query) {
    TransferLabel(filename);
  }
  std::vector<std::vector<Vertex>> adj_list;

  // Load Graph
  std::ifstream fin(filename);
  std::set<Label> label_set;

  if (!fin.is_open()) {
    std::cout << "Graph file " << filename << " not found!\n";
    exit(EXIT_FAILURE);
  }

  char type;

  fin >> type >> graph_id_ >> num_vertices_;

  adj_list.resize(num_vertices_);

  // start_offset_.resize(num_vertices_ + 1);
  // label_.resize(num_vertices_);

  num_edges_ = 0;

  // preprocessing
  while (fin >> type) {
    if (type == 'e') {
      Vertex v1, v2;
      Label l;
      fin >> v1 >> v2 >> l;
      // add

      std::pair<Vertex, Vertex> pair = std::make_pair(v1, v2);
      edges_.push_back(pair);

      // done
      adj_list[v1].push_back(v2);
      adj_list[v2].push_back(v1);

      num_edges_ += 1;
    }
  }
  fin.close();
}

// done

Checkergraph::~Checkergraph() {}