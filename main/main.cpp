#include <fstream>

#include "../component/parser.h"
#include "../component/compiler_params.h"

void print_usage() {}

int main(int argc, char* argv[]) {
    compiler_params_t params;
    params.filename = argv[1];
    params.numRegisters = 32;
    params.memorySize = 4096;
    
    Parser parser(&params);
}