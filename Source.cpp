#include <iostream>
#include <string>
#include <vector>
#include "Scale.hpp"

using namespace std;

bool check(string input) {
	if (input.length() > 2) return false;
	vector<string> chromScale = { "A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#" };
	vector<string> altChromScale = { "A", "Bb", "B", "C", "Db", "D", "Eb", "E", "F", "Gb", "G", "Ab" };
	for (int i = 0; i < chromScale.size(); i++)
		if (input == chromScale[i]) return true;
	for (int i = 0; i < altChromScale.size(); i++)
		if (input == altChromScale[i]) return true;
	if (input == "Q") return true;
	return false;
}

bool checkNames(string scaleName, vector<string> allScaleNames) {
	for (int i = 0; i < allScaleNames.size(); i++) {
		if (scaleName == allScaleNames.at(i)) return true;
	}
	return false;
}

int main() {

	Scale scale;
	bool stop = false;
	scale.loadScales();
	vector<string> allScaleNames = scale.getScaleNames();
	//scale.printScales();
	while (!stop) {
		string scaleName = "";
		cout << "Please enter a scale name." << endl;
		do {
			getline(cin, scaleName);
			if (!checkNames(scaleName, allScaleNames)) scaleName = "";
		} while (scaleName == "");
		cout << "Choose root. Enter Q to stop." << endl;
		string input = "";
		do {
			getline(cin, input);
			if (!check(input)) input = "";
		} while (input == "");

		vector<string> myScale;

		if(input != "Q") scale.makeScale(input, scaleName);
		//vector<ScaleNotes> notes = scale.getCurrentScale();
		/*for (int i = 0; i < notes.size(); i++) {
			cout << notes.at(i).note << endl;
		}*/

		myScale = scale.getCurrentKey();
		cout << "Scale (based on piano keys): ";
		for (int i = 0; i < myScale.size(); i++) {
			cout << myScale.at(i) << " ";
		}
		cout << endl;
		myScale = scale.getCurrentFixed();
		cout << "Scale (based on note names): ";
		for (int i = 0; i < myScale.size(); i++) {
			cout << myScale.at(i) << " ";
		}
		
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
		}



		if (input == "Q") stop = true;
	}

	return false;
}