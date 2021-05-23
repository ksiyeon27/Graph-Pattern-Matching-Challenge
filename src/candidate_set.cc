/**
 * @file candidate_set.cc
 *
 */

#include "candidate_set.h"

CandidateSet::CandidateSet(const std::string& filename) {
  std::ifstream fin(filename);

  if (!fin.is_open()) {
    std::cout << "Candidate set file " << filename << " not found!\n";
    exit(EXIT_FAILURE);
  }

  char type;
  size_t num_query_vertices;

  fin >> type >> num_query_vertices;

  //추가
  embedding_id_ = 0;
  //추가 끝

  cs_.resize(num_query_vertices);
  embeddings_.resize(embedding_id_ + 1);

  while (fin >> type) {
    if (type == 'c') {
      Vertex id;
      size_t candidate_set_size;

      fin >> id >> candidate_set_size;

      cs_[id].resize(candidate_set_size);

      for (size_t i = 0; i < candidate_set_size; ++i) {
        Vertex data_vertex;
        fin >> data_vertex;
        cs_[id][i] = data_vertex;
      }
    }

    //추가
    else if (type == 'a') {
      embeddings_[embedding_id_].resize(num_query_vertices);

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
    //추가 끝
    embeddings_.resize(embedding_id_ + 1);
  }

  fin.close();
  // std::cout << "cs";
}

CandidateSet::~CandidateSet() {}
