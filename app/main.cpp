//
// Created by alisher on 1/9/22.
//
#include <iostream>
#include <drogon/drogon.h>

int main(int argc, char** argv){
    if(argc<2){
        std::cerr<< argv[0]<<" <config file name>\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}