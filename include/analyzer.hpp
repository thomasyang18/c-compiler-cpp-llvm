#ifndef _ANALYZER_HPP_
#define _ANALYZER_HPP_

#include <peglib.h>

/* Supports single file "Definitions must be declared before use" */

// TODO: Maybe refactor to use our own AST nodes. Probably makes analysis easier, and we can swap out different frontends


enum defOrderMode{
    look_for_uses,
    look_for_defs
};


void definition_ordering(std::shared_ptr<peg::Ast> ast, defOrderMode=look_for_uses);
void reset_driver();

#endif