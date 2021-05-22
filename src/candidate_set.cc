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

  cs_.resize(num_query_vertices);
  //추가
  Vertex embedding_id = 0;
  //추가 끝
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
      for (size_t i = 0; i < num_query_vertices; i++) {
        Vertex embedding_vertex;
        fin >> embedding_vertex;
        embs_[embedding_id][i] = embedding_vertex;
      }
      embedding_id++;
    }
    //추가 끝
  }

  fin.close();
}

CandidateSet::~CandidateSet() {}
