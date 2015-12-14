#ifndef SCALE_HPP
#define SCALE_HPP

#include <string>
#include <vector>
#include <iostream>

struct ScaleNotes {
	int ident;
	std::string note;
};

struct ScaleSteps {
	std::string scaleName;
	std::vector<int> steps;
};

struct OneChord {
	std::string name;
	int numOfNotes;
	std::vector<std::string> notesInChord;
	OneChord(int num) {
		numOfNotes = num;
		switch (num){
		case 3:
			name = "TRIAD"; 
			break;
		case 5:
			name = "FIFTH";
			break;
		case 7:
			name = "SEVENTH";
			break;
		case 9:
			name = "NINTH";
			break;
		case 11:
			name = "ELEVENTH";
			break;
		case 13:
			name = "THIRTEENTH";
			break;

		}
	}
};

struct ChordsFromNotes {
	std::string RootNote;
	std::vector<OneChord> chordsFromOneNote;
};

class Scale {
protected:
	std::vector<ScaleSteps> scaleStepsVec;
	std::vector<ScaleNotes> notes;
	std::vector<std::string> chromScale = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
	std::vector<std::string> altChromScale = { "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" };
	std::vector<std::string> currentScaleKey;
	std::vector<std::string> currentScaleFixed;
	std::vector<ChordsFromNotes> chordsKeys;
	std::vector<ChordsFromNotes> chordsFixed;
	
	
public:
	bool loadScales();
	bool makeScale(std::string root, std::string inputScale);
	
	bool clearScale() { 
		currentScaleKey.erase(currentScaleKey.begin(), currentScaleKey.end()); 
		currentScaleFixed.erase(currentScaleFixed.begin(), currentScaleFixed.end());
		return true;
	}

	std::vector<int> setSelectedScale(std::string inputScale);

	std::vector<std::string> getCurrentKey() { return currentScaleKey; }
	std::vector<std::string> getCurrentFixed() { return currentScaleFixed; }

	std::vector<std::string> fixScale(std::vector<std::string> inputScale);
	std::vector<std::string> fixScaleSharp(std::vector<std::string> inputScale);
	std::vector<std::string> Scale::fixScaleFlat(std::vector<std::string> inputScale);

	bool makeChords();

	std::vector<ChordsFromNotes> getChordsKeys() { return chordsKeys;	}
	std::vector<ChordsFromNotes> getChordsFixed() { return chordsFixed;  }

	std::vector<std::string> getScaleNames() {
		std::vector<std::string> names;
		for (int i = 0; i < scaleStepsVec.size(); i++) {
			names.push_back(scaleStepsVec.at(i).scaleName);
		}
		return names;
	}

};
 
#endif