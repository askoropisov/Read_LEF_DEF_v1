#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "LEF_class.h"
#include "DEF_class.h"
#include "VER_class.h"
#include "LEF_reading.h"
#include "DEF_reading.h"
#include "VER_reading.h"


std::string lefFileName = "D_CELLS_test.lef";
std::string defFileName = "mult24.def";
std::string verFileName = "mult24.v";

LEFFile lef;
DEFFile def;
VERFile ver;

//исправить без глобальных переменных
bool check_compliance(){                                        //LEF and DEF file compatibility check
    std::string target_name;
    int value_sovp = 0;                                         //count sovpadeniy
    for (int i = 0; i < name_Component.size(); i++) {           //count Component in DEF
        target_name = name_Component[i];

        for (int j = 0; j < name_Macro.size(); j++) {           //count Macro in LEF
            if (target_name == name_Macro[j]) {
                value_sovp++;
                break;
            }      
        } 
    }
    if (value_sovp == name_Component.size()) {
        std::cout << std::endl << std::endl << " LEF file and DEF file they match!" << std::endl;
        return true;
    }
    else
        return false;
}



int main() {

    //LEF main
    
    if (!lef.Read(lefFileName)) {
        std::cerr << "_err_ : Can't read input file '" << lefFileName << "'." << std::endl << "\tAbort." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << std::endl << " LEF file read successfully" << std::endl;


    //DEF main
    
    if (!def.Read(defFileName)) {
        std::cerr << "_err_ : Can't read input file '" << defFileName << "'." << std::endl << "\tAbort." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << std::endl <<std::endl<< " DEF file read successfully" << std::endl;


    //Verilog main
    VERFile ver;
    if (!ver.Read(verFileName)) {
        std::cerr << "_err_ : Can't read input file '" << verFileName << "'." << std::endl << "\tAbort." << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << std::endl << std::endl << " Verilog file read successfully" << std::endl;


    if (!check_compliance())                                        //LEF and DEF file compatibility check
    {
        std::cout << std::endl << std::endl << "_err_ : LEF file " << lefFileName << " and DEF file " << defFileName << " don't match!" << std::endl;
        return EXIT_FAILURE;
    };  

    return EXIT_SUCCESS;
}



