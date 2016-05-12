all:
	g++ -std=c++11 Scales4All.cpp Scale.hpp Scale.cpp ScaleWindow.hpp ScaleWindow.cpp -o Scales -lfltk

clean:
	rm Scales 
