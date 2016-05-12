#ifndef SCALEWINDOW_HPP
#define SCALEWINDOW_HPP

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Text_Display.H>
#include "Scale.hpp"
#include <vector>
#include <string>
	
class ScaleWindow : public Fl_Window{
	public:
		ScaleWindow(int w, int h, const char * title);
		void getScale(int mode, int root);
		int getChord(int chord, int chordRoot);
		void printErrorInProgression();
		void fillProgressionChoice();
		void showProgression();
		Fl_Choice * modeMenu;
		Fl_Choice * rootMenu;
		Fl_Choice * chordMenu;
		Fl_Choice * chordRootMenu;
		Fl_Choice * progressionMenu;
		Fl_Button * startBut;
		Fl_Button * quitBut;
		Fl_Button * chordBut;
		Fl_Button * addChordBut;
		Fl_Button * removeChordBut;
		Fl_Text_Display * out;
		Fl_Text_Display * outProg;
		char fullscale[500];
		int fullscaleLocation;
		int fullscaleMax;
		Scale scale;

	private:
		static void cb_quit(Fl_Widget *, void*);
		inline void cb_quit_i();
};

#endif
