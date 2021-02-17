//
// impressionist.cpp
//
// The main driver program for the other parts. We have two major components,
// UI and Doc.
// They do have a link to each other as their member such that they can 
// communicate.
//

#include <stdio.h>

#include <FL/Fl.h>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "impressionistUI.h"
#include "impressionistDoc.h"
#include "time.h"
ImpressionistUI *impUI;
ImpressionistDoc *impDoc;

int main(int	argc, 
		 char**	argv) 
{
	srand(time(0));

	impDoc = new ImpressionistDoc();

	// Create the UI
	impUI = new ImpressionistUI();

	// Set the impDoc which is used as the bridge between UI and brushes
	impUI->setDocument(impDoc);
	impDoc->setUI(impUI);

	Fl::visual(FL_DOUBLE|FL_INDEX);

	impUI->show();

// my test
	//Fl_Window* window = new Fl_Window(300, 180);
	//Fl_Box* box = new Fl_Box(20, 40, 260, 100, "Hello World!");
	////box->(FL_UP_BOX);
	//box->labelsize(36);
	//box->labelfont(FL_BOLD);
	//box->labeltype(FL_SHADOW_LABEL);
	//window->end();
	//window->show(argc, argv);

	//// test GL
	//glBegin(GL_POLYGON);
	//glVertex2i(10, 5);
	//glVertex2i(20, 15);
	//glVertex2i(10, 15);
	//glEnd();

	//glFlush();   // don't forget this!
// my test end
	return Fl::run();
}

// global functions
float frand()
{
	return (float)rand()/RAND_MAX;
}

int irand(int max)
{
	return rand()%max;
}
