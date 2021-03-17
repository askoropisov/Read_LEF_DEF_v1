#pragma once


enum class Orintation {                 //orientation of the element on the crystall
    undefined = 0,
    N, S, E, W,
    FN, FS, FE, FW,
};

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
public:
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