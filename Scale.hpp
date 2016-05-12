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
	std::vector<std::string> currentScaleFixed;
	std::vector<std::string> currentChord;
	std::vector<std::vector<std::string> > currentProgression;

	
public:
	bool loadScales();
	void makeScale(std::string root, std::string inputScale);
	
		std::vector<int> setSelectedScale(std::string inputScale);

	void fixScale();
	void fixScaleSharp();
	void fixScaleFlat();
	
	void makeChord(int rootNote, std::vector<int> intervals);

	//Simple function returns false if current chord is not empty
	//and true if it is.	
	bool isCurrentChordEmpty(){
		if(currentChord.size() > 0) return false;
		return true;
	}

	//Getter for current chord	
	std::vector<std::string> getCurrentChord(){
		return currentChord;
	}

	//Getter for current progression
	std::vector< std::vector<std::string> > getProgression(){
		return currentProgression;
	}

	//Getter returns all scale names
	std::vector<std::string> getScaleNames() {
		std::vector<std::string> names;
		for (unsigned int i = 0; i < scaleStepsVec.size(); i++) {
			names.push_back(scaleStepsVec.at(i).scaleName);
		}
		return names;
	}

	//Simple function that removes chord vector in progression vector 
	void removeFromProgression(int positionInProgression){
   		currentProgression.erase(currentProgression.begin() + positionInProgression);
	}

	//Simple function that adds chord vector to progression vector.	
	void addToProgression(){
		currentProgression.push_back(currentChord);
	}

	//Getter for current scalse
	std::vector<std::string> getCurrentScale(){
		return currentScale;
	}

	//Getter for current fixed scale
	std::vector<std::string> getCurrentScaleFixed(){
		return currentScaleFixed;
	}

	//Simple function to clear the current scale	
	bool clearScale() { 
		currentScale.erase(currentScale.begin(), currentScale.end()); 
		return true;
	}

	//Sipmle function to clear current chord
	void clearCurrentChord(){
		currentChord.clear();
	}
};
 
#endif
