#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "VER_class.h"


bool VERFile::Read(std::string filename) {
    std::ifstream verFile(filename);
    if (!verFile.is_open())
        return false;


    std::string line,
        token, trash;

    while (true) {
        // 1. Trim the line to be sure that first symbol is nos a space char
        verFile >> token;
        // 2. Ignore empty and commented lines
        if (token.empty())
            continue;
        if (token == "#") {
            std::getline(verFile, trash);
            continue;
        }
        if (token == ";")
            continue;
        if (token == "input" || token == "output")
        {
            if (!ReadInout(verFile, token)) {
                verFile.close();
                return false;
            }
            continue;
        }
        if (token == "wire")
        {
            verFile >> token;
            name_wire.push_back(token);
            verFile >> trash;
            continue;
        }


        if (token == "endmodule") {
            return true;
            break;
        }
    }
}

bool VERFile::ReadInout(std::ifstream& defFile, std::string)
{

    return true;
}

bool VERFile::ReadElement(std::ifstream& defFile, std::string)
{

    return true;
}
