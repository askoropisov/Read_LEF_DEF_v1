#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Class.h"


void trim_left(std::string& text) {                     //skip line
    size_t pos = text.find_first_not_of(" \t");
    if (!pos)
        return;
    if (pos == std::string::npos)
        return;
    text.erase(0, pos);
}

//functions read LEF section
bool LEFFile::ReadUnits(std::ifstream& lefFile) {       //read Units
    std::string line,
        token;
    while (std::getline(lefFile, line)) {
        trim_left(line);
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> token;
        if (token == "END") {
            iss >> token;
            if (token == "UNITS")
                return true;
            std::cerr << "_err_ : [Reading UNITS] 'END' statement met with unsupported token '" << token << "'. UNITS read failed." << std::endl;
            return false;
        }

        if (token == "DATABASE") {
            iss >> token;
            if (token == "MICRONS") {
                iss >> microns;                         //edinizi i3mereniya
                continue;
            }
            std::cerr << "_wrn_ : [Reading UNITS] Supported 'DATABASE' statement is 'MICRONS' only! Line ignored." << std::endl;
            continue;
        }
        std::cerr << "_wrn_ : [Reading UNITS] Unsupported token '" << token << "'. Line ignored." << std::endl;
        continue;
    }
    return true;
}

bool LEFFile::ReadSite(std::ifstream& lefFile, std::string& name) {             //read Site
    Site* p_site = new Site(name);
    sites.push_back(p_site);

    std::string line,
        token;


    while (std::getline(lefFile, line)) {
        trim_left(line);
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> token;

        if (token == "END") {
            iss >> token;
            if (token == name)
                return true;
            std::cerr << "_err_ : [Reading SITE] 'END' statement met with name different from '" << name << "' (which was used to define site). UNITS read failed." << std::endl;
            return false;
        }

        if (token == "SYMMETRY") {
            iss >> token;
            if (token == "Y")
                p_site->symmetryY = true;
            if (token == "X") {
                p_site->symmetryX = true;
                iss >> token;
                if (token == "Y")
                    p_site->symmetryY = true;
            }
            continue;
        }

        if (token == "CLASS") {
            iss >> token;
            if (token == "CORE") {
                p_site->siteClass = SiteClass::core;
                continue;
            }
            if (token == "PAD") {
                p_site->siteClass = SiteClass::pad;
                continue;
            }
            std::cerr << "_wrn_ : [Reading SITE] Unsupported SITE CLASS type '" << token << "'. Line ignored." << std::endl;
            continue;
        }

        if (token == "SIZE") {
            iss >> p_site->size.x >> token >> p_site->size.y;
            continue;
        }

        std::cerr << "_wrn_ : [Reading SITE] Unsupported token '" << token << "'. Line ignored." << std::endl;
        continue;
    }
    return true;
}


bool Pin::ReadPolygon(std::ifstream& lefFile, std::string) {            //read point position in Polygon and Rect
    Polygon* p_pol = new Polygon;
    polygons.push_back(p_pol);

    std::string line,
        token;
    while (std::getline(lefFile, line)) {
        trim_left(line);
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> token;

        if (token == "END")                             //check end section
            return true;

        if (token == "RECT") {
            for (int i = 0; ; i++) {
                iss >> token;
                if (token == ";")                       //check end REct
                    break;
                p_pol->position.push_back(std::stod(token));
            }
        }
        if (token == "POLYGON") {
            for (int i = 0; ; i++) {
                iss >> token;
                if (token == ";")                       //check end polygon
                    break;
                if (iss.eof()) {                                    //skleyka strok pri perenose
                    std::getline(lefFile, line);
                    iss.str("");
                    iss.clear();
                    iss.str(line);
                }
                p_pol->position.push_back(std::stod(token));
            }
        }
    }
    return true;
}

bool Macro::ReadPin(std::ifstream& lefFile, std::string& name) {        //read pins
    Pin* p_pin = new Pin(name);
    pins.push_back(p_pin);

    std::string line,
        token;
    while (std::getline(lefFile, line)) {
        trim_left(line);
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> token;

        if (token == "END") {                   //check end section
            iss >> token;
            if (token == name)
                return true;
            std::cerr << "_err_ : [Reading PIN] 'END' statement met with name different from '" << name << "' (which was used to define site). UNITS read failed." << std::endl;
            return false;
        }

        if (token == "DIRECTION") {
            iss >> token;
            if (token == "INPUT") {
                p_pin->direction = PinDirection::input;
                continue;
            }
            if (token == "OUTPUT") {
                p_pin->direction = PinDirection::output;
                continue;
            }
            if (token == "INOUT") {
                p_pin->direction = PinDirection::inout;
                continue;
            }
            if (token == "FEEDTHRU") {
                p_pin->direction = PinDirection::feedthru;
                continue;
            }
            if (token == "TRISTATE") {
                p_pin->direction = PinDirection::tristate;
                continue;
            }
            std::cerr << "_wrn_ : [Reading PIN] Unsupported DIRECTION'" << token << "'. Line ignored." << std::endl;
            continue;
        }
        if (token == "LAYER") {
            iss >> token;
            if (!p_pin->ReadPolygon(lefFile, token)) {
                lefFile.close();
                return false;
            }
            continue;
        }
    }
    return true;
}



bool Macro::ReadObs(std::ifstream& lefFile, std::string) {                      //read OBS(препятствия)
    OBS* p_obs = new OBS;
    obss.push_back(p_obs);
    std::string line,
        token;
    while (std::getline(lefFile, line)) {
        trim_left(line);
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> token;

        if (token == "END") {                           //check end section
            return true;
            std::cerr << "_wrn_ : [Reading LAYER] Unsupported" << token << "'. Line ignored." << std::endl;
            return false;
        }

        if (token == "RECT") {
            Polygon* p_pol = new Polygon;
            for (int i = 0; ; i++) {
                iss >> token;
                if (token == ";") {                      //check end Rect
                    break;
                }
                p_pol->position.push_back(std::stod(token));
            }
        }
        if (token == "POLYGON") {
            Polygon* p_pol = new Polygon;

            for (int i = 0; ; i++) {
                iss >> token;
                if (token == ";") {                    //check end Polygon
                    break;
                }
                if (iss.eof()) {                        //skleyka strok
                    std::getline(lefFile, line);
                    iss.str("");
                    iss.clear();
                    iss.str(line);
                }
                p_pol->position.push_back(std::stod(token));
            }
        }
    }
    return true;
}

bool LEFFile::ReadMacro(std::ifstream& lefFile, std::string& name) {                //read Macros
    Macro* p_mac = new Macro(name);
    macroes.push_back(p_mac);

    std::string line,
        token;

    while (std::getline(lefFile, line)) {
        trim_left(line);
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> token;

        if (token == "END") {                                   //check end section
            iss >> token;
            if (token == name)
                return true;
            std::cerr << "_err_ : [Reading MACRO " << name << " ] 'END' statement met with name different from '" << name << "' (which was used to define site). UNITS read failed." << std::endl;
            return false;
        }

        if (token == "CLASS") {
            iss >> token;
            if (token == "CORE") {
                p_mac->macro_class = MacroClass::core;
                continue;
            }
            if (token == "PAD") {
                p_mac->macro_class = MacroClass::pad;
                continue;
            }
            std::cerr << "_wrn_ : [Reading MACRO " << name << " ] Unsupported SITE CLASS type '" << token << "'. Line ignored." << std::endl;
            continue;
        }

        if (token == "SYMMETRY") {
            iss >> token;
            if (token == "Y")
                p_mac->symmetryY = true;
            if (token == "X") {
                p_mac->symmetryX = true;
                iss >> token;
                if (token == "Y")
                    p_mac->symmetryY = true;
            }
            continue;
        }

        if (token == "SIZE") {
            iss >> p_mac->size_x >> token >> p_mac->size_y;
            continue;
        }

        if (token == "PIN") {
            iss >> token;
            if (!p_mac->ReadPin(lefFile, token)) {
                lefFile.close();
                return false;
            }
            continue;
        }
        if (token == "OBS") {
            if (!p_mac->ReadObs(lefFile, token)) {
                lefFile.close();
                return false;
            }
            continue;
        }
        std::cerr << "_wrn_ : [Reading MACRO " << name << " ] Unsupported token '" << token << "'. Line ignored." << std::endl;
        continue;
    }
    return true;
}



