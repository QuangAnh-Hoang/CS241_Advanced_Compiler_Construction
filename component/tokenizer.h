#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__

#include <vector>

#include "file_reader.h"
#include "token_table.h"
#include "symbol_table.h"

class Tokenizer {
    public:
        Tokenizer(compiler_params_t *_params) {
            file_reader = new FileReader(_params);
            next();
        }

        void Error(std::string errorMsg) {
            file_reader->Error(errorMsg);
        }

        TokenType getNext() {
            resetString();
            if (currentChar == EOF) { return eofToken; }
            while (currentChar == 0) { next(); continue; }
            while (isspace(currentChar)) { next(); continue; }

            if (isalpha(currentChar)) {
                while(isalnum(currentChar)) {
                    currentString.push_back(currentChar);
                    next();
                }
                return generateIdentifierToken();
            }
            else if (isdigit(currentChar)) {
                while(isdigit(currentChar)) {
                    currentString.push_back(currentChar);
                    next();
                }
                return generateNumberToken();
            }
            else {
                return generateSymbolToken();
            }
        }

        // SymbolTable* symbolTable;

        std::string varLookup(int id) {
            for (auto entry : id_table) {
                if (entry.second == id) {
                    return entry.first;
                }
            }
            return nullptr;
        }

        std::string currentString;
        int number;
        int id;

    private:
        void next()         { currentChar = file_reader->getNext(); }
        void resetString()  { currentString = ""; }

        TokenType generateIdentifierToken() {
            auto token_it = TokenTable.token_map.begin();
            while (token_it != TokenTable.token_map.end()) {
                if (token_it->first == currentString) {
                    return token_it->second;
                }
                token_it++;
            }

            // int ident_index = symbolTable->checkIdentifierDeclared(currentString);
            // if (ident_index < 0) {
            //     ident_index = symbolTable->insertNewIdentifier(currentString);
            // }
            // id = ident_index;
            // return identToken;

            auto ident_it = id_table.begin();
            int ident_index = 0;
            while (ident_it != id_table.end()) {
                if (ident_it->first == currentString) {
                    id = ident_index;
                    return identToken;
                }
                ident_it++;
                ident_index++;
            }
            id = id_table.size();
            id_table.push_back(std::make_pair(currentString, id));
            return identToken;
        }

        TokenType generateNumberToken() {
            number = stoi(currentString);
            return numberToken;
        }

        TokenType generateSymbolToken() {
            switch (currentChar) {
                case '*':
                    next(); 
                    return timesToken;
                case '/':
                    next(); 
                    return divToken;
                case '+':
                    next(); 
                    return plusToken;
                case '-':
                    next(); 
                    return minusToken;

                case '=':
                    next();
                    if (currentChar == '=')         { next(); return eqlToken; }
                case '!':
                    next();
                    if (currentChar == '=')         { next(); return neqToken; }
                case '<':
                    next();
                    if (isspace(currentChar))       { next(); return lssToken; }
                    else if (currentChar == '=')    { next(); return leqToken; }
                    else if (currentChar == '-')    { next(); return becomesToken; }

                case '>':
                    next();
                    if (isspace(currentChar))       { next(); return gtrToken; }
                    else if (currentChar == '=')    { next(); return geqToken; }

                case '.':
                    next();
                    return periodToken;
                case ',':
                    next();
                    return commaToken;
                case '[':
                    next();
                    return openbracketToken;
                case ']':
                    next();
                    return closebracketToken;
                case ')':
                    next();
                    return closeparenToken;

                case '(':
                    next();
                    return openparenToken;

                case ';':
                    next();
                    return semiToken;

                case '}':
                    next();
                    return endToken;

                case '{':
                    next();
                    return beginToken;

                default:
                    file_reader->Error("SyntaxError: Invalid symbol");
                    return errorToken;
            }
        }


        std::vector<std::pair<std::string, int>> id_table;
        char        currentChar;
        FileReader* file_reader;


};


#endif