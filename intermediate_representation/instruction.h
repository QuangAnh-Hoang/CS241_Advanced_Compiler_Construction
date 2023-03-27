#ifndef __INSTRUCTION_H__
#define __INSTRUCTION_H__

#include "../component/token_table.h"
#include "result.h"

#include <vector>
#include <iostream>

class Instruction {
    public:
        Instruction(int _instId, Opcode _opcode, std::string _dest, std::string _src1, std::string _src2)
            : instId(_instId), opcode(_opcode) {
                operands.push_back(_dest);
                operands.push_back(_src1);
                operands.push_back(_src2);

                std::cout << toString() << std::endl;
            }

        Instruction(int _instId, Opcode _opcode, std::string _dest, std::string _src1, int _immediate)
            : instId(_instId), opcode(_opcode) {
                operands.push_back(_dest);
                operands.push_back(_src1);
                operands.push_back(std::to_string(_immediate));
                std::cout << toString() << std::endl;
            }

        std::string toString() {
            std::string inst = std::to_string(opcode);
            inst.append(":");
            for (auto oper : operands) {
                inst.append(" ");
                inst.append(oper);
            }
            return inst;
        }

    private:
        int instId;
        Opcode opcode;
        std::vector<std::string> operands;
        

};

#endif