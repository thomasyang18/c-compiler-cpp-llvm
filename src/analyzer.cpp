#include "analyzer.hpp"
#include "parser.hpp"
#include "symbol_table.hpp"
#include <stdexcept>
#include <set>

std::unique_ptr<SymbolTable> sym;

void reset_driver(){
    sym.reset(new SymbolTable());
}

/**
 *  No duplicate declarations
 *  No use before declaration
 */

void definition_ordering(std::shared_ptr<peg::Ast> ast, defOrderMode mode){
 //   ENTER_GRAPH(ast)

   if (ast->name == "Block") sym->IncScope();

    if (mode == look_for_uses){

        if (ast->name == "Dec"){
            for (auto &x: ast->nodes){
                if (x->name == "type"); // for now, since we only support ints, don't care
                else definition_ordering(x, look_for_defs); 
            }
            return;
        }
        else {
            if (ast->name == "Ident"){
                if (!(*sym)[ast->token]) {
                std::string error = "Ident ";
                error += ast->token;
                error += " doesn't exist yet";
                throw std::invalid_argument(error);
                }
            }
        }
    }
    else if (mode == look_for_defs){
        if (ast->name == "Ident"){
            if (!sym->add_def(ast->token)) {
            std::string error = "Ident ";
            error += ast->token;
            error += " already exists";
            throw std::invalid_argument(error);
            }
            
        }    
    }


    for (auto &x: ast->nodes) {
        definition_ordering(x, mode);
    }   


   if (ast->name == "Block") sym->DecScope();


 //   EXIT_GRAPH(ast)
}