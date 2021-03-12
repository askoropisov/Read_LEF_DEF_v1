#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "Class.h"
#include "LEF_reading.h"
#include "DEF_reading.h"



//Read LEF file
bool LEFFile::Read(std::string filename) {
    std::ifstream lefFile(filename);
    if (!lefFile.is_open())
        return false;

    std::string line,
        token;
    while (std::getline(lefFile, line)) {
        // 1. Trim the line to be sure that first symbol is nos a space char
        trim_left(line);
        // 2. Ignore empty and commented lines
        if (line.empty())
            continue;
        if (line[0] == '#')
            continue;

        std::istringstream iss(line);
        iss >> token;
        // 3. Ignore unimportans statements
        if (token == "DIVIDERCHAR")
            continue;
        if (token == "BUSBITCHARS")
            continue;
        // 4. Read importans statements
        if (token == "VERSION") {
            iss >> version;
            continue;
        }
        if (token == "UNITS") {
            if (!ReadUnits(lefFile)) {
                lefFile.close();
                return false;
            }
            continue;
        }
        if (token == "MANUFACTURINGGRID") {
            iss >> manufacturingGrid;
            continue;
        }
        if (token == "SITE") {
            iss >> token;
            if (!ReadSite(lefFile, token)) {
                lefFile.close();
                return false;
            }
            continue;
        }
        if (token == "MACRO") {
            iss >> token;
            if (!ReadMacro(lefFile, token)) {
                lefFile.close();
                return false;
            }
            continue;
        }
    }

    lefFile.close();
    fileName = filename;
    return true;
}

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
            std::cout << std::endl << "DIEAREA successfully read";
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
                    std::cout << std::endl << "COMPONENTS successfully read";
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
                    std::cout << std::endl << "PINS successfully read";
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


int main() {

    std::string lefFileName = "ilin.lef";
    std::string defFileName = "mult24.def";

    //LEF main
    LEFFile lef;
    if (!lef.Read(lefFileName)) {
        std::cerr << "_err_ : Can't read input file '" << lefFileName << "'." << std::endl << "\tAbort." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << std::endl << " LEF file read successfully" << std::endl;


    //DEF main
    DEFFile def;
    if (!def.Read(defFileName)) {
        std::cerr << "_err_ : Can't read input file '" << defFileName << "'." << std::endl << "\tAbort." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << std::endl << " DEF file read successfully" << std::endl;
    return EXIT_SUCCESS;
}
