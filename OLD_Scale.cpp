#include "Scale.hpp"
#include <fstream>

/*************************************************************************************
** Function: setSelectedSCale
** Most recent date modified: 2015/10/23
** Description: Take name of scale and returns vector with selected scale
*************************************************************************************/
std::vector<int> Scale::setSelectedScale(std::string inputScale) {
	int pos = -1;

	//Make empty scale
	std::vector<int> theScale = { 0 };

	//Loop through vector of scale name to find selected scale
	for (int i = 0; i < scaleStepsVec.size(); i++) {
		if (inputScale == scaleStepsVec.at(i).scaleName) pos = i;
	}

	//Return empty scale if selected scale not found
	if (pos == -1) return theScale;
	//Fill scale with interval ints if found
	else {
		for (int i = 0; i < scaleStepsVec.at(pos).steps.size(); i++) {
			//Add int from steps vector held in vector of scaleSteps struct
			theScale.push_back(scaleStepsVec.at(pos).steps.at(i));
		}
	}

	//Return scale
	return theScale;
}

/*************************************************************************************
** Function: loadScales
** Most recent date modified: 2015/10/23
** Description: Reads scale information from .txt file and stores information in 
**			 vectors.
*************************************************************************************/
bool Scale::loadScales() {
	
	//Open input file (hardcoded as scales.txt)
	std::ifstream inFile;
	inFile.open("scales.txt");
	if (inFile.fail()) {
		return false;
	}
	
	//Read file and store info in vectors
	while (!inFile.eof()) {
		
		//Vector for scale information
		ScaleSteps temp;

		//Input string
		std::string inp = "";
		
		std::getline(inFile, inp);
		
		//Set name of scale
		temp.scaleName = inp;
		
		//Get next line for scale intervals
		std::getline(inFile, inp);
		std::string nums = "";
		
		//Add intervals to vector of ints
		for (int i = 0; i < inp.length(); i++) {
			//Put interval int when space is found
			if (inp.at(i) == ' ') {
				std::string z = nums;
				
				temp.steps.push_back(atoi(z.c_str()));
				nums = "";
			}
			
			else nums += inp.at(i);
			
			//If at end of line, push final int
			if(i == inp.length() - 1) {
				std::string z = nums;
				temp.steps.push_back(atoi(z.c_str()));
				nums = "";
			}
		}

		//Add scale vector to vector of scale vectors
		scaleStepsVec.push_back(temp);
	}

	//Close input file
	inFile.close();

	return true;
}

/*************************************************************************************
** Function: makeScale
** Most recent date modified: 2015/10/23
** Description: Uses chromatic scale and vector of scale intervals to find the notes
**              in a chosen scale. Returns vector of strings.
*************************************************************************************/
std::vector<std::string> Scale::makeScale(std::string root, std::string inputScale) {
	
	//If scale var is currently full, clear it
	if(currentScale.size() > 0) clearScale();

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
	
	for (int i = 0; i < scaleReturn.size(); i++) {
		std::cout << std::setw(4) << std::left << scaleReturn.at(i) << " ";
	}

	std::cout << std::endl;

	if(root.length() < 2) scaleReturn = fixScale(scaleReturn);
	else if (root.at(1) == '#') scaleReturn = fixScaleSharp(scaleReturn);
	else if (root.at(1) == 'b') scaleReturn = fixScaleFlat(scaleReturn);

	//Set currentScale to scaleReturn, so currentScale can be used to find chords
	currentScale = scaleReturn;

	return scaleReturn;
}

/*************************************************************************************
** Function: fixScale
** Most recent date modified: 2015/10/23
** Description: Set output notes so every note is represented (A through G) instead
**			 of note names on keyboard.
*************************************************************************************/
std::vector<std::string> Scale::fixScale(std::vector<std::string> inputScale) {
	
	std::vector<std::string> outScale = inputScale;

	//If less than 7 notes, this won't work, so just return it.
	if (outScale.size() < 6) return outScale;

	//Get root note
	int stop = 0;
	std::string root = outScale.at(0);

	//Find position of root note on 12-step scale
	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	//Basic scalse
	std::vector<std::string> aToG = {"A", "B", "C", "D", "E", "F", "G" };

	int aToGPos = 0;

	//Set aToGPos to position of root note on the aToG scale
	for (int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;
	}

	//Loop through vector of notes until we get back to the root note (descending half corrected later)
	for (int i = 1; i < stop; i++) {
		//Wrap around scale while incrementing and set swap equal to next note
		std::string swap = aToG.at((aToGPos + i) % 7);

		//If next note does not equal note in scale vector...
		if (outScale.at(i).at(0) != swap.at(0)) {

			//...find out how long the note in the vector is and...
			int len = outScale.at(i).length();
			//...find out how big the difference is between current note and "correct" note
			//For example: Current note is C, but should be B, so the differnce is -1
			int swapDiff = outScale.at(i).at(0) - swap.at(0);

			//If the length of the current note is greater than one (i.e. if it has a sharp or flat)
			if (len > 1) {
				//Set note in vector to correct note
				outScale.at(i).at(0) = swap.at(0);
				//If note was sharp, make it flat.
				if (outScale.at(i).at(1) == '#') {
					outScale.at(i).at(1) = 'b';
				}
				//Otherwise, just make it sharp.
				else {
					outScale.at(i).at(1) = '#';
				}
			}
			//If the note string lenght is only 1 (i.e. is has not flat or sharp)
			else {
				//Set the correct note
				outScale.at(i) = swap;
				//If the difference is positive, append a sharp
				if (swapDiff > 0) {
					outScale.at(i) = swap + "#";
				}
				//Otherwise, append a flat.
				else outScale.at(i) = swap + "b";
			}
		}
	}

	//Perform above operation backwards for descending half of the scale
	for (int i = outScale.size()-2; i > stop; i--) {
		aToGPos++;
		//Main difference is in wrapping procedure
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

/*************************************************************************************
** Function: fixScaleSharp
** Most recent date modified: 2015/10/23
** Description: Set output notes so every note is represented (A through G) instead
**			 of note names on keyboard.
*************************************************************************************/
std::vector<std::string> Scale::fixScaleSharp(std::vector<std::string> inputScale) {
	std::vector<std::string> outScale = inputScale;

	if (outScale.size() < 6) return outScale;

	int stop = 0;
	std::string root = outScale.at(0);

	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	std::vector<std::string> aToG = { "A#", "B#", "C#", "D#", "E#", "F#", "G#" };

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
			
			if (len > 1) {
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

			if (len > 1) {
				outScale.at(i) = outScale.at(i) + "#";
			}
			
		}
	}

	return outScale;
}

/*************************************************************************************
** Function: fixScaleFlat
** Most recent date modified: 2015/10/23
** Description: Set output notes so every note is represented (A through G) instead
**			 of note names on keyboard.
*************************************************************************************/
std::vector<std::string> Scale::fixScaleFlat(std::vector<std::string> inputScale) {
	std::vector<std::string> outScale = inputScale;

	if (outScale.size() < 6) return outScale;

	int stop = 0;
	std::string root = outScale.at(0);

	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	std::vector<std::string> aToG = { "Ab", "Bb", "Cb", "Db", "Eb", "Fb", "Gb" };

	int aToGPos = 0;
	for (int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;

	}

	for (int i = 1; i < stop; i++) {
		int len = outScale.at(i).length();
		std::string swap = aToG.at((aToGPos + i) % 7);
		if (outScale.at(i).at(0) != swap.at(0)) {

			int swapDiff = outScale.at(i).at(0) - swap.at(0);
			
			if (outScale.at(i) == "A" && swap == "Bb") {
				outScale.at(i) = "Bb";
				len = 2;
			}
			else if (outScale.at(i) == "D" && swap == "Eb") {
				outScale.at(i) = "Eb";
				len = 2;
			}
			else if (outScale.at(i) == "G" && swap == "Ab") {
				outScale.at(i) = "Ab";
				len = 2;
			}
			else if (outScale.at(i) == "C" && swap == "Db") {
				outScale.at(i) = "Db";
				len = 2;
			}
			else outScale.at(i) = swap;

			if (len > 1) {
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
			if (outScale.at(i) == "A" && swap == "Bb") {
				outScale.at(i) = "Bb";
				len = 2;
			} 
			else if (outScale.at(i) == "D" && swap == "Eb") {
				outScale.at(i) = "Eb";
				len = 2;
			}
			else if (outScale.at(i) == "G" && swap == "Ab") {
				outScale.at(i) = "Ab";
				len = 2;
			}
			else if (outScale.at(i) == "C" && swap == "Db") {
				outScale.at(i) = "Db";
				len = 2;
			}
			else outScale.at(i) = swap;

			if (len > 1) {
				outScale.at(i) = outScale.at(i) + "b";
			}
			

		}
	}

	return outScale;
}