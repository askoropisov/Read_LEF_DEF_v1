#pragma once

std::vector<std::string>        name_Macro;

enum class ElementClass {               //macro and site class
    undefined = 0,
    core,
    pad,
};
enum class PinUse {                     //pin assignment
    SIGNAL, POWER, GROUND, CLOCK,
    TIEOFF, ANALOG, SCAN, RESET,
};
enum class PinDirection {               //pin direction
    undefined = 0,
    input,
    output,
    tristate,
    inout,
    feedthru,
};
enum class LayerType {                  //type layer
    undefined = 0,
    cut,
    routing,
    implant,
    masterslice,
};
enum class LayerDirection {             //layer direction
    undefined = 0,
    horizontal,
    vertical,
};

struct Size {                           //Size cell
    float x,
          y;
};

struct Site {                           //The SITE section describes the cell for the location of the components
    std::string name;
    bool        symmetryX,
                symmetryY;
    ElementClass   siteClass;
    Size        size;
public:
    Site(std::string name) : name(name), symmetryX(false), symmetryY(false), siteClass(ElementClass::undefined), size({ 0.0f, 0.0f }) {}
};


//LEF
class Polygon {                         
public:
    std::vector<double>  position;          //position point
};

class Pin {
public:
    std::string             name;
    PinDirection            direction;
    std::vector<Polygon*>   polygons;
public:
    Pin(std::string name);
    ~Pin();
public:
    bool ReadPolygon(std::ifstream& lefFile, std::string);
};
Pin::Pin(std::string name) : name(name), direction(PinDirection::undefined) {}
Pin::~Pin() {}

class OBS {
public:
    std::vector<Polygon*>   polygons;
public:
    ~OBS();
public:
    bool ReadPolygon(std::ifstream& lefFile, std::string);
};

class Macro {
public:
    std::string         name;
    std::vector<Pin*>   pins;
    std::vector<OBS*>   obss;
    ElementClass        site_class;
    float size_x, size_y;
    bool        symmetryX,
                symmetryY;
    ElementClass        macro_class;
public:
    Macro(std::string name);
    ~Macro();
public:
    bool ReadPin(std::ifstream& lefFile, std::string& name);
    bool ReadObs(std::ifstream& lefFile, std::string);
};

Macro::Macro(std::string name) : name(name) {}
Macro::~Macro() {
    for (size_t i = 0; i < pins.size(); ++i)
        delete pins[i];
    pins.clear();
}

class LEFFile {
public:
    std::string           fileName;
    // LEF data starts here
    float                 version;
    int                   microns;  // units
    float                 manufacturingGrid;
    std::vector<Site*>    sites;
    std::vector<Macro*>   macroes;
public:
    LEFFile();
    ~LEFFile();
public:
    bool Read(std::string filename);
private:
    bool ReadUnits(std::ifstream& lefFile);
    bool ReadSite(std::ifstream& lefFile, std::string& name);
    bool ReadMacro(std::ifstream& lefFile, std::string& name);
};

LEFFile::LEFFile() {}
LEFFile::~LEFFile() {
    for (size_t i = 0; i < sites.size(); ++i)
        sites[i];
    sites.clear();
    for (size_t i = 0; i < macroes.size(); ++i)
        delete macroes[i];
    macroes.clear();
}