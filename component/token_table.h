#ifndef __TOKEN_TABLE_H__
#define __TOKEN_TABLE_H__

#include <string>
#include <vector>

enum TokenType {
    errorToken          = 0,

    timesToken          = 1,
    divToken            = 2,

    plusToken           = 11,
    minusToken          = 12,

    eqlToken            = 20,
    neqToken            = 21,
    lssToken            = 22,
    geqToken            = 23,
    leqToken            = 24,
    gtrToken            = 25,

    periodToken         = 30,
    commaToken          = 31,
    openbracketToken    = 32,
    closebracketToken   = 34,
    closeparenToken     = 35,

    becomesToken        = 40,
    thenToken           = 41,
    doToken             = 42,

    openparenToken      = 50,

    numberToken         = 60,
    identToken          = 61,

    semiToken           = 70,

    endToken            = 80,
    odToken             = 81,
    fiToken             = 82,

    elseToken           = 90,

    letToken            = 100,
    callToken           = 101,
    ifToken             = 102,
    whileToken          = 103,
    returnToken         = 104,

    varToken            = 110,
    arrToken            = 111,
    voidToken           = 112,
    funcToken           = 113,
    procToken           = 114,

    beginToken          = 150,
    mainToken           = 200,
    eofToken            = 255
};

struct token_table
{
    std::vector<std::pair<std::string, TokenType>> token_map = {
        {"then",        thenToken},
        {"do",          doToken},
        {"fi",          fiToken},
        {"else",        elseToken},
        {"let",         letToken},
        {"call",        callToken},
        {"if",          ifToken},
        {"while",       whileToken},
        {"return",      returnToken},
        {"var",         varToken},
        {"array",       arrToken},
        {"void",        voidToken},
        {"function",    funcToken},
        {"procedure",   procToken},
        {"main",        mainToken}
    };
    int reserved_size = token_map.size();
}TokenTable;

enum Opcode {
    // Arithmetic instructions - registers
    ADD     = 0,
    SUB,
    MUL,
    DIV,
    MOD,
    CMP,
    OR      = 8,
    AND,
    BIC,
    XOR,

    LSH,
    ASH,

    CHK,

    BASE_IMMEDIATE = 16,
    // Arithmetic instructions - intermediate
    ADDI    = 16,
    SUBI,
    MULI,
    DIVI,
    MODI,
    CMPI,
    ORI     = 24,
    ANDI,
    BICI,
    XORI,

    LSHI,
    ASHI,

    CHKI,

    // Load/Store instructions
    LDW     = 32,
    LDX,
    POP,

    STW     = 36,
    STX,
    PSH,

    // Control instructions
    BEQ     = 40,
    BNE,
    BLT,
    BGE,
    BLE,
    BGT,

    BSR,
    JSR     = 48,
    RET,

    // Input/Output instructions
    RDD     = 50,
    WRD,
    WRH,
    WRL

};

// Opcode token2opcode(TokenType token) {
//     switch (token) {
//         case plusToken:
//             return ADD;
        
//         default:
//             break;
//     }
// }

#endif