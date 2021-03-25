#pragma once

enum class PinDirectionVerilog {               //pin direction
	undefined = 0,
	input,
	output,
	inout,
};

class Pin_Verilog {
public:
	std::string   name_pin;
	std::string   name_wire;

};

class Element {
public:
	std::string					name;
	std::string					name_component_in_def;
	std::vector<Pin_Verilog*>   pins;
public:
	bool ReadPinVerilog(std::ifstream& verFile, std::string& word);
public:
	Element(std::string name);
	~Element();
};

Element::Element(std::string name) : name(name) {}
Element::~Element() {
	for (size_t i = 0; i < pins.size(); ++i)
		delete pins[i];
	pins.clear();
}


class Inout {
public:
	std::string  name_inout;
	PinDirectionVerilog   direction;
	int          bit_size;
};

class VERFile {
public:
	std::vector<std::string>	name_wire;
	std::vector<Element*>		elements;
	std::vector<Inout*>			inouts;
public:
	VERFile();
	~VERFile();
public:
	bool Read(std::string filename);
	bool ReadInout(std::ifstream& verFile, std::string& direction);
	bool ReadElement(std::ifstream& verFile, std::string& name);
};

VERFile::VERFile() {}
VERFile::~VERFile() {
  for (size_t i = 0; i < elements.size(); ++i)
    delete elements[i];
  elements.clear();

  for (size_t i = 0; i < inouts.size(); ++i)
	  delete inouts[i];
  inouts.clear();
}
