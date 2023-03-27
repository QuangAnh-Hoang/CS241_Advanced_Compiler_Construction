#ifndef __FILE_READER_H__
#define __FILE_READER_H__

#include <iostream>

#include "compiler_params.h"

class FileReader {
    public:
        FileReader(compiler_params_t *_params) {
            filename = _params->filename;
            if ((filename == NULL) || (filename[0] == '\0')) {
                input_stream = &std::cin;
            }
            else {
                input_file.open(filename);
                if (input_file.is_open()) {
                    input_stream = &input_file;
                }
                else {
                    std::cout << "invalid filename" << std::endl;
                    exit(-1);
                }
            }
        }

        ~FileReader() {
            if (input_file.is_open()) {
                input_file.close();
            }
        }

        void Error(std::string errorMsg) {
            std::cerr << errorMsg << std::endl;
            exit(-1);
        }

        bool eof() {
            return (*input_stream).eof();
        }

        char getNext() {
            if (!(*input_stream).eof()) {
                (*input_stream).get(current);
                return current;
            }
            return EOF;
        }

    private:
        std::istream* input_stream;
        std::ifstream input_file;
        char* filename;
        char current;
};

#endif