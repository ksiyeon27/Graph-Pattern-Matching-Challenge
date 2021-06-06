/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

#include <cstdlib>

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // implement your code here.

  Dag dag(query, cs);
  // (index, value) pair at extendable_cs of mapped image of query vertices
  std::vector<std::pair<size_t, Vertex>> mapping(query.GetNumVertices(),
                                                 {0, -1});
  // data_visited[v] is true iff vertex v of data graph is marked as visited
  std::vector<bool> data_visited(data.GetNumVertices(), false);

  // set of extendable candidates
  std::vector<std::vector<Vertex>> extendable_cs(query.GetNumVertices());
  for (Vertex u = 0; u < static_cast<Vertex>(query.GetNumVertices()); ++u) {
    extendable_cs[u].reserve(cs.GetCandidateSize(u));
  }

  // list of mapped query vertices, in the order of visited time
  std::vector<Vertex> search_stack;

  // candidate set size(|C_M(u)|)에 대해 내림차순으로 정렬
  auto candidate_size_order = [&extendable_cs](Vertex u, Vertex v) {
    return extendable_cs[u].size() > extendable_cs[v].size();
  };
  std::vector<Vertex> queue;

  size_t max_matches = 100000;  // maximum number of matches
  size_t matches = 0;           // current number of matches

  // current vertex
  Vertex u = dag.GetRoot();  // root부터 시작
  for (size_t i = 0; i < cs.GetCandidateSize(u); ++i) {
    extendable_cs[u].push_back(cs.GetCandidate(u, i));
  }
  search_stack.push_back(u);

  // execute branch-and-bound
  while (true) {
    GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("current mapping: %s",
                                         vector_to_string(mapping).c_str());
    GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
        "currently visited query vertices: %s",
        vector_to_string(search_stack).c_str());
    GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("current queue: %s",
                                         vector_to_string(queue).c_str());
#ifndef NDEBUG
    // GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
    //     "current visit status: %s", vector_to_string(data_visited).c_str());
    std::vector<Vertex> temp;
    for (size_t i = 0; i < data_visited.size(); ++i) {
      if (data_visited[i]) {
        temp.push_back(i);
      }
    }
    GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("current visit status: %s",
                                         vector_to_string(temp).c_str());
#endif

    if (search_stack.size() == query.GetNumVertices()) {
      // fast-forward to end of extendable candidates
      size_t i;
      for (i = mapping[u].first; i < extendable_cs[u].size(); ++i) {
        if (data_visited[extendable_cs[u][i]]) {
          continue;
        }
        mapping[u].second = extendable_cs[u][i];
        GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("map %d -> %d", u,
                                             mapping[u].second);
#ifndef NDEBUG
        std::vector<Vertex> temp_mapping;
        for (const auto &p : mapping) {
          temp_mapping.push_back(p.second);
        }
        GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
            "found embedding (%s)", vector_to_string(temp_mapping).c_str());
#endif
        // # mapping 1개 완료 -> 출력
        std::cout << "a ";
        for (const auto &v : mapping) {
          std::cout << v.second << ' ';
        }
        std::cout << '\n';
        if (++matches >= max_matches) {
          exit(EXIT_SUCCESS);
        }
      }
      mapping[u].first = i;
    }

    if (mapping[u].first != 0) {
      // bound as much as possible
      while (true) {
        // erase children of u out of queue
        // first, mark all children by clearing their candidate sets
        for (size_t i = dag.GetChildStartOffset(u);
             i < dag.GetChildEndOffset(u); ++i) {
          extendable_cs[dag.GetChild(i)].clear();
        }
        // next, move all marked elements to the back of the queue,
        // effectively removing them
        size_t back = queue.size();
        for (auto it = queue.rbegin(); it != queue.rend(); ++it) {
          if (extendable_cs[*it].empty()) {
            std::swap(queue[--back], *it);
          }
        }
        // finally, rebuild the queue
        if (back != queue.size()) {
          queue.resize(back);
          std::make_heap(queue.begin(), queue.end(), candidate_size_order);
        }

        if (mapping[u].first != extendable_cs[u].size()) {
          // if any of the extendable candidates are not visited, do not bound
          break;
        }

        // un-visit u
        if (mapping[u].second >= 0) {
          data_visited[mapping[u].second] = false;
        }
        mapping[u].first = 0;
        mapping[u].second = -1;
        queue.push_back(u);

        // find previous vertex in the stack
        search_stack.pop_back();
        GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
            "backtrack from query vertex u = %d to %s", u,
            search_stack.empty() ? "(end)"
                                 : std::to_string(search_stack.back()).c_str());
        if (search_stack.empty()) {
          // end of search
          GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("End printing");
          return;
        }
        u = search_stack.back();
        ++mapping[u].first;
      }
      data_visited[mapping[u].second] = false;
    }

    // branch
    // find next mapping and extendable children
    std::vector<Vertex> extendable_children;
    bool can_branch;
    do {
      GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
          "mapping[u].first == %lu, extendable_cs[u].size() == %lu",
          mapping[u].first, extendable_cs[u].size());
      Vertex v =
          extendable_cs[u][mapping[u].first];  // mapping of current vertex
      if (data_visited[v]) {
        // v is already visited; find next embedding
        can_branch = false;
        ++mapping[u].first;
        continue;
      }
      mapping[u].second = v;
      GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("map %d -> %d", u,
                                           mapping[u].second);
      data_visited[v] = true;

      can_branch = true;
      for (size_t i = dag.GetChildStartOffset(u); i < dag.GetChildEndOffset(u);
           ++i) {
        Vertex child = dag.GetChild(i);
        GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("query node %d has child %d", u,
                                             child);

        // find embeddings of all parent nodes of child
        std::vector<Vertex> mapped_parents;
        bool is_extendable_vertex = true;
        for (size_t j = dag.GetParentStartOffset(child);
             j < dag.GetParentEndOffset(child); ++j) {
          // v = M(p_i) (Definition 5.2)
          Vertex v = mapping[dag.GetParent(j)].second;
          // if mapping exists
          if (v >= 0) {
            mapped_parents.push_back(v);
          } else {
            // u is not extendable
            GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
                "node %d cannot be added because parent node %d is not "
                "embedded",
                child, dag.GetParent(j));
            is_extendable_vertex = false;
            break;
          }
        }
        if (!is_extendable_vertex) {
          continue;
        }

        // find extendable candidates of child
        for (size_t j = 0; j < cs.GetCandidateSize(child); ++j) {
          Vertex candidate = cs.GetCandidate(child, j);
          bool is_extendable_candidate = true;
          for (const Vertex v : mapped_parents) {
            if (!data.IsNeighbor(v, candidate)) {
              is_extendable_candidate = false;
              break;
            }
          }
          if (is_extendable_candidate && !data_visited[candidate]) {
            extendable_cs[child].push_back(candidate);
          }
        }

        if (extendable_cs[child].empty()) {
          // fprintf(stderr, "%s\n", vector_to_string(mapping).c_str());
          GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
              "mapping %d -> %d is impossible because child node %d has no "
              "candidates",
              u, v, child);
          for (size_t j = dag.GetChildStartOffset(u); j < i; ++j) {
            extendable_cs[dag.GetChild(j)].clear();
          }
          extendable_children.clear();
          can_branch = false;
          break;
        } else {
          GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
              "extendable candidates of %d: %s", child,
              vector_to_string(extendable_cs[child]).c_str());
          extendable_children.push_back(child);
        }
      }

      if (!can_branch) {
        data_visited[v] = false;
        ++mapping[u].first;
      }
    } while (!can_branch && mapping[u].first != extendable_cs[u].size());

    if (mapping[u].first == extendable_cs[u].size()) {
      // out of candidates; bound
      continue;
    }

    // push extendable children of u to queue
    for (Vertex child : extendable_children) {
      queue.push_back(child);
      std::push_heap(queue.begin(), queue.end(), candidate_size_order);
    }

    // find next extendable vertex
    u = queue.front();
    std::pop_heap(queue.begin(), queue.end(), candidate_size_order);
    queue.pop_back();
    GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
        "mapping extended to %d (candidates: %s)", u,
        vector_to_string(extendable_cs[u]).c_str());
    search_stack.push_back(u);
  }
}
