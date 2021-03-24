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
        if (token == "//") {
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

bool VERFile::ReadInout(std::ifstream& verFile, std::string& direction)
{
    //using namespace std;
    std::string token, trash, temp_dir, name, temp;
    int temp_size;

    temp_dir = direction;
    verFile >> temp;
    temp.erase(0, 1);
    temp.erase(2, 3);
    verFile >> name;
    name.erase(1, 1);

    Inout* p_ino = new Inout;
    inouts.push_back(p_ino);

    p_ino->name_inout=name;
    if (direction == "input") {
        p_ino->direction = PinDirectionVerilog::input;
    }
    if (direction == "output") {
        p_ino->direction = PinDirectionVerilog::output;
    }
    if (direction == "inout") {
        p_ino->direction = PinDirectionVerilog::inout;
    }
    p_ino->bit_size = stod(temp)+1;
    return true;
}

bool VERFile::ReadElement(std::ifstream& defFile, std::string)
{

    return true;
}

