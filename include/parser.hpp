#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <peglib.h>
#include <memory>

#define ENTER_GRAPH(x) {std::cout << "Enter " << ++debug_graph_depth << " " << (x)->name << std::endl;}
#define EXIT_GRAPH(x) {std::cout << "Exit " << debug_graph_depth-- << " " << (x)->name << std::endl;}


extern int debug_graph_depth;
std::unique_ptr<peg::parser> new_parser(); 



#endif