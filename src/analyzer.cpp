#include "analyzer.hpp"
#include "parser.hpp"
#include <stdexcept>
#include <set>

std::set<std::string_view> declared;

void reset_driver(){
    declared.clear();
    // bad hackaround until i can start declaring functions
    declared.insert("main");
}

/**
 *  No duplicate declarations
 *  No use before declaration
 */

void definition_ordering(std::shared_ptr<peg::Ast> ast, defOrderMode mode){
   // ENTER_GRAPH(ast)

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
                if (!declared.count(ast->token)) {
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
            if (declared.count(ast->token)) {
            std::string error = "Ident ";
            error += ast->token;
            error += " already exists";
            throw std::invalid_argument(error);
            }
            declared.insert(ast->token);
        }    
    }


    for (auto &x: ast->nodes) {
        definition_ordering(x, mode);
    }   


//    EXIT_GRAPH(ast)
}