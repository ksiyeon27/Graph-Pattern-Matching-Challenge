/**
 * @file checker.cc
 *
 */

#include "common.h"
#include "graph.h"

void checker(int argc, char *argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: ./program <data graph file> <query graph file> "
                 "<output file>\n";
  }

  std::string data_file_name = argv[1];
  std::string query_file_name = argv[2];
  std::string output_file_name = argv[3];

  Graph data(data_file_name);
  Graph query(query_file_name, true);

  size_t vnum;
  vnum = query.GetNumVertices();
  std::cout << vnum;
}
