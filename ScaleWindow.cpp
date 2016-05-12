#include "ScaleWindow.hpp"

/*************************************************************************************
** Constants
** Description: Constants for creating scales/chords
*************************************************************************************/
const std::vector<std::string> chordNames = {"Triad", "Fifth", "Seventh", "Ninth", "Eleventh", "Thirteenth"};

const std::vector<std::vector<int> > chordIntervals = { {0, 2, 2}, {0, 2, 2, 2}, {0, 2, 2, 2, 2}, {0, 2, 2, 2, 2, 2}, {0, 2, 2, 2, 2, 2, 2}, {0, 2, 2, 2, 2, 2, 2, 2} };

const char * modes[] = {"Major", "Natural Minor", "Harmonic Minor", "Melodic Minor", "Ioanian", "Dorian", "Phrygian", "Lydian", "Mixolydian", "Aeolian", "Locrian"};

const char * roots[] = {"Ab", "A", "A#", "Bb", "B", "C", "C#", "Db", "D", "D#", "Eb", "E", "F", "F#", "Gb", "G", "G#" };

const char * chords[] = {"Triad", "Fifth", "Seventh", "Ninth", "Eleventh", "Thirteenth"};

/*************************************************************************************
** Function: putScaleInTextBox
** Description: Create current scale and write it to fullscale c string, pass
**				fullscale to buffer and print to display text box.
*************************************************************************************/
void putScaleInTextBox(Fl_Widget * o, void *v){
	//Get the scale using the ScaleWindow's scale object
	((ScaleWindow*)v)->getScale(((ScaleWindow*)v)->modeMenu->value(), ((ScaleWindow*)v)->rootMenu->value());
	//Get a buffer so we can print in the display box
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
	//Set the buffer to use the fullscale char string
	buffer->text(((ScaleWindow *)v)->fullscale);
	//Print the buffer
	((ScaleWindow*)v)->out->buffer(buffer);
	//Reset the chrord root menu and chord menu selections to zero
	((ScaleWindow*)v)->chordRootMenu->value(0);	
	((ScaleWindow*)v)->chordMenu->value(0);
	((ScaleWindow*)v)->scale.clearCurrentChord();
}

/*************************************************************************************
** Function: putChordInTextBox
** Description: Create current scale and write it to fullscale c string, pass
**				fullscale to buffer and print to display text box.
*************************************************************************************/
void putChordInTextBox(Fl_Widget * o, void *v){
	//Get the chord based on the chord meu and chord root menu selections
	//using the ScaleWindow's scale object
	((ScaleWindow*)v)->getChord(((ScaleWindow*)v)->chordMenu->value(), ((ScaleWindow*)v)->chordRootMenu->value());
	//Prepare the text with fullscale c-string and the buffer
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
	buffer->text(((ScaleWindow *)v)->fullscale);
	//Print the buffer
	((ScaleWindow*)v)->out->buffer(buffer);
}

/*************************************************************************************
** Function: addChordToProgression
** Description: Adds the current chord to the chord progression 
*************************************************************************************/
void addChordToProgression(Fl_Widget * o, void *v){
	//Make sure the current chord is not empty
	if( ((ScaleWindow*)v)->scale.isCurrentChordEmpty() ){
		((ScaleWindow*)v)->printErrorInProgression();
		return;
	}

	//Add chord to progression using the ScaleWindow's scale object	
	((ScaleWindow*)v)->scale.addToProgression();
	//Display new progression and reset progression menu
	((ScaleWindow*)v)->showProgression();
	((ScaleWindow*)v)->fillProgressionChoice();
}

/*************************************************************************************
** Function: removeChordFromProgression
** Description: Removes the selected chord from the chord progression 
*************************************************************************************/
void removeChordFromProgression(Fl_Widget * o, void *v){
	
	//Get the selected item...	
	int menuChoice = ((ScaleWindow *)v)->progressionMenu->value();
	//...and make sure it's not the first (non-choice) selection
	if(menuChoice == 0){
		return;
	}

	//Adjust placement to correspond to selection's position in the vector	
	menuChoice--;
	//Remove the chord using WindowScale's scale object
	((ScaleWindow *)v)->scale.removeFromProgression(menuChoice);
	
	//Reset and redisplay the menu and progression display
	((ScaleWindow *)v)->fillProgressionChoice();
	((ScaleWindow *)v)->showProgression();
	((ScaleWindow *)v)->progressionMenu->value(0);
}

/*************************************************************************************
** Function: ScaleWindow
** Description: This is the constructor for the ScaleWindow object 
*************************************************************************************/
ScaleWindow::ScaleWindow(int w, int h, const char * title):Fl_Window(w, h, title){
	begin();
	
	//Set up mode menu
	modeMenu = new Fl_Choice(10, 10, 150, 30);
	modeMenu->add("Select mode");
	//Fill menu with the mode names
	for(unsigned int i = 0; i < 11; i++){
		modeMenu->add(modes[i]);
	}
	//Set default
	modeMenu->value(0);

	//Set up root menu
	rootMenu = new Fl_Choice(180, 10, 110, 30);
	rootMenu->add("Select root");
	//Fill it with the variou roots available
	for(unsigned int i = 0; i < 17; i++){
		rootMenu->add(roots[i]);
	}
	//Set default
	rootMenu->value(0);

	//Set up chord menu
	chordMenu = new Fl_Choice(10, 50, 150, 30);
	chordMenu->add("Select chord");
	//Fill it with the various chord types
	for(unsigned int i = 0; i < 6; i++){
		chordMenu->add(chords[i]);
	}
	//Set default
	chordMenu->value(0);

	//Set up the chord root menu
	//This will be filled once a scale has been created
	chordRootMenu = new Fl_Choice(180, 50, 110, 30);
	chordRootMenu->add("Chord root");
	chordRootMenu->value(0);

	//Load the scales data using WindowScale's scale object
	scale.loadScales();

	//Add button to get selected scale
	startBut = new Fl_Button(300, 10, 100, 30, "&Show scale");
	startBut->callback(putScaleInTextBox, this);

	//Add button to get selected chord	
	chordBut = new Fl_Button(300, 50, 100, 30, "&Show chord");
	chordBut->callback(putChordInTextBox, this);

	//Add a button to quit
	quitBut = new Fl_Button(10, 500, 560, 30, "&Quit");
	quitBut->callback(cb_quit, this);

	//out is the display box for the scale and current chord
	out = new Fl_Text_Display(10, 100, 560, 150);
	out->textfont(FL_COURIER);

	//Button to add current chord to the current progression
	addChordBut = new Fl_Button(10, 260, 150, 40, "Add chord to\nprogression");
	addChordBut->callback(addChordToProgression, this);

	//A menu for selecting a chord to remove from the progression
	progressionMenu = new Fl_Choice(180, 260, 150, 40);
	progressionMenu->add("Remove chord");
	progressionMenu->value(0);

	//Button for removing selected chord from progression
	removeChordBut = new Fl_Button(340, 260, 150, 40, "Remove chord from\nprogression");
	removeChordBut->callback(removeChordFromProgression, this);

	//Display box for chord progression
	outProg = new Fl_Text_Display(10, 310, 560, 180);
	outProg->textfont(FL_COURIER);

	//Set fullscale current location and max size
	fullscaleLocation = 0;
	fullscaleMax = 500;

	end();
	show();
}

//Simple function to call quit function
void ScaleWindow::cb_quit(Fl_Widget*, void *v){
	((ScaleWindow*)v)->cb_quit_i();
}

//Quit the program!
void ScaleWindow::cb_quit_i(){
	hide();
}

/*************************************************************************************
** Function: getScale
** Description: This is a function to get the selected scale and add it to
**				fullscale so it can be printed in the display (using the buffer)
*************************************************************************************/
void ScaleWindow::getScale(int mode, int root){	

	//Clear out and reset fullscale and current chord
	for(int i = 0; i < fullscaleMax; i++) fullscale[i] = '\0';
	fullscaleLocation = 0;

	scale.clearCurrentChord();

	//Make sure both a mode and root are selected and print error if not
	if(mode == 0 || root == 0){ 
		std::string errorMessage = "Please select a mode and a root note.";
		for(int i = 0; i < errorMessage.length(); i++){
			fullscale[fullscaleLocation] = errorMessage[i];
			fullscaleLocation++;
		}
		return;
	}

	//Get the name of the mode and the root note
	std::string	modeString = modes[mode-1];
	std::string rootString = roots[root-1];

	//Convert mode string to match what the scale object expects to receive
	for(unsigned int i = 0; i < modeString.length(); i++){
		modeString[i] = tolower(modeString[i]);
		if(modeString[i] == ' ') modeString[i] = '_';
	}

	//Call make scale with scale object, which will create/prepare
	//currentScale and currentScaleFixe
	scale.makeScale(rootString, modeString);
	std::vector<std::string> curScale = scale.getCurrentScale();
	std::vector<std::string> curScaleFixed = scale.getCurrentScaleFixed();

	//Add the current scale to the fullscale c-string, with up to 3 spaces between notes 
	//For notes without a sharp or flat, 3 spaces
	//For notes with a single sharp or flat, 2 spaces
	//For notes with two sharps or flats, 1 space
	for(unsigned int i = 0; i < curScale.size(); i++){
		for(unsigned int j = 0; j < curScale[i].length(); j++){
			fullscale[fullscaleLocation] = curScale[i][j];
			fullscaleLocation++;
		}
		if(fullscale[fullscaleLocation-1] != '#' && fullscale[fullscaleLocation-1] != 'b'){
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
		}
		else if(fullscale[fullscaleLocation-2] != '#' && fullscale[fullscaleLocation-2] != 'b'){
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
		}
		fullscale[fullscaleLocation] = ' ';
		fullscaleLocation++;
	}
	//Add newline and then get the fixed scale
	fullscale[fullscaleLocation] = '\n';
	fullscaleLocation++;

	//Add the current fixed scale to the fullscale c-string, with up to 3 spaces between notes 
	//For notes without a sharp or flat, 3 spaces
	//For notes with a single sharp or flat, 2 spaces
	//For notes with two sharps or flats, 1 space
	for(unsigned int i = 0; i < curScaleFixed.size(); i++){
		for(unsigned int j = 0; j < curScaleFixed[i].length(); j++){
			fullscale[fullscaleLocation] = curScaleFixed[i][j];
			fullscaleLocation++;
		}
		if(fullscale[fullscaleLocation-1] != '#' && fullscale[fullscaleLocation-1] != 'b'){
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
		}
		else if(fullscale[fullscaleLocation-2] != '#' && fullscale[fullscaleLocation-2] != 'b'){
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
		}
		fullscale[fullscaleLocation] = ' ';
		fullscaleLocation++;
	}

	if(chordRootMenu->size() > 1){
		for(int i = chordRootMenu->size() - 1; i > 0; i--){
			chordRootMenu->remove(i);
		}
	}

	//Clear chord root menu
	for(int i = 0; i < 7; i++){
		char newNote[4] = "\0\0\0";
	   	for(int j = 0; j < curScale[i].length(); j++)
			newNote[j] = curScale[i][j];
		chordRootMenu->add(newNote);
	};

	//Clear current chord
	scale.clearCurrentChord();
}

/*************************************************************************************
** Function: getChord
** Description: This function gets a chord based on selected values, sets it as
**				the currentChord in the 
*************************************************************************************/
int ScaleWindow::getChord(int chord, int chordRoot){
	//Clear currentChord so we can make a new one	
	scale.clearCurrentChord();
	
	//Make sure we have selected actual options and print errors if we haven't
	if(chord == 0 || chordRoot == 0){
		std::string errorMessage = "\nPlease select both a chord type and root note.";
		if(fullscaleLocation > 120) fullscaleLocation = 121;
		else {
			fullscaleLocation = 0;
			errorMessage = "Please select a mode and a root note." + errorMessage;
		}
		for(int i = 0; i < errorMessage.length(); i++){
			fullscale[fullscaleLocation] = errorMessage[i];
			fullscaleLocation++;
		}	
		while(fullscaleLocation < fullscaleMax){
			fullscale[fullscaleLocation] = '\0';
			fullscaleLocation++;
		}	
		return 0;
	}

	//Clear everything after the scales so we can append the currentChord
	for(int i = 0; i < fullscaleMax; i++)
		fullscale[121 + i] = '\0';

	//Adjust chord and chordRoot selection to match positions in vectors
	chord--;
	chordRoot--;

	//Reset current location in fullscale, add a newline, increment
	//current location in fullscale
	fullscaleLocation = 121;
	fullscale[fullscaleLocation] = '\n';
	fullscaleLocation++;

	//Add name of chord type (triad, etc.) to fullscale
	for(int i = 0; i < chordNames[chord].length(); i++){
		fullscale[fullscaleLocation] = chordNames[chord][i];
		fullscaleLocation++;
	}
	fullscale[fullscaleLocation] = '\n';
	fullscaleLocation++;

	//Actually create the chord using the scale object and the selected values
	scale.makeChord(chordRoot, chordIntervals[chord]);
	//And then get the chord so we can print it!
	std::vector<std::string> curChord = scale.getCurrentChord();

	//Add the chord to fullscale so it can be printed (including spaces
	//too keep everything aligned)
	for(int i = 0; i < curChord.size(); i++){
		for(int j = 0; j < curChord[i].length(); j++){	
			fullscale[fullscaleLocation] = curChord[i][j];
			fullscaleLocation++;
		}
		if(fullscale[fullscaleLocation-1] != '#' && fullscale[fullscaleLocation-1] != 'b'){
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
		}
		else if(fullscale[fullscaleLocation-2] != '#' && fullscale[fullscaleLocation-2] != 'b'){
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
			fullscale[fullscaleLocation] = ' ';
			fullscaleLocation++;
		}
	}

	return 1;
}

/*************************************************************************************
** Function: printErrorInProgression
** Description: This function is called when there's an error in adding a chord
**				to the chord progression. It adds an error message to the output
**				c-string which is printed in the text display
*************************************************************************************/
void ScaleWindow::printErrorInProgression(){
	//Create a buffer to printing
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();

	//Get the current progression so we can add it to the c-string
	//before addiging the error message
	std::vector<std::vector<std::string> > prog;
	prog = scale.getProgression();

	//Clear c-string
	char progChar[2000];
	for(int i = 0; i < 2000; i++)
		progChar[i] = '\0';

	//Find out which chord in the progression has the most notes
	//in it (since we're printing them vertically)
	int pos = 0;
	int max = 0; 
	for(int i = 0; i < prog.size(); i++){
		if(prog[i].size() > max) max = prog[i].size();
	}

	//Loop through the progression, getting the first, second, third, etc.
	//note in each chord and printing them row by row.
	//So the first note from each chord is printed in the first row,
	//then the second notes in the second row, etc.
	int x = 0;
	while(x < max){
		for(int i = 0; i < prog.size(); i++){
			int curLen = 0;
			if(x < prog[i].size()){
				curLen = prog[i][x].length();
				for(int j = 0; j < curLen; j++){
					progChar[pos] = prog[i][x][j];
					pos++;
				}
			}
			//If the corresponding chord has less than the max
			//number of notes, just print a space for it
			else{
				progChar[pos] = ' ';
				pos++;
				curLen = 1;
			}
			//Pad it out with white space
			while(curLen < 4){
				progChar[pos] = ' ';
				pos++;
				curLen++;
			}
		}
		//Add a new line at the end of each row
		progChar[pos] = '\n';
		pos++;
		x++;
	}

	//Error message to add to the c-string
	std::string errorMessage = "You must select a chord before adding to progression.";

	//Add the message!
	for(int i = 0; i < errorMessage.length(); i++){
		progChar[pos] = errorMessage[i];
		pos++;
	}

	//Print everything
	buffer->text(progChar);
	outProg->buffer(buffer);
}

/*************************************************************************************
** Function: fillProgressionChoice
** Description: This function fills the progression choice menue with the chords
**				currently in the chord progression.
*************************************************************************************/
void ScaleWindow::fillProgressionChoice(){
	//Get current chord progression	
	std::vector< std::vector<std::string> > prog = scale.getProgression();

	//Clear the progression menu
	if(progressionMenu->size() > 1){
		for(int i = progressionMenu->size() - 1; i > 0; i--){
			progressionMenu->remove(i);
		}
	}

	//Fill the menu, including the number corresponding to the chords position
	//in the progression and the first note in the chord
	for(int i = 0; i < prog.size(); i++){
		char newItem[11] = "   -- " ;
		if(i + 1 < 10) newItem[0] = i + 49;
		else{
			newItem[1] = ((i + 1) % 10) + 48;
		   	newItem[0] = ((i + 1) / 10) + 48;
		}
		int j = 0;
		for(; j < prog[i][0].length(); j++)
			newItem[6 + j] = prog[i][0][j];
		newItem[6 + j] = '\0';
		progressionMenu->add(newItem);
	}
}

/*************************************************************************************
** Function: showProgression
** Description: This function gets the current chord progression and displays it
**				in the text display box
*************************************************************************************/
void ScaleWindow::showProgression(){

	//If the current chord is empty, we have a problem. Abort!
	if(scale.isCurrentChordEmpty() == true){
		printErrorInProgression();	
		return;
	}

	//Get a buffer for printing later and vector to hold the progression
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
	std::vector<std::vector<std::string> > prog;

	//Get the progression!
	prog = scale.getProgression();

	//Create and initialize a c-string for printing
	char progChar[2000];
	for(int i = 0; i < 2000; i++)
		progChar[i] = '\0';

	//Find how many notes the chord with the most notes has
	int pos = 0;
	int max = 0; 
	for(int i = 0; i < prog.size(); i++){
		if(prog[i].size() > max) max = prog[i].size();
	}

	//Loop through the progression, getting the first, second, third, etc.
	//note in each chord and printing them row by row.
	//So the first note from each chord is printed in the first row,
	//then the second notes in the second row, etc.
	int x = 0;
	while(x < max){
		for(int i = 0; i < prog.size(); i++){
			int curLen = 0;
			if(x < prog[i].size()){
				curLen = prog[i][x].length();
				for(int j = 0; j < curLen; j++){
					progChar[pos] = prog[i][x][j];
					pos++;
				}
			}
			//If the corresponding chord has less than the max
			//number of notes, just print a space for it
			else{
				progChar[pos] = ' ';
				pos++;
				curLen = 1;
			}
			//Pad with white space!
			while(curLen < 4){
				progChar[pos] = ' ';
				pos++;
				curLen++;
			}
		}
		progChar[pos] = '\n';
		pos++;
		x++;
	}

	//Print the progression!
	buffer->text(progChar);
	outProg->buffer(buffer);

}
