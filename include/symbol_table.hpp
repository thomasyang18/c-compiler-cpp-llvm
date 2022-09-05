#ifndef _SYMBOL_TABLE_HPP_
#define _SYMBOL_TABLE_HPP_

#include <map>
#include <string>
#include <memory>

/**
 * Builds out the symbol table backwards, i.e.
 * [1] <- [2] <- [3]
 */


class SymbolTable {
    typedef struct ll_map {
        // should be string -> type, but this is fine for now
        std::map<std::string_view, int> map;
        std::shared_ptr<struct ll_map> prev;
    } ll_map;

    std::shared_ptr<ll_map> end;

public:
    ~SymbolTable();
    SymbolTable();
    void IncScope();
    void DecScope();
    bool operator [](std::string_view input);
    bool add_def(std::string_view input);

};

#endif