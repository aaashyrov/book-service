//
// Created by alisher on 1/9/22.
//
#include <Controller.hpp>
#include <drogon/drogon.h>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << argv[0] << "<config file name>\n";
        return EXIT_FAILURE;
    }

    drogon::app().loadConfigFile(argv[1]);
    drogon::app().run();

    return EXIT_SUCCESS;
}