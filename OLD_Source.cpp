#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
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
	
	return false;
}

bool checkNames(string *scaleName, vector<string> allScaleNames) {
	string stringN = *scaleName;
	if (stringN.length() < 1) return false;
	if (toupper(stringN[0]) == 'Q') return true;
	for (int i = 0; i < stringN.length(); i++)
		if (!isdigit(stringN[i])) return false;
	int choice = atoi(stringN.c_str());
	if (choice < 0 || choice > 11) return false;
	*scaleName = allScaleNames.at(choice - 1);
	return true;


}

int main() {

	Scale scale;
	bool stop = false;
	scale.loadScales();
	vector<string> allScaleNames = scale.getScaleNames();
	cout << "\tWelcome to Scale4All" << endl << endl;
	cout << "Give me a mode and a root note, and I will give you all the notes in the scale!" << endl << endl;
	
	while (!stop) {
		string scaleName = "";
				
		cout << "1. Major, 2. Natural Minor, 3. Harmonic minor, 4. Melodic minor, 5. Ioanian," << endl;
		cout << "6. Dorian, 7. Phrygian, 8. Lydian, 9. Mixolydian, 10. Aeolian, 11. Locrian, " << endl;
		cout << "Enter Q to stop." << endl << endl;
		
		do {
			cout << "Please enter the number corresponding to the desired mode listed above." << endl;
			getline(cin, scaleName);
			if (!checkNames(&scaleName, allScaleNames)) scaleName = "";
		} while (scaleName == "");
		
		if (toupper(scaleName[0]) != 'Q') {
			string input = "";
			do {
				cout << "Choose a root note using A through G, with '#' for sharps and 'b' for flats." << endl;
				cout << "Please that the following notes cannot be used: 'B#', 'E#', 'Cb', and 'Fb'" << endl;
				getline(cin, input);
				input[0] = toupper(input[0]);
				if (!check(input)) input = "";
			} while (input == "");

			cout << endl << input << " " << scaleName << endl;
			cout << "Scale using piano keys:" << endl;
			vector<string> myScale;

			
			myScale = scale.makeScale(input, scaleName);
			cout << "Scale using all the notes: " << endl;
			for (int i = 0; i < myScale.size(); i++) {
				cout << setw(4) << left << myScale.at(i) << " ";
			}
		}
		cout << endl;
		cout << endl;
		if (toupper(scaleName[0]) == 'Q') stop = true;
	}

	cout << endl << endl;
	cout << "Thank you for using Scales4All! Have a great day!" << endl;
	cout << "Press enter to quit." << endl;
	
	cin.get();


	return 0;
}