#ifndef __SYMBOL_TABLE_H__
#define __SYMBOL_TABLE_H__

#include "../intermediate_representation/result.h"

#include <unordered_map>
#include <vector>
#include <iostream>

class SymbolTable {
    public:
        SymbolTable() {}

        bool checkEntryExist(int id) {
            return (table.find(id) != table.end());
        }

        Result* lookupEntry(int id, int ssaVal) {
            if (checkEntryExist(id)) {
                if (table.find(id)->second.size() > 0) {
                    if (ssaVal < 0) {
                        return table.find(id)->second.back(); // return last assigned
                    }
                    else if (ssaVal < table.find(id)->second.size()){
                        return table.find(id)->second.at(ssaVal);
                    }
                }
            }
            return nullptr;
        }

        void insertNewAssignment(int id, Result* var) {
            if (checkEntryExist(id)) {
                table.find(id)->second.push_back(var);
            }
            else {
                std::cerr << "Invalid assignment: Identifier's entry not exist" << std::endl;
                exit(-1);
            }
        }

        void insertNewIdentifier(int id) {
            if (checkEntryExist(id)) {
                std::cerr << "Invalid declaration: Identifier's entry already exists" << std::endl;
                exit(-1);
            }
            else {
                std::vector<Result*> newVector; 
                table.insert({id, newVector});
            }
        }


    private:
        std::unordered_map<int, std::vector<Result*>> table;
};

#endif