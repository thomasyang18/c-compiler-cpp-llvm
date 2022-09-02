#ifndef _PARSER_HPP_
#define _PARSER_HPP_

#include <peglib.h>
#include <memory.h>

std::unique_ptr<peg::parser> new_parser(); 

#endif