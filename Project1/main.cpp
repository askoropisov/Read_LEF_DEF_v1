#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "LEF_class.h"
#include "DEF_class.h"
#include "LEF_reading.h"
#include "DEF_reading.h"





bool check_compliance(std::string, std::string)
{
    return true;
}

int main() {

    std::string lefFileName = "D_CELLS_test.lef";
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
    std::cout << std::endl <<std::endl<< " DEF file read successfully" << std::endl;
    check_compliance(lefFileName, defFileName);
    return EXIT_SUCCESS;
}

