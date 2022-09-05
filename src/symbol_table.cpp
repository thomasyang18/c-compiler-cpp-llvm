#include "symbol_table.hpp"
#include <iostream>

bool SymbolTable::operator [](std::string_view input){
    std::shared_ptr<ll_map> temp = end;
    do{
        if (temp->map.count(input)) return true;
        temp = temp->prev;
    }
    while (temp != nullptr);
    return false;
}

bool SymbolTable::add_def(std::string_view input){
    if (end->map.count(input)) return false;
    end->map[input] = 1;
    return true;
}

void SymbolTable::DecScope(){

 //   std::cerr << "dec " << std::endl;
    end = end->prev;
}


void SymbolTable::IncScope(){
    std::shared_ptr<ll_map> temp(new ll_map());

    temp->prev = end;

    end = temp;
}

SymbolTable::SymbolTable(){
    end.reset(new ll_map());
}

SymbolTable::~SymbolTable(){
    // Presumably, the only scope existing after everything will still be the global scope, but just in case do ll free
    for (std::shared_ptr<ll_map>  temp = end; temp != nullptr;){
        std::shared_ptr<ll_map>  temp2 = temp;
        temp = temp->prev;
    }
}