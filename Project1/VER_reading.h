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
            while (token == "wire") {
                verFile >> token;
                token.erase(token.size() - 1, 1);
                name_wire.push_back(token);
                verFile >> token;
                continue;
            }

            while (token != "endmodule")
            {
                if (!ReadElement(verFile, token)) {
                    verFile.close();
                    return false;
                }
                verFile >> token;
            }
            continue;
        }
        if (token == "endmodule") {
            break;
        }

    }
    verFile.close();
    return true;
}

bool VERFile::ReadInout(std::ifstream& verFile, std::string& direction)
{
    std::string temp_dir, name, temp;

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
    all_bit_size += p_ino->bit_size;
    return true;
}

int ascii_cod(char x)
{
    int a;
    a = x;
    return a;
}

//исправить!
bool Element::ReadPinVerilog(std::ifstream& verFile, std::string& word) {
    Pin_Verilog* p_pin = new Pin_Verilog;
    pins.push_back(p_pin);

    std::string temp2, token;
    std::string s = "(";                                        //опорные точки для определения имени wire
    std::string f = ")";
    char last_symbol;
    int pos1=0, pos2=0;


    token = word;

    token.erase(0, 1);                                      //delete .
    if (token[0]=='.')
        token.erase(0, 1);
    temp2 = token;

    pos1 = token.find(s, 0);
    p_pin->name_pin = token.erase(pos1, token.size() - pos1);           //обрезаем имя пина

    token = temp2;
    token.erase(0, pos1 + 1);
    pos2 = token.find(f, 0);
    p_pin->name_wire = token.erase(pos2, token.size() - pos2);           //обрезаем wire

    return true;
}

bool VERFile::ReadElement(std::ifstream& verFile, std::string& name)
{
    Element* p_el = new Element(name);
    elements.push_back(p_el);

    std::string token;
    char last_symbol = '0';
    int a = 0;
    std::string target = "[";
    

    verFile >> p_el->name_component_in_def;
    do {
        verFile >> token;
        a = token.find(target, 0);                              //поиск "["
        if (a != -1) {                                          //проверка на то, что данный пин не относится к внешним пинам
            if (token[token.length() - 1] == ';')               //проверка на окончание пина
                return true;
            continue;
        }
       
           if (!p_el->ReadPinVerilog(verFile, token)) {         //Считывание пина
               verFile.close();
               return false;
           }

   } while (token[token.length() - 1] != ';');                  //если встречаем ;, то элемент считан полностью
    return true;
}


