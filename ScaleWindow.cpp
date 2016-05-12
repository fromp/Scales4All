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
	((ScaleWindow*)v)->getScale(((ScaleWindow*)v)->modeMenu->value(), ((ScaleWindow*)v)->rootMenu->value());
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
	buffer->text(((ScaleWindow *)v)->fullscale);
	((ScaleWindow*)v)->out->buffer(buffer);
	((ScaleWindow*)v)->chordRootMenu->value(0);
}

/*************************************************************************************
** Function: putChordInTextBox
** Description: Create current scale and write it to fullscale c string, pass
**				fullscale to buffer and print to display text box.
*************************************************************************************/
void putChordInTextBox(Fl_Widget * o, void *v){
	((ScaleWindow*)v)->getChord(((ScaleWindow*)v)->chordMenu->value(), ((ScaleWindow*)v)->chordRootMenu->value());
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
	buffer->text(((ScaleWindow *)v)->fullscale);
	((ScaleWindow*)v)->out->buffer(buffer);
}

void addChordToProgression(Fl_Widget * o, void *v){
	if( ((ScaleWindow*)v)->scale.isCurrentChordEmpty() )
		return;
	
	((ScaleWindow*)v)->scale.addToProgression();
	((ScaleWindow*)v)->showProgression();
	((ScaleWindow*)v)->fillProgressionChoice();
}

void removeChordFromProgression(Fl_Widget * o, void *v){
	int menuChoice = ((ScaleWindow *)v)->progressionMenu->value();

	if(menuChoice == 0){
		return;
	}
	
	menuChoice--;
	((ScaleWindow *)v)->scale.removeFromProgression(menuChoice);	
	((ScaleWindow *)v)->fillProgressionChoice();
	((ScaleWindow *)v)->showProgression();
	((ScaleWindow *)v)->progressionMenu->value(0);
}
ScaleWindow::ScaleWindow(int w, int h, const char * title):Fl_Window(w, h, title){
	begin();
	modeMenu = new Fl_Choice(10, 10, 150, 30);
	modeMenu->add("Select mode");
	for(unsigned int i = 0; i < 11; i++){
		modeMenu->add(modes[i]);
	}
	modeMenu->value(0);
	
	rootMenu = new Fl_Choice(180, 10, 110, 30);
	rootMenu->add("Select root");
	for(unsigned int i = 0; i < 17; i++){
		rootMenu->add(roots[i]);
	}
	rootMenu->value(0);

	chordMenu = new Fl_Choice(10, 50, 150, 30);
	chordMenu->add("Select chord");
	for(unsigned int i = 0; i < 6; i++){
		chordMenu->add(chords[i]);
	}
	chordMenu->value(0);

	chordRootMenu = new Fl_Choice(180, 50, 110, 30);
	chordRootMenu->add("Chord root");
	chordRootMenu->value(0);

	scale.loadScales();

	startBut = new Fl_Button(300, 10, 100, 30, "&Show scale");
	startBut->callback(putScaleInTextBox, this);
	
	chordBut = new Fl_Button(300, 50, 100, 30, "&Show chord");
	chordBut->callback(putChordInTextBox, this);

	quitBut = new Fl_Button(10, 500, 560, 30, "&Quit");
	quitBut->callback(cb_quit, this);

	out = new Fl_Text_Display(10, 100, 560, 150);
	out->textfont(FL_COURIER);

	addChordBut = new Fl_Button(10, 260, 150, 40, "Add chord to\nprogression");
	addChordBut->callback(addChordToProgression, this);

	progressionMenu = new Fl_Choice(180, 260, 150, 40);
	progressionMenu->add("Remove chord");
	progressionMenu->value(0);

	removeChordBut = new Fl_Button(340, 260, 150, 40, "Remove chord from\nprogression");
	removeChordBut->callback(removeChordFromProgression, this);

	outProg = new Fl_Text_Display(10, 310, 560, 180);
	outProg->textfont(FL_COURIER);

	fullscaleLocation = 0;
	fullscaleMax = 500;

	end();
	show();
}

void ScaleWindow::cb_quit(Fl_Widget*, void *v){
	((ScaleWindow*)v)->cb_quit_i();
}

void ScaleWindow::cb_quit_i(){
	hide();
}

void ScaleWindow::getScale(int mode, int root){	

	for(int i = 0; i < fullscaleMax; i++) fullscale[i] = '\0';	

	fullscaleLocation = 0;
	if(mode == 0 || root == 0){ 
		std::string errorMessage = "Please select a mode and a root note.";
		for(int i = 0; i < errorMessage.length(); i++){
			fullscale[fullscaleLocation] = errorMessage[i];
			fullscaleLocation++;
		}
		return;
	}

	std::string	modeString = modes[mode-1];
	std::string rootString = roots[root-1];
	
	for(unsigned int i = 0; i < modeString.length(); i++){
		modeString[i] = tolower(modeString[i]);
		if(modeString[i] == ' ') modeString[i] = '_';
	}

	scale.makeScale(rootString, modeString);
	std::vector<std::string> curScale = scale.getCurrentScale();
	std::vector<std::string> curScaleFixed = scale.getCurrentScaleFixed();

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
	fullscale[fullscaleLocation] = '\n';
	fullscaleLocation++;

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

	for(int i = 0; i < 7; i++){
		char newNote[4] = "\0\0\0";
	   	for(int j = 0; j < curScale[i].length(); j++)
			newNote[j] = curScale[i][j];
		chordRootMenu->add(newNote);
	};

	scale.clearCurrentChord();
}

int ScaleWindow::getChord(int chord, int chordRoot){
	scale.clearCurrentChord();
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

	for(int i = 0; i < fullscaleMax; i++)
		fullscale[121 + i] = '\0';

	chord--;
	chordRoot--;
	
	fullscaleLocation = 121;

	fullscale[fullscaleLocation] = '\n';
	fullscaleLocation++;
	
	int pos = chordRoot - 1;
	
	for(int i = 0; i < chordNames[chord].length(); i++){
		fullscale[fullscaleLocation] = chordNames[chord][i];
		fullscaleLocation++;
	}
	fullscale[fullscaleLocation] = '\n';
	fullscaleLocation++;


	scale.makeChord(chordRoot, chordIntervals[chord]);
	std::vector<std::string> curChord = scale.getCurrentChord();

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

void ScaleWindow::printErrorInProgression(){
	
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
	std::vector<std::vector<std::string> > prog;
	prog = scale.getProgression();
	char progChar[2000];
	for(int i = 0; i < 2000; i++)
		progChar[i] = '\0';
	int pos = 0;
	int max = 0; 
	for(int i = 0; i < prog.size(); i++){
		if(prog[i].size() > max) max = prog[i].size();
	}

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
			else{
				progChar[pos] = ' ';
				pos++;
				curLen = 1;
			}
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

	std::string errorMessage = "You must select a chord before adding to progression.";

	for(int i = 0; i < errorMessage.length(); i++){
		progChar[pos] = errorMessage[i];
		pos++;
	}

	buffer->text(progChar);
	outProg->buffer(buffer);
}

void ScaleWindow::fillProgressionChoice(){
	
	std::vector< std::vector<std::string> > prog = scale.getProgression();

	if(progressionMenu->size() > 1){
		for(int i = progressionMenu->size() - 1; i > 0; i--){
			progressionMenu->remove(i);
		}
	}

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

void ScaleWindow::showProgression(){
	if(scale.isCurrentChordEmpty() == true){
		printErrorInProgression();	
		return;
	}
	
	Fl_Text_Buffer *buffer = new Fl_Text_Buffer();
	std::vector<std::vector<std::string> > prog;
	prog = scale.getProgression();
	char progChar[2000];
	for(int i = 0; i < 2000; i++)
		progChar[i] = '\0';
	int pos = 0;
	int max = 0; 
	for(int i = 0; i < prog.size(); i++){
		if(prog[i].size() > max) max = prog[i].size();
	}

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
			else{
				progChar[pos] = ' ';
				pos++;
				curLen = 1;
			}
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

	buffer->text(progChar);
	outProg->buffer(buffer);

}
