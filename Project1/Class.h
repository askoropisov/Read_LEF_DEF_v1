#pragma once


enum class ElementClass {               //macro and site class
    undefined = 0,
    core,
    pad,
};
enum class Orintation {                 //orientation of the element on the crystall
    undefined = 0,
    N, S, E, W,
    FN, FS, FE, FW,
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

//DEF
class Point {                           //for DieArea 
public:
    int x_position;
    int y_position;
};

class DieArea {                     //crystal size and form
public:
    std::vector<Point*>  points;
public:
    bool ReadPoint(std::ifstream& defFile);
};

class Row {                         //lines with components
public:
    std::string          name;
    Orintation           orintation;
    int                  count_horizontal;
    int                  count_vertical;
    int                  step_horizontal;
    int                  step_vertical;
    int                  x_position;                        //first cell position
    int                  y_position;
public:
    Row(std::string name);
};
Row::Row(std::string name) : name(name) {}

class Component {
public:
    std::string         name;
    std::string         name_model_in_LEF;                  //name model in LEF file
    Orintation          orintation;
    int                 x_position, y_position;             //location
public:
    Component(std::string name);
};
Component::Component(std::string name) : name(name) {}

class PinDef {
public:
    std::string         name;
    std::string         name_connected_wire;                //name connected wire
    PinDirection        direction;
    int                 x_position, y_position;             //location
    Orintation          orintation;
    PinUse              use;
public:
    PinDef(std::string name);
};
PinDef::PinDef(std::string name) : name(name) {}

class DEFFile {
    std::string                 fileName;
    std::string                 desing_name;
    std::vector<DieArea*>       dieareas;                    //size and form cristall
    std::vector<Row*>           rows;
    std::vector<Component*>     components;
    std::vector<PinDef*>        pins;
    float                       version;
    int                         microns;                // units
    int                   count_components, count_pins, count_special_nets, count_nets;     //kol-vo pins, components, ...
public:
    DEFFile();
    ~DEFFile();
public:
    bool Read(std::string filename);
private:
    bool ReadRow(std::ifstream& defFile, std::string& name);
    bool ReadDieArea(std::ifstream& defFile);
    bool ReadComponents(std::ifstream& defFile, std::string& name);
    bool ReadPins(std::ifstream& defFile, std::string& name);
    bool ReadSpecialNets(std::ifstream& defFile);
    bool ReadNets(std::ifstream& defFile);
};

DEFFile::DEFFile() {}
DEFFile::~DEFFile() {
    for (size_t i = 0; i < rows.size(); ++i)
        delete rows[i];
    rows.clear();
}


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