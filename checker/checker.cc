/**
 * @file checker.cc
 *
 */

#include "checker.h"

#include "checkergraph.h"
#include "common.h"
#include "graph.h"

Output::Output(const std::string &filename) {
  std::ifstream fin(filename);

  if (!fin.is_open()) {
    std::cout << "Output file " << filename << " not found!\n";
    exit(EXIT_FAILURE);
  }

  char type;
  size_t num_query_vertices;

  fin >> type >> num_query_vertices;
  std::cout << "num_query_vertices " << num_query_vertices << "\n";
  embedding_id_ = 0;
  embeddings_.resize(embedding_id_ + 1);

  while (fin >> type) {
    if (type == 'a') {
      embeddings_[embedding_id_].resize(num_query_vertices);
      // std::cout << "embedding_id_ " << embedding_id_ << "\n";
      for (size_t j = 0; j < num_query_vertices; j++) {
        Vertex embedding_vertex;
        fin >> embedding_vertex;

        embeddings_[embedding_id_][j] = embedding_vertex;
        // std::cout << embedding_id_ << j << " " <<
        // embeddings_[embedding_id_][j]
        //           << "\n";
      }
      embedding_id_ += 1;
    }
    // done
    embeddings_.resize(embedding_id_ + 1);
  }

  fin.close();
  // std::cout << "cs";
}
Output::~Output() {}

void Output::isValidate(const Graph &data, const Checkergraph &query) {
  for (size_t i = 0; i < GetEmbeddingNum(); i++) {
    //각 임베딩마다 체크.
    bool embedding_i_true = true;

    for (size_t k = 0; k < query.GetNumEdges(); k++) {
      Vertex v1 = GetEmbeddingVertex(i, query.GetVertex1InEdge(k));
      Vertex v2 = GetEmbeddingVertex(i, query.GetVertex2InEdge(k));
      bool neighbor = data.IsNeighbor(v1, v2);
      // std::cout << neighbor;
      if (neighbor == false) {
        embedding_i_true = false;
        break;
      }
    }

    if (embedding_i_true == true) {
      std::cout << " Embedding" << i << " is true "
                << "\n";
    } else {
      std::cout << " Embedding" << i << " is false "
                << "\n";
    }
  }

  std::cout << "\n"
            << "checking is done"
            << "\n";
}

int main(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: ./program <data graph file> <query checkergraph file> "
                 "<output file>\n";
    return EXIT_FAILURE;
  }

  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string output_file_name = argv[3];

  Graph data(data_file_name);
  Checkergraph checkerquery(query_file_name, true);
  Output output(output_file_name);

  output.isValidate(data, checkerquery);
  return EXIT_SUCCESS;
}
