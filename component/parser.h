#ifndef __PARSER_H__
#define __PARSER_H__

#include "../intermediate_representation/instruction.h"
#include "../intermediate_representation/result.h"

#include "tokenizer.h"
#include "symbol_table.h"

#include <unordered_map>

class Parser {
    public:
        Parser(compiler_params_t *_params) {
            pc = 0;

            symbolTable = new SymbolTable();

            tokenizer = new Tokenizer(_params);

            for (int i = 0; i < _params->numRegisters; i++) {
                regFile.push_back(false);
            }
            setAllocateReg(0);

            next();
            parseComputation();
            printInstructions();
            // codegen->printAllInstructions();
        };

        SymbolTable* symbolTable;

    private:
        void printInstructions() {
            for (auto inst : instructionBuffer) {
                std::cout << inst->toString() << std::endl;
            }
        }

        void printTokens() {
            checkFor(mainToken);
            while (inputSym != eofToken)
            {
                std::cout << inputSym << std::endl;
                next();
            }   
        }

        void parseComputation() {
            checkFor(mainToken);
            while (inputSym == varToken || inputSym == arrToken) {
                processVarDecl();
            }
            while (inputSym == funcToken) {
                next();
                processFuncDecl();
            }
            if (inputSym == beginToken) {
                next();
                processStatSeq();
                checkFor(endToken);
            }
            checkFor(periodToken);
        }

        void processVarDecl() {
            processTypeDecl();
            if (inputSym == identToken) {
                symbolTable->insertNewIdentifier(tokenizer->id);
                next();
            }
            while (inputSym != semiToken) {
                if (inputSym == commaToken) {
                    next();
                    checkFor(identToken);
                    symbolTable->insertNewIdentifier(tokenizer->id);
                }
            }
            checkFor(semiToken);
        }

        void processFuncDecl() {}

        void processStatSeq() {
            processStatement();
            while (inputSym == semiToken) {
                next();
                if (inputSym == endToken) {
                    return;
                }
                processStatement();
            }
        }

        void processStatement() {
            switch (inputSym) {
                case letToken:
                    processAssign();
                    break;

                case callToken:
                    processFuncCall();
                    break;

                case ifToken:
                    processIf();
                    break;

                case whileToken:
                    processWhile();
                    break;

                case returnToken:
                    processReturn();
                    break;
                
                default:
                    syntaxError("Invalid token: no statement token detected.");
                    break;
            }
        }

        void processTypeDecl() {
            if (inputSym == varToken) {
                next();
            }
            // TODO: Add array type
        }

        void processAssign() {
            checkFor(letToken);
            checkFor(identToken);
            Result* last_x = symbolTable->lookupEntry(tokenizer->id, -1);

            Result* x;

            if (last_x == nullptr) {
                x = new Result(varType, tokenizer->varLookup(tokenizer->id), pc);
            }
            else {
                x = new Result(last_x);
            }

            symbolTable->insertNewAssignment(tokenizer->id, x);

            if (x->kind == varType) {
                load(x);
            }

            checkFor(becomesToken);

            Result* y = expr();

            instructionBuffer.push_back(new Instruction(pc++, ADDI, x->toString(), y->toString(), 0));
        }

        void processFuncCall() {}

        void processIf() {}

        void processWhile() {}

        void processReturn() {}

        void compute(Opcode opcode, Result* x, Result* y) {
            if (x->kind == constType && y->kind == constType) {
                switch (opcode) {
                    case ADD:
                        x->value = x->value + y->value;
                        break;
                    case SUB:
                        x->value = x->value - y->value;
                        break;
                    case MUL:
                        x->value = x->value * y->value;
                        break;
                    case DIV:
                        x->value = x->value / y->value;
                        break;
                }
            }
            else {
                load(x);
                if (x->regno == 0) {
                    x->regno = allocateReg();
                    instructionBuffer.push_back(
                        new Instruction(
                            pc++, ADD, x->toString(), "(R0)", "(R0)"
                        )
                    );
                }
                if (y->kind == constType) {
                    instructionBuffer.push_back(
                        new Instruction(
                            pc++, (Opcode)(opcode + BASE_IMMEDIATE),
                            x->toString(), x->toString(), y->toString()
                        )
                    );
                }
                else {
                    load(y);
                    instructionBuffer.push_back(
                        new Instruction(
                            pc++, opcode,
                            x->toString(), x->toString(), y->toString()
                        )
                    );
                    deallocateReg(y);
                }
            }
        }

        void load(Result* x) {
            if (x->kind == varType) {
                int newReg = allocateReg();
                instructionBuffer.push_back(
                    new Instruction(
                        pc++, LDW,
                        "(R" + std::to_string(newReg) + ")", "0", x->toString()
                    )
                );
                x->regno = newReg;
                x->kind = regType;
            }
            else if (x->kind == constType)
            {
                if (x->value == 0) {
                    x->regno = 0;
                }
                else {
                    int newReg = allocateReg();
                    instructionBuffer.push_back(
                        new Instruction(
                            pc++, ADDI,
                            "(R" + std::to_string(newReg) + ")", "0", x->toString()
                        )
                    );
                    x->regno = newReg;
                }
                x->kind = regType;
            }
        }

        Result* expr() {
            Result* x = new Result();
            x = term();
            while (inputSym == plusToken || inputSym == minusToken) {
                Opcode opcode = (inputSym == plusToken) ? ADD : SUB; 
                Result* y = new Result();
                next();
                y = term();
                compute(opcode, x, y);
            }
            return x;
        }

        Result* term() {
            Result* x = new Result();
            x = fact();
            while (inputSym == timesToken || inputSym == divToken) {
                Opcode opcode = (inputSym == timesToken) ? MUL : DIV;
                Result* y = new Result();
                next();
                y = fact();
                compute(opcode, x, y);
            }
            return x;
        }        

        Result* fact() {
            Result* x = new Result();
            if (inputSym == openparenToken) {
                next();
                x = expr();
                checkFor(closeparenToken);
            }
            else if (inputSym == numberToken)
            {
                x->kind = constType;
                x->value = tokenizer->number;
                next();

            }
            else if (inputSym == identToken)
            {
                x = symbolTable->lookupEntry(tokenizer->id, -1); // get the last declared
                next();
            }
            else {
                syntaxError("Invalid syntax: parsing factor");
            }
            return x;
        }

        // Register file's methods
        int allocateReg() {
            for (int i = 1; i < regFile.size(); i++) {
                if (!regFile[i]) {
                    setAllocateReg(i);
                    return i;
                }
            }
            return -1;  // cannot allocate register
        }

        void deallocateReg(Result* result) {
            regFile[result->regno] = false;
        }

        void setAllocateReg(int regId) {
            regFile[regId] = true;
        }

        void checkFor(TokenType token) {
            if (inputSym == token) {
                next();
            }
            else {
                std::string errorMsg;
                std::sprintf(&errorMsg[0], "Invalid token: expected %d, received %d", token, inputSym);
                syntaxError(errorMsg);
            }
        }

        void syntaxError(std::string errorMsg) {
            tokenizer->Error(errorMsg);
        }

        void next() { inputSym = tokenizer->getNext(); }

        Tokenizer* tokenizer;

        int inputSym;
        int pc;

        std::vector<Instruction*> instructionBuffer;
        std::vector<bool> regFile;

};



#endif