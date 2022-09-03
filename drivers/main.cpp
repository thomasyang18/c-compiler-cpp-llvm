#include <fstream>
#include <stdexcept>
#include "parser.hpp"
#include "analyzer.hpp"

static std::string get_file_as_string(std::string file){
  std::ifstream t(file);
  std::cout << " Yo " << file << '\n';
  assert (t.is_open());
  std::stringstream buffer;
  buffer << t.rdbuf();
  return buffer.str();
}

int main(int argc, char **argv) {
  
  //if (argc != 2) throw std::invalid_argument(" Need exactly one file argument ");
  
  std::unique_ptr<peg::parser> parser = std::move(new_parser());

  parser->enable_ast();
  
  std::shared_ptr<peg::Ast> ast;
  if(parser->parse("int main(){int y, y;}", ast)){
    std::cout << "before " << std::endl;
    ast = parser->optimize_ast(ast);
    std::cout << "after " << std::endl;

    definition_ordering(ast);
  }
}