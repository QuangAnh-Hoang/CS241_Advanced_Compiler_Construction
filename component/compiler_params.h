#ifndef __COMPILER_PARAMS_H__
#define __COMPILER_PARAMS_H__

typedef struct _compiler_params_t {
    char* filename;
    int numRegisters;
    int memorySize;
} compiler_params_t;

#endif