#include "Scale.hpp"
#include <fstream>

/*************************************************************************************
** Function: setSelectedSCale
** Description: Take name of scale and returns vector with selected scale
*************************************************************************************/
std::vector<int> Scale::setSelectedScale(std::string inputScale) {
	int pos = -1;

	//Make empty scale
	std::vector<int> theScale = { 0 };

	//Loop through vector of scale name to find selected scale
	for (unsigned int i = 0; i < scaleStepsVec.size(); i++) {
		if (inputScale == scaleStepsVec.at(i).scaleName) pos = i;
	}

	//Return empty scale if selected scale not found
	if (pos == -1) return theScale;
	//Fill scale with interval ints if found
	else {
		for (unsigned int i = 0; i < scaleStepsVec.at(pos).steps.size(); i++) {
			//Add int from steps vector held in vector of scaleSteps struct
			theScale.push_back(scaleStepsVec.at(pos).steps.at(i));
		}
	}

	//Return scale
	return theScale;
}

/*************************************************************************************
** Function: loadScales
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
		for (unsigned int i = 0; i < inp.length(); i++) {
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
** Description: Uses chromatic scale and vector of scale intervals to find the notes
**              in a chosen scale. Returns vector of strings.
*************************************************************************************/
void Scale::makeScale(std::string root, std::string inputScale) {
	
	//If scale var is currently full, clear it
	if(currentScale.size() > 0) clearScale();	
	
	//Get scale intervals
	std::vector<int> selectedScale = setSelectedScale(inputScale);
	
	//Var for start position (found by comparing root note to chromatic scale
	int startPos = -1;

	//Bool var for determining if alt chromatic scale should be used
	bool alt = false;
	
	//Search chromatic scale
	for (unsigned int i = 0; i < chromScale.size(); i++) {
		if (chromScale[i] == root) startPos = i;
	}
	
	//Search alt chromatic scale
	if (startPos == -1) {
		alt = true;
		for (unsigned int i = 0; i < altChromScale.size(); i++) {
			if (altChromScale[i] == root) startPos = i;
		}
	}
	
	//Var for looping through scale interval vector
	unsigned int id = 0;

	//var for holding current note, to be pushed to vector
	std::string tempNote = root;

	//Var for holding position on Chromatic scale
	int z = startPos;
	
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
		currentScale.push_back(tempNote);
		
		id++;		
	}
	
	if (currentScale.at(0).length() < 2) fixScale();
	else if (currentScale.at(0).at(1) == '#') fixScaleSharp();
	else if (currentScale.at(0).at(1) == 'b') fixScaleFlat();
}

/*************************************************************************************
** Function: fixScale
** Description: Set output notes so every note is represented (A through G) instead
**			 of note names on keyboard.
*************************************************************************************/
void Scale::fixScale() {
	
	std::vector<std::string> outScale = currentScale;

	//If less than 7 notes, this won't work, so just return it.
	if (outScale.size() < 6) currentScaleFixed = currentScale;

	//Get root note
	unsigned int stop = 0;
	std::string root = outScale.at(0);

	//Find position position where ascending scale "peaks"
	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	//Base scale
	std::vector<std::string> aToG = {"A", "B", "C", "D", "E", "F", "G" };

	int aToGPos = 0;

	//Set aToGPos to position of root note on the aToG scale
	for (unsigned int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;
	}

	//Loop through vector of notes until we get back to the root note (descending half corrected later)
	for (unsigned int i = 1; i < stop; i++) {
		
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
	for (unsigned int i = outScale.size()-2; i > stop; i--) {
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
	
	currentScaleFixed = outScale;
}

/*************************************************************************************
** Function: fixScaleSharp
** Description: Set output notes so every note is represented (A through G) instead
**			 of note names on keyboard.
*************************************************************************************/
void Scale::fixScaleSharp() {
	
	//Prepare scale for "fixing"
	std::vector<std::string> outScale = currentScale;

	//If less than 7, this won't work, so just return it.
	if (outScale.size() < 6) currentScaleFixed = currentScale;


	unsigned int stop = 0;
	//Set root note
	std::string root = outScale.at(0);

	//Find position position where ascending scale "peaks"
	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	//Base scale
	std::vector<std::string> aToG = { "A#", "B", "C#", "D#", "E", "F#", "G#" };

	//Set aToGPos to position of root note on the aToG scale
	int aToGPos = 0;
	for (unsigned int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;

	}

	//Loop through vector of notes until we get back to the root note (descending half corrected later)
	for (unsigned int i = 1; i < stop; i++) {

		int len = outScale.at(i).length();

		//Wrap around scale while incrementing and set swap equal to next note
		std::string swap = aToG.at((aToGPos + i) % 7);

		//If next note does not equal note in scale vector...
		if (outScale.at(i).at(0) != swap.at(0)) {
		
			outScale.at(i) = swap;
			
			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "#";
			}
			
		}
	}

	for (unsigned int i = outScale.size() - 2; i > stop; i--) {
		aToGPos++;
		int len = outScale.at(i).length();
		
		if (aToGPos > 6) aToGPos -= 7;

		std::string swap = aToG.at(aToGPos);
		
		if (outScale.at(i).at(0) != swap.at(0)) {

			outScale.at(i) = swap;

			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "#";
			}
			
		}
	}

	currentScaleFixed = outScale;
}

/*************************************************************************************
** Function: fixScaleFlat
** Description: Set output notes so every note is represented (A through G) instead
**			 of note names on keyboard.
*************************************************************************************/
void Scale::fixScaleFlat() {
	std::vector<std::string> outScale = currentScale;

	if (outScale.size() < 6) currentScaleFixed = currentScale;

	unsigned int stop = 0;
	std::string root = outScale.at(0);

	int i = 0;
	while (!stop) {
		i++;
		if (outScale[i] == root) stop = i;
	}

	std::vector<std::string> aToG = { "Ab", "Bb", "C", "Db", "Eb", "F", "Gb" };

	int aToGPos = 0;
	for (unsigned int i = 0; i < aToG.size(); i++) {
		if (aToG.at(i) == root) aToGPos = i;

	}

	for (unsigned int i = 1; i < stop; i++) {
		int len = outScale.at(i).length();
		std::string swap = aToG.at((aToGPos + i) % 7);
		if (outScale.at(i).at(0) != swap.at(0)) {

			outScale.at(i) = swap;

			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "b";
			}

		}
	}

	for (unsigned int i = outScale.size() - 2; i > stop; i--) {
		aToGPos++;
		int len = outScale.at(i).length();

		if (aToGPos > 6) aToGPos -= 7;

		std::string swap = aToG.at(aToGPos);

		if (outScale.at(i).at(0) != swap.at(0)) {

			outScale.at(i) = swap;

			if (len > 0) {
				outScale.at(i) = outScale.at(i) + "b";
			}
			

		}
	}

	currentScaleFixed = outScale;
}	

/*************************************************************************************
** Function: makeChord
** Description: Makes a chord using the root note (an int) passed to the function
**				and the intervals (a vector of ints) to find each note in the
**				chord. (Loops starts at indicated root note and then jumps to
**				the next note based on each interval.)
*************************************************************************************/
void Scale::makeChord(int rootNote, std::vector<int> intervals){

	currentChord.clear();

	int pos = rootNote;

	for(int i = 0; i < intervals.size(); i++){
		pos = (pos + intervals[i]) % 7;
		currentChord.push_back(currentScale[pos]);
	}
}

   
