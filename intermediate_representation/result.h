#ifndef __RESULT_H__
#define __RESULT_H__

enum ResultType {
    constType,
    varType,
    regType
};

class Result {
    public:
        Result() {
            value = 0;
            addr = 0;
            regno = 0;
        }

        Result(Result* _x) 
            : varName(_x->varName), kind(_x->kind), 
              value(_x->value), addr(_x->addr), regno(_x->regno) 
        {
            ssa = _x->ssa + 1;
        }

        Result(ResultType _init_type, std::string _varName, int pc) {
            varName = _varName;
            ssa = pc;

            kind = _init_type;
            value = 0;
            addr = 0;
            regno = 0;
        }

        std::string toString() {
            if (kind == constType) {
                return std::to_string(value);
            }
            else if (kind == regType) {
                return "(R" + std::to_string(regno) + ")";
            }
            else {
                return varName + "_" + std::to_string(ssa);
            }
        }

        std::string varName;
        int ssa;

        ResultType kind;   // const, var, reg
        int value;  // if const
        int addr;   // if var
        int regno;  // if reg
};


#endif