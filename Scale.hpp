#ifndef SCALE_HPP
#define SCALE_HPP

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>

struct ScaleNotes {
	int ident;
	std::string note;
};

struct ScaleSteps {
	std::string scaleName;
	std::vector<int> steps;
};

class Scale {
protected:
	std::vector<ScaleSteps> scaleStepsVec;
	std::vector<ScaleNotes> notes;
	std::vector<std::string> chromScale = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
	std::vector<std::string> altChromScale = { "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" };
	std::vector<std::string> currentScale;
	
	
public:
	bool loadScales();
	std::vector<std::string> makeScale(std::string root, std::string inputScale);
	
	bool clearScale() { 
		currentScale.erase(currentScale.begin(), currentScale.end()); 
		return true;
	}

	std::vector<int> setSelectedScale(std::string inputScale);

	std::vector<std::string> fixScale(std::vector<std::string> inputScale);
	std::vector<std::string> fixScaleSharp(std::vector<std::string> inputScale);
	std::vector<std::string> fixScaleFlat(std::vector<std::string> inputScale);

	std::vector<std::string> getScaleNames() {
		std::vector<std::string> names;
		for (int i = 0; i < scaleStepsVec.size(); i++) {
			names.push_back(scaleStepsVec.at(i).scaleName);
		}
		return names;
	}

};
 
#endif