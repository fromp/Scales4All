#include "Scale.hpp"
#include <fstream>

/*************************************************************************************
** Function: setSelectedScale
** Most recent date modified: 2015/9/23
** Description: Uses inputScale string to loop through all scales loaded from
**				text file to find match and load intervals and then return intervals
**              in a vector.
*************************************************************************************/
std::vector<int> Scale::setSelectedScale(std::string inputScale) {
	
	//pos variable will hold index of scale.
	int pos = -1;

	//temporary vector for returning intervals
	std::vector<int> theScale = { 0 };

	//loop through vector with all scales to find name match
	//scales are stored as struct & .scaleName holds scale name
	for (int i = 0; i < scaleStepsVec.size(); i++) {
		if (inputScale == scaleStepsVec.at(i).scaleName) pos = i;
	}

	//if not found, return with scale of interval 0
	if (pos == -1) return theScale;
	//otherwise, look through found scale and put intervals in return scale
	else {
		for (int i = 0; i < scaleStepsVec.at(pos).steps.size(); i++) {
			theScale.push_back(scaleStepsVec.at(pos).steps.at(i));
		}
	}
	
	//and return it
	return theScale;
}

/*************************************************************************************
** Function: loadScale
** Most recent date modified: 2015/9/23
** Description: Open text file with scales and load the information using vectors and
**              structures
*************************************************************************************/
bool Scale::loadScales() {

	//open file
	std::ifstream inFile;
	inFile.open("scales.txt");
	if (inFile.fail()) {
		return false;
	}
	
	//Read to end of file
	while (!inFile.eof()) {

		//temp structure to hold each scale (name and intervals)
		ScaleSteps temp;

		//temporary input variable
		std::string inp = "";

		//scale name to input
		std::getline(inFile, inp);
		temp.scaleName = inp;

		//get all intervals
		std::getline(inFile, inp);
		
		//loop through interval string to extract intervals
		//and save them in a vector
		std::string nums = "";
		for (int i = 0; i < inp.length(); i++) {
			//delinated by spaces, if space convert and add to vector
			if (inp.at(i) == ' ') {
				//convert nums to int and push to struct
				temp.steps.push_back(atoi(nums.c_str()));
				nums = "";
			}
			//otherwise, add char to nums
			else nums += inp.at(i);
			//If at end of string, convert and push
			if(i == inp.length() - 1) {
				std::string z = nums;
				temp.steps.push_back(atoi(z.c_str()));
				nums = "";
			}
		}
		//add temp struct to scaleStepsVec (vector of ScaleSteps struct defined by class)
		scaleStepsVec.push_back(temp);
	}

	//close the file
	inFile.close();

	return true;
}

/*************************************************************************************
** Function: makeScale
** Most recent date modified: 2015/9/23
** Description: Uses chromatic scale and vector of scale intervals to find the notes
**              in a chosen scale. Returns vector of strings.
*************************************************************************************/
bool Scale::makeScale(std::string root, std::string inputScale) {
	
	//If scale var is currently full, clear it
	if(currentScaleKey.size() > 0) clearScale();
	if(currentScaleFixed.size() > 0) clearScale();

	//Get scale intervals
	std::vector<int> selectedScale = setSelectedScale(inputScale);
	
	//Var for start position (found by comparing root note to chromatic scale
	int startPos = -1;

	//Bool var for determining if alt chromatic scale should be used
	bool alt = false;
	
	//Search chromatic scale
	for (int i = 0; i < chromScale.size(); i++) {
		if (chromScale[i] == root) startPos = i;
	}
	
	//Search alt chromatic scale
	if (startPos == -1) {
		alt = true;
		for (int i = 0; i < altChromScale.size(); i++) {
			if (altChromScale[i] == root) startPos = i;
		}
	}
	
	//Var for looping through scale interval vector
	int id = 0;

	//var for holding current note, to be pushed to vector
	std::string tempNote = root;

	//Var for holding position on Chromatic scale
	int z = startPos;
	
	//Vector holding string representing all notes in scale
	std::vector<std::string> scaleReturn;
	
	//Loop through interval vector
	while (id < selectedScale.size()) {
		//Position in chromatic scale = int in interval vector
		//Position in interval vector is incremented by 1 (using id var)
		z += selectedScale[id];

		//If outside range of chromatic scale, "loop" around
		if (z > 11) z -= 12;
		if (z < 0) z += 12;

		//tempNote = "note" on chromatic scale, position z
		if(!alt) tempNote = chromScale[z];
		else tempNote = altChromScale[z];
		
		//Push tempNote to scaleReturn
		scaleReturn.push_back(tempNote);
		
		id++;		
	}
	
	std::cout << std::endl;

	//Set currentScaleKey to scaleReturn, so currentScaleKey can be used to find chords
	currentScaleKey = scaleReturn;
	
	//fix scale for proper scale (instead of keyboard notes)
	if(root.length() < 2) scaleReturn = fixScale(scaleReturn);
	else if (root.at(1) == '#') scaleReturn = fixScaleSharp(scaleReturn);
	else if (root.at(1) == 'b') scaleReturn = fixScaleFlat(scaleReturn);

	//Set currentScaleFixed to scaleReturn, so currentScaleFixed can be used to find chords
	currentScaleFixed = scaleReturn;

	return true;
}

/*************************************************************************************
** Function: fixScale
** Most recent date modified: 2015/9/23
** Description: Uses chromatic scale and vector of scale intervals to modify notes
**              so each "letter" is used and appropriate flats/sharps are added.
*************************************************************************************/
std::vector<std::string> Scale::fixScale(std::vector<std::string> inputScale) {
	std::vector<std::string> outScale = inputScale;

	//if (outScale.size() < 6) return outScale;

	int stop = 0;
	std::string root = outScale.at(0);

	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	if (stop < 6) return outScale;

	std::vector<std::string> aToG = {"A", "B", "C", "D", "E", "F", "G" };

	int aToGPos = 0;
	for (int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;

	}

	for (int i = 1; i < stop; i++) {
		std::string swap = aToG.at((aToGPos + i) % 7);
		if (outScale.at(i).at(0) != swap.at(0)) {

			int len = outScale.at(i).length();
			int swapDiff = outScale.at(i).at(0) - swap.at(0);

			if (len > 1) {
				
				outScale.at(i).at(0) = swap.at(0);
				if (outScale.at(i).at(1) == '#') {
					outScale.at(i).at(1) = 'b';
				}
				else {
					
					outScale.at(i).at(1) = '#';
				}
			}
			else {
				outScale.at(i) = swap;
				if (swapDiff > 0) {
					outScale.at(i) = swap + "#";
				}
				else outScale.at(i) = swap + "b";
			}
		}
	}

	for (int i = outScale.size()-2; i > stop; i--) {
		aToGPos++;
		if (aToGPos > 6) aToGPos -= 7;

		std::string swap = aToG.at(aToGPos);
		if (outScale.at(i).at(0) != swap.at(0)) {

			int len = outScale.at(i).length();
			int swapDiff = outScale.at(i).at(0) - swap.at(0);

			if (len > 1) {

				outScale.at(i).at(0) = swap.at(0);
				if (outScale.at(i).at(1) == '#') {
					outScale.at(i).at(1) = 'b';
				}
				else {
					
					outScale.at(i).at(1) = '#';
				}
			}
			else {
				outScale.at(i) = swap;
				if (swapDiff > 0) {
					outScale.at(i) = swap + "#";
				}
				else outScale.at(i) = swap + "b";
			}
		}
	}
	
	return outScale;
}

std::vector<std::string> Scale::fixScaleSharp(std::vector<std::string> inputScale) {
	std::vector<std::string> outScale = inputScale;

	//if (outScale.size() < 6) return outScale;

	int stop = 0;
	std::string root = outScale.at(0);

	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	if (stop < 6) return outScale;

	std::vector<std::string> aToG = { "A#", "B", "C#", "D#", "E", "F#", "G#" };

	int aToGPos = 0;
	for (int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;

	}

	for (int i = 1; i < stop; i++) {
		int len = outScale.at(i).length();
		std::string swap = aToG.at((aToGPos + i) % 7);
		if (outScale.at(i).at(0) != swap.at(0)) {

			int swapDiff = outScale.at(i).at(0) - swap.at(0);
			outScale.at(i) = swap;
			
			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "#";
			}
			
		}
	}

	for (int i = outScale.size() - 2; i > stop; i--) {
		aToGPos++;
		int len = outScale.at(i).length();
		
		if (aToGPos > 6) aToGPos -= 7;

		std::string swap = aToG.at(aToGPos);
		
		if (outScale.at(i).at(0) != swap.at(0)) {

			int swapDiff = outScale.at(i).at(0) - swap.at(0);
			outScale.at(i) = swap;

			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "#";
			}
			
		}
	}

	return outScale;
}

std::vector<std::string> Scale::fixScaleFlat(std::vector<std::string> inputScale) {
	std::vector<std::string> outScale = inputScale;

	//if (outScale.size() < 6) return outScale;

	int stop = 0;
	std::string root = outScale.at(0);

	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	if (stop < 6) return outScale;

	std::vector<std::string> aToG = { "Ab", "Bb", "C", "Db", "Eb", "F", "Gb" };

	int aToGPos = 0;
	for (int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;

	}

	for (int i = 1; i < stop; i++) {
		int len = outScale.at(i).length();
		std::string swap = aToG.at((aToGPos + i) % 7);
		if (outScale.at(i).at(0) != swap.at(0)) {

			int swapDiff = outScale.at(i).at(0) - swap.at(0);
			outScale.at(i) = swap;

			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "b";
			}

		}
	}

	for (int i = outScale.size() - 2; i > stop; i--) {
		aToGPos++;
		int len = outScale.at(i).length();

		if (aToGPos > 6) aToGPos -= 7;

		std::string swap = aToG.at(aToGPos);

		if (outScale.at(i).at(0) != swap.at(0)) {

			int swapDiff = outScale.at(i).at(0) - swap.at(0);
			outScale.at(i) = swap;

			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "b";
			}

		}
	}

	return outScale;
}

bool Scale::makeChords() {
	//chordsKeys
	//chordsfromnotes
	//onechord

	
	for (int i = 0; i < 7; i++) {
		ChordsFromNotes temp;
		int count = 3;
		
		temp.RootNote = currentScaleKey.at(i);
		
		int chord = 3;
		while (count < 9) {
			int note = 0;
			int counter = 0;
			//int chord = 3;
			OneChord tempChords(chord);
			while (counter < count) {
				tempChords.notesInChord.push_back(currentScaleKey.at((i + note) % 7));
				note += 2;
				counter++;
			}
			temp.chordsFromOneNote.push_back(tempChords);
			count ++;
			chord += 2;
		}
		chordsKeys.push_back(temp);
	}
	

	//chordsFixed
	
	for (int i = 0; i < 7; i++) {
		ChordsFromNotes temp;
		int count = 3;
		
		temp.RootNote = currentScaleFixed.at(i);
		
		int chord = 3;
		while (count < 9) {
			int note = 0;
			int counter = 0;
			//int chord = 3;
			OneChord tempChords(chord);
			while (counter < count) {
				tempChords.notesInChord.push_back(currentScaleFixed.at((i + note) % 7));
				note += 2;
				counter++;
			}
			temp.chordsFromOneNote.push_back(tempChords);
			count ++;
			chord += 2;
		}
		chordsFixed.push_back(temp);
	}

	return true;
}