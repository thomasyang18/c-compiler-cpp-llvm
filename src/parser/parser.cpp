#include "parser.hpp"
#include <fstream>
#include <stdexcept>

/*
    TODO: Refactor into a parser-gen type of code, so I don't have to depend on text files and relative path stuff that'll be annoying
    
    Never mind, apparently doing relative paths just works if I keep it in the bin folder for now...
*/

std::string get_file_as_string(std::string file){
  std::ifstream t(file);
  assert (t.is_open());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

std::unique_ptr<peg::parser> new_parser(){
    std::unique_ptr<peg::parser> ret(new peg::parser(get_file_as_string("resources/c_subset.peg")));
    assert(static_cast<bool>(*ret) == true);
    return std::move(ret);
}