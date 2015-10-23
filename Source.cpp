#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include "Scale.hpp"


using namespace std;

/*************************************************************************************
** Function: check
** Most recent date modified: 2015/9/23
** Description: Check that the root note input is 2 or less chars long and can be 
**			 found in the chromatic scale (or alternative chromatic scale)
**			 designated by the program. (Currently B#, E#, Cb, and Fb are not
**			 supported for root notes.
*************************************************************************************/
bool check(string input) {
	if (input.length() > 2) return false;
	vector<string> chromScale = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
	vector<string> altChromScale = { "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" };
	for (int i = 0; i < chromScale.size(); i++)
		if (input == chromScale[i]) return true;
	for (int i = 0; i < altChromScale.size(); i++)
		if (input == altChromScale[i]) return true;

	return false;
}

/*************************************************************************************
** Function: checkNames
** Most recent date modified: 2015/9/23
** Description: Check that the scale/mode selection is vaild (must be number between
**			 1 and 11).
*************************************************************************************/
bool checkNames(string *scaleName, vector<string> allScaleNames) {
	string stringN = *scaleName;
	if (stringN.length() < 1) return false;
	if (toupper(stringN[0]) == 'Q') {
		*scaleName = 'Q';
		return true;
	}
	for (int i = 0; i < stringN.length(); i++)
		if (!isdigit(stringN[i])) return false;
	int choice = atoi(stringN.c_str());
	if (choice < 0 || choice > 11) return false;
	*scaleName = allScaleNames.at(choice - 1);
	return true;


}


int main() {

	//Make scale object
	Scale scale;

	//While stop is false, main will loop
	bool stop = false;

	//Get scale information from txt file
	scale.loadScales();

	//Get scale names from scale object (will later support user-generated scales/scale names)
	vector<string> allScaleNames = scale.getScaleNames();
	
	//Welcome message
	cout << "\tWelcome to Scale4All" << endl << endl;
	cout << "Give me a mode and a root note, and I will give you all the notes in the scale!" << endl << endl;

	
	while (!stop) {

		string scaleName = "";

		cout << "1. Major, 2. Natural Minor, 3. Harmonic minor, 4. Melodic minor, 5. Ioanian," << endl;
		cout << "6. Dorian, 7. Phrygian, 8. Lydian, 9. Mixolydian, 10. Aeolian, 11. Locrian, " << endl;
		cout << "Enter Q to stop." << endl << endl;

		//Get scale
		do {
			cout << "Please enter the number corresponding to the desired mode listed above." << endl;
			getline(cin, scaleName);
			if (!checkNames(&scaleName, allScaleNames)) scaleName = "";
		} while (scaleName == "");

		//If we haven't received the quit command, get root note and print scales
		if (toupper(scaleName[0]) != 'Q') {

			//Input var for root note
			string input = "";

			//Print root note information
			do {
				cout << "Choose a root note using A through G, with '#' for sharps and 'b' for flats." << endl;
				cout << "Please that the following notes cannot be used: 'B#', 'E#', 'Cb', and 'Fb'" << endl;
				getline(cin, input);

				//Make input upper case for verification function
				input[0] = toupper(input[0]);
				if (!check(input)) input = "";

			} while (input == "");


			vector<string> myScale;

			//Make scale in scale object (will be save in current scale var)
			scale.makeScale(input, scaleName);
			
			//Get scale that uses names of piano keyes
			myScale = scale.getCurrentKey();

			//Print scale 
			cout << "Scale (based on piano keys): " << endl;
			for (int i = 0; i < myScale.size(); i++) {
				cout << setw(4) << left << myScale.at(i);
			}
			cout << endl;

			//Get scale that uses all notes ("fixed scale")
			myScale = scale.getCurrentFixed();
			//Print scale
			cout << "Scale (based on note names): " << endl;
			for (int i = 0; i < myScale.size(); i++) {
				cout << setw(4) << left << myScale.at(i);
			}
			
			cout << endl << endl << "Press enter to continue...";
			cin.get();

			/*
			cout << endl << "keys" << endl;

			scale.makeChords();
			vector<ChordsFromNotes> keyChords = scale.getChordsKeys();

			for (int i = 0; i < keyChords.size(); i++) {
				cout << "Root: " << keyChords.at(i).RootNote << endl;
				for (int z = 0; z < keyChords.at(i).chordsFromOneNote.size(); z++) {
					cout << keyChords.at(i).chordsFromOneNote.at(z).name << endl;
					for (int y = 0; y < keyChords.at(i).chordsFromOneNote.at(z).notesInChord.size(); y++) {
						//for (int y = 0; y < 2; y++) {
						cout << keyChords.at(i).chordsFromOneNote.at(z).notesInChord.at(y) << " ";
					}
					cout << endl;
				}
			}

			cout << "fixed" << endl;

			keyChords = scale.getChordsFixed();

			for (int i = 0; i < keyChords.size(); i++) {
				cout << "Root: " << keyChords.at(i).RootNote << endl;
				for (int z = 0; z < keyChords.at(i).chordsFromOneNote.size(); z++) {
					cout << keyChords.at(i).chordsFromOneNote.at(z).name << endl;
					for (int y = 0; y < keyChords.at(i).chordsFromOneNote.at(z).notesInChord.size(); y++) {
						//for (int y = 0; y < 2; y++) {
						cout << keyChords.at(i).chordsFromOneNote.at(z).notesInChord.at(y) << " ";
					}
					cout << endl;
				}
			}

			cout << endl;

			cout << endl;

			for (int x = 0; x < keyChords.size(); x++) {
				cout << keyChords.at(x).RootNote << " ";
			}
			cout << endl;

			cout << "Root note name: ";
			getline(cin, input);

			for (int x = 0; x < keyChords.size(); x++) {
				if (input == keyChords.at(x).RootNote) {
					for (int z = 0; z < keyChords.at(x).chordsFromOneNote.size(); z++) {
						cout << keyChords.at(x).chordsFromOneNote.at(z).name << endl;
						for (int y = 0; y < keyChords.at(x).chordsFromOneNote.at(z).notesInChord.size(); y++) {
							//for (int y = 0; y < 2; y++) {
							cout << keyChords.at(x).chordsFromOneNote.at(z).notesInChord.at(y) << " ";
						}
						cout << endl;
					}
				}
			}*/

		}

		system("cls");

		if (scaleName == "Q") stop = true;
	}
	
	cout << "Thank you for using Scales4All! Have a great day!" << endl;
	cout << "Press enter to quit." << endl;

	cin.get();


	return false;
}