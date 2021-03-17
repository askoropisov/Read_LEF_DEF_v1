#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "DEF_class.h"



//Read DEF file

bool DEFFile::Read(std::string filename) {
    std::ifstream defFile(filename);
    if (!defFile.is_open())
        return false;

    std::string line,
        token, trash;
    while (true) {
        // 1. Trim the line to be sure that first symbol is nos a space char
        defFile >> token;
        // 2. Ignore empty and commented lines
        if (token.empty())
            continue;
        if (token == "#") {
            std::getline(defFile, trash);
            continue;
        }
        if (token == ";")
            continue;
        // 3. Ignore unimportans statements
        if (token == "DIVIDERCHAR") {
            std::getline(defFile, trash);
            continue;
        }
        if (token == "BUSBITCHARS") {
            std::getline(defFile, trash);
            continue;
        }
        // 4. Read importans statements
        if (token == "VERSION") {
            defFile >> version;
            continue;
        }
        if (token == "DESIGN") {
            defFile >> desing_name;
            continue;
        }
        if (token == "UNITS") {
            defFile >> trash;
            defFile >> trash;
            defFile >> microns;
            continue;
        }
        if (token == "DIEAREA") {
            if (!ReadDieArea(defFile)) {
                defFile.close();
                return false;
            }
           // std::cout << std::endl << "DIEAREA successfully read";
            continue;
        }
        if (token == "ROW") {
            defFile >> token;
            if (!ReadRow(defFile, token)) {
                defFile.close();
                return false;
            }
            continue;
        }

        if (token == "TRACKS") {
            continue;
        }
        if (token == "COMPONENTS") {
            defFile >> count_components;
            defFile >> trash;
            while (true) {
                defFile >> token;
                if (token == "END") {
                    defFile >> trash;
                   // std::cout << std::endl << "COMPONENTS successfully read";
                    break;
                }
                if (token == "-") {
                    defFile >> token;
                    if (!ReadComponents(defFile, token)) {
                        defFile.close();
                        return false;
                    }
                }
            }
            continue;
        }
        if (token == "PINS") {
            defFile >> count_pins;
            defFile >> trash;
            while (true) {
                defFile >> token;
                if (token == "END") {
                    defFile >> trash;
                    //std::cout << std::endl << "PINS successfully read";
                    break;
                }
                if (token == "-") {
                    defFile >> token;
                    if (!ReadPins(defFile, token)) {
                        defFile.close();
                        return false;
                    }
                }
            }
            continue;
        }
        if (token == "SPECIALNETS") {
            defFile >> count_special_nets;
            if (!ReadSpecialNets(defFile)) {
                defFile.close();
                return false;
            }
            continue;
        }
        if (token == "NETS") {
            defFile >> count_nets;
            if (!ReadNets(defFile)) {
                defFile.close();
                return false;
            }
            continue;
        }
        if (token == "END")
        {
            defFile >> token;
            if (token == "DESIGN")
            {
                break;
            }
        }
    }

    defFile.close();
    fileName = filename;
    return true;
}


//functions read DEF section

bool DEFFile::ReadRow(std::ifstream& defFile, std::string& name) {              //read Rows
    Row* p_row = new Row(name);
    rows.push_back(p_row);

    std::string token, trash;
    defFile >> trash;                           //core
    defFile >> p_row->x_position;               //position first element in row
    defFile >> p_row->y_position;
    defFile >> token;                           //orintation
    if (token == "N") {
        p_row->orintation = Orintation::N;
    }
    if (token == "S") {
        p_row->orintation = Orintation::S;
    }
    if (token == "E") {
        p_row->orintation = Orintation::E;
    }
    if (token == "W") {
        p_row->orintation = Orintation::W;
    }
    if (token == "FN") {
        p_row->orintation = Orintation::FN;
    }
    if (token == "FS") {
        p_row->orintation = Orintation::FS;
    }
    if (token == "FE") {
        p_row->orintation = Orintation::FE;
    }
    if (token == "FW") {
        p_row->orintation = Orintation::FW;
    }
    defFile >> trash;                            //DO
    defFile >> p_row->count_horizontal;
    defFile >> trash;                            //BY
    defFile >> p_row->count_vertical;
    defFile >> token;
    if (token == "STEP") {                       //dont use in this version
        defFile >> p_row->step_horizontal;
        defFile >> p_row->step_vertical;
    }
    return true;
}

bool DEFFile::ReadComponents(std::ifstream& defFile, std::string& name) {               //read components
    Component* p_com = new Component(name);
    components.push_back(p_com);

    std::string token, trash;
    defFile >> p_com->name_model_in_LEF;                                                //name model in LEF file
    name_Component.push_back(p_com->name_model_in_LEF);
    defFile >> trash >> trash >> trash;
    defFile >> p_com->x_position;                                                       //location component
    defFile >> p_com->y_position;
    defFile >> trash;
    defFile >> token;                                                                   //orintation
    if (token == "N") {
        p_com->orintation = Orintation::N;
    }
    if (token == "S") {
        p_com->orintation = Orintation::S;
    }
    if (token == "E") {
        p_com->orintation = Orintation::E;
    }
    if (token == "W") {
        p_com->orintation = Orintation::W;
    }
    if (token == "FN") {
        p_com->orintation = Orintation::FN;
    }
    if (token == "FS") {
        p_com->orintation = Orintation::FS;
    }
    if (token == "FE") {
        p_com->orintation = Orintation::FE;
    }
    if (token == "FW") {
        p_com->orintation = Orintation::FW;
    }
    defFile >> trash;
    return true;
}

bool DEFFile::ReadPins(std::ifstream& defFile, std::string& name) {                     //read Pins
    PinDef* p_pin = new PinDef(name);                                                   //name Pin
    pins.push_back(p_pin);

    std::string token, trash;
    defFile >> trash >> trash;
    defFile >> p_pin->name_connected_wire;                                              //name connected wire
    defFile >> trash >> trash;
    defFile >> token;                                                                   //direction pin
    if (token == "INPUT") {
        p_pin->direction = PinDirection::output;
    }
    if (token == "OUTPUT") {
        p_pin->direction = PinDirection::input;
    }
    if (token == "TRISTATE") {
        p_pin->direction = PinDirection::tristate;
    }
    if (token == "INOUT") {
        p_pin->direction = PinDirection::inout;
    }
    if (token == "FEEDTHRU") {
        p_pin->direction = PinDirection::feedthru;
    }
    defFile >> trash >> trash >> trash;
    defFile >> p_pin->x_position;                                                       //location pin
    defFile >> p_pin->y_position;
    defFile >> trash;
    defFile >> token;                                                                   //orintation
    if (token == "N") {
        p_pin->orintation = Orintation::N;
    }
    if (token == "S") {
        p_pin->orintation = Orintation::S;
    }
    if (token == "E") {
        p_pin->orintation = Orintation::E;
    }
    if (token == "W") {
        p_pin->orintation = Orintation::W;
    }
    if (token == "FN") {
        p_pin->orintation = Orintation::FN;
    }
    if (token == "FS") {
        p_pin->orintation = Orintation::FS;
    }
    if (token == "FE") {
        p_pin->orintation = Orintation::FE;
    }
    if (token == "FW") {
        p_pin->orintation = Orintation::FW;
    }
    std::getline(defFile, trash);
    return true;
}

bool DEFFile::ReadSpecialNets(std::ifstream& defFile) {     //dont need now
    return true;
}

bool DEFFile::ReadNets(std::ifstream& defFile) {            //dont need now
    return true;
}

bool DieArea::ReadPoint(std::ifstream& defFile) {           //read position point for DieArea
    Point* p_point = new Point;
    points.push_back(p_point);
    std::string token;

    while (token != ")") {
        defFile >> p_point->x_position;
        defFile >> p_point->y_position;
        defFile >> token;
    }
    return true;
}


bool DEFFile::ReadDieArea(std::ifstream& defFile) {         //read Size and form cristall
    DieArea* p_da = new DieArea;
    std::string token;

    do {
        defFile >> token;
        if (token == "(") {
            if (!p_da->ReadPoint(defFile)) {
                defFile.close();
                return false;
            }
        }
    } while (token != ";");
    return true;
}
