//
// impressionistUI.h
//
// The user interface part for the program.
//


#include <FL/fl_ask.h>

#include <math.h>
#include <iostream>

#include <thread>

// #include <Eigen/Dense>


#include "impressionistUI.h"
#include "impressionistDoc.h"


/*
//------------------------------ Widget Examples -------------------------------------------------
Here is some example code for all of the widgets that you may need to add to the
project.  You can copy and paste these into your code and then change them to
make them look how you want.  Descriptions for all of the widgets here can be found
in links on the fltk help session page.

//---------Window/Dialog and Menubar-----------------------------------

	//----To install a window--------------------------
	Fl_Window* myWindow = new Fl_Window(600, 300, "MyWindow");
		myWindow->user_data((void*)(this));	// record self to be used by static callback functions

		// install menu bar
		myMenubar = new Fl_Menu_Bar(0, 0, 600, 25);
		Fl_Menu_Item ImpressionistUI::myMenuItems[] = {
			{ "&File",		0, 0, 0, FL_SUBMENU },
				{ "&Load...",	FL_ALT + 'l', (Fl_Callback *)ImpressionistUI::cb_load },
				{ "&Save...",	FL_ALT + 's', (Fl_Callback *)ImpressionistUI::cb_save }.
				{ "&Quit",			FL_ALT + 'q', (Fl_Callback *)ImpressionistUI::cb_exit },
				{ 0 },
			{ "&Edit",		0, 0, 0, FL_SUBMENU },
				{ "&Copy",FL_ALT + 'c', (Fl_Callback *)ImpressionistUI::cb_copy, (void *)COPY },
				{ "&Cut",	FL_ALT + 'x', (Fl_Callback *)ImpressionistUI::cb_cut, (void *)CUT },
				{ "&Paste",	FL_ALT + 'v', (Fl_Callback *)ImpressionistUI::cb_paste, (void *)PASTE },
				{ 0 },
			{ "&Help",		0, 0, 0, FL_SUBMENU },
				{ "&About",	FL_ALT + 'a', (Fl_Callback *)ImpressionistUI::cb_about },
				{ 0 },
			{ 0 }
		};
		myMenubar->menu(myMenuItems);
	m_mainWindow->end();

	//----The window callback--------------------------
	// One of the callbacks
	void ImpressionistUI::cb_load(Fl_Menu_* o, void* v)
	{
		ImpressionistDoc *pDoc=whoami(o)->getDocument();

		char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName() );
		if (newfile != NULL) {
			pDoc->loadImage(newfile);
		}
	}


//------------Slider---------------------------------------

	//----To install a slider--------------------------
	Fl_Value_Slider * mySlider = new Fl_Value_Slider(10, 80, 300, 20, "My Value");
	mySlider->user_data((void*)(this));	// record self to be used by static callback functions
	mySlider->type(FL_HOR_NICE_SLIDER);
	mySlider->labelfont(FL_COURIER);
	mySlider->labelsize(12);
	mySlider->minimum(1);
	mySlider->maximum(40);
	mySlider->step(1);
	mySlider->value(m_nMyValue);
	mySlider->align(FL_ALIGN_RIGHT);
	mySlider->callback(cb_MyValueSlides);

	//----The slider callback--------------------------
	void ImpressionistUI::cb_MyValueSlides(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyValue=int( ((Fl_Slider *)o)->value() ) ;
	}


//------------Choice---------------------------------------

	//----To install a choice--------------------------
	Fl_Choice * myChoice = new Fl_Choice(50,10,150,25,"&myChoiceLabel");
	myChoice->user_data((void*)(this));	 // record self to be used by static callback functions
	Fl_Menu_Item ImpressionistUI::myChoiceMenu[3+1] = {
	  {"one",FL_ALT+'p', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)ONE},
	  {"two",FL_ALT+'l', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)TWO},
	  {"three",FL_ALT+'c', (Fl_Callback *)ImpressionistUI::cb_myChoice, (void *)THREE},
	  {0}
	};
	myChoice->menu(myChoiceMenu);
	myChoice->callback(cb_myChoice);

	//-----The choice callback-------------------------
	void ImpressionistUI::cb_myChoice(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI *)(o->user_data()));
		ImpressionistDoc* pDoc=pUI->getDocument();

		int type=(int)v;

		pDoc->setMyType(type);
	}


//------------Button---------------------------------------

	//---To install a button---------------------------
	Fl_Button* myButton = new Fl_Button(330,220,50,20,"&myButtonLabel");
	myButton->user_data((void*)(this));   // record self to be used by static callback functions
	myButton->callback(cb_myButton);

	//---The button callback---------------------------
	void ImpressionistUI::cb_myButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI* pUI=((ImpressionistUI*)(o->user_data()));
		ImpressionistDoc* pDoc = pUI->getDocument();
		pDoc->startPainting();
	}


//---------Light Button------------------------------------

	//---To install a light button---------------------
	Fl_Light_Button* myLightButton = new Fl_Light_Button(240,10,150,25,"&myLightButtonLabel");
	myLightButton->user_data((void*)(this));   // record self to be used by static callback functions
	myLightButton->callback(cb_myLightButton);

	//---The light button callback---------------------
	void ImpressionistUI::cb_myLightButton(Fl_Widget* o, void* v)
	{
		ImpressionistUI *pUI=((ImpressionistUI*)(o->user_data()));

		if (pUI->myBool==TRUE) pUI->myBool=FALSE;
		else pUI->myBool=TRUE;
	}

//----------Int Input--------------------------------------

	//---To install an int input-----------------------
	Fl_Int_Input* myInput = new Fl_Int_Input(200, 50, 5, 5, "&My Input");
	myInput->user_data((void*)(this));   // record self to be used by static callback functions
	myInput->callback(cb_myInput);

	//---The int input callback------------------------
	void ImpressionistUI::cb_myInput(Fl_Widget* o, void* v)
	{
		((ImpressionistUI*)(o->user_data()))->m_nMyInputValue=int( ((Fl_Int_Input *)o)->value() );
	}

//------------------------------------------------------------------------------------------------
*/

//------------------------------------- Help Functions --------------------------------------------

//------------------------------------------------------------
// This returns the UI, given the menu item.  It provides a
// link from the menu items to the UI
//------------------------------------------------------------
ImpressionistUI* ImpressionistUI::whoami(Fl_Menu_* o)
{
	return ((ImpressionistUI*)(o->parent()->user_data()));
}


//--------------------------------- Callback Functions --------------------------------------------

//------------------------------------------------------------------
// Brings up a file chooser and then loads the chosen image
// This is called by the UI when the load image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_load_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadImage(newfile);
	}
}
void ImpressionistUI::cb_load_another_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAnotherImage(newfile);
	}
}
void ImpressionistUI::cb_load_edge_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadEdgeImage(newfile);
	}
}

void ImpressionistUI::cb_load_alpha_mapped_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadAlphaMappedImage(newfile);
	}
}
void ImpressionistUI::cb_new_mural_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadMuralImage(newfile);
	}
}

void ImpressionistUI::cb_load_dissolve_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->dissolve_image(newfile);
	}
	
}

void ImpressionistUI::cb_load_video(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.avi", NULL);
	if (newfile != NULL) 
		pDoc->loadVideo(newfile);


	Fl::add_timeout(pDoc->avi->getPeriod(), cb_play_video, whoami(o));
}
void ImpressionistUI::cb_play_video(void* p_ui)
{

	ImpressionistDoc* pDoc = ((ImpressionistUI*)(p_ui))->getDocument(); 
	
	if (pDoc->processVideo())
		Fl::repeat_timeout(pDoc->avi->getPeriod(), cb_play_video, p_ui);
	else
		pDoc->avi->readEnd();
}

void ImpressionistUI::cb_load_alpha_image_for_matting(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Open File?", "*.bmp", pDoc->getImageName());
	if (newfile != NULL) {
		pDoc->loadMattingAlphaImage(newfile);
	}
}

void ImpressionistUI::cb_matting(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	pDoc->MattingImage();
	pDoc->m_pUI->m_paintView->refresh();
}



void ImpressionistUI::cb_faded_background_window(Fl_Menu_* o, void* v)
{
	whoami(o)->m_fadedBackgroundWindow->show();
}

void ImpressionistUI::cb_faded_slider(Fl_Widget* o, void* v)
{
	

	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	pUI->fadedRate = double(((Fl_Slider*)o)->value());

	pDoc->generateFadedBackground();
	pDoc->generatemCompositeBitmap
	
	();
	pUI->m_paintView->refresh();
}

void ImpressionistUI::cb_filter_kernel_design_window(Fl_Menu_* o, void* v)
{
	whoami(o)->m_filterKernelDesignWindow->show();
}

int convert_to_int(const char* a) 
{
	int l = strlen(a);
	int r = 0;
	for (int i = 0; i < l; i++)
	{
		r += (a[i] - '0') * pow(10.0, (l - i - 1) * 1.0);
	}
	return r;
}

double convert_to_double(const char* a, int l)
{
	int dot = 0;
	for (dot; dot < l; dot++)
	{
		if (a[dot] == '.')
		{
			break;
		}
	}

	double r = 0.0;
	for (int i = 0; i < dot; i++)
	{
		r += (a[i] - '0') * pow(10.0, (dot - i - 1) * 1.0);
	}
	for (int i = dot + 1; i < l; i++)
	{
		r += (a[i] - '0') * pow(10.0, -(i - dot) * 1.0) * 1.0;
	}

	return r;
}

void ImpressionistUI::cb_filter_size_input(Fl_Widget* o, void* v)
{
	ImpressionistUI* pDoc = ((ImpressionistUI*)(o->user_data()));
	const char* temp = (((Fl_Int_Input*)o)->value());
	int r = convert_to_int(temp);
	if (r % 2 == 0)
	{
		fl_alert("Filter size must be odd!");
		return;
	}
	pDoc->filterSize = r;
}

void ImpressionistUI::cb_filter_weight_input(Fl_Widget* o, void* v)
{
	ImpressionistUI* pDoc = ((ImpressionistUI*)(o->user_data()));
	const char* temp = (((Fl_Input*)o)->value());

	int space_count = 0;
	for (int i = 0; i < strlen(temp); i++)
	{
		if (temp[i] == ' ')
			space_count++;
	}

	if (space_count != pDoc->filterSize * pDoc->filterSize - 1)
	{
		fl_alert("Number of weight does not fit filter size!");
		return;
	}

	if (pDoc->filterWeight) delete[] pDoc->filterWeight;

	pDoc->filterWeight = new double[pDoc->filterSize * pDoc->filterSize];

	int count = 0;
	for (int i = 0; i < strlen(temp); i++)
	{
		int start = i;
		int end = i;
		for (i; temp[i] != ' ' && i < strlen(temp); i++, end++);

		std::cout << start;
		pDoc->filterWeight[count] = convert_to_double(temp + start, end - start);
		count++;
	}

}

void ImpressionistUI::cb_filter_apply(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	pDoc->applyWeightedFilter();
	pUI->m_paintView->refresh();

}

void ImpressionistUI::cb_filter_normalized(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->normalized = bool(((Fl_Check_Button*)o)->value());
}
//------------------------------------------------------------------
// Brings up a file chooser and then saves the painted image
// This is called by the UI when the save image menu item is chosen
//------------------------------------------------------------------
void ImpressionistUI::cb_save_image(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	char* newfile = fl_file_chooser("Save File?", "*.bmp", "save.bmp");
	if (newfile != NULL) {
		pDoc->saveImage(newfile);
	}
}

void ImpressionistUI::cb_mosaic_of_thumbnail_window(Fl_Menu_* o, void* v)
{
	whoami(o)->m_MosaicWindow->show();
}

void ImpressionistUI::cb_mosaic_of_thumbnail_ratioSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->ratio = int(((Fl_Slider*)o)->value());
}


void ImpressionistUI::cb_mosaic_of_thumbnail_alphaSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->alphaOfMosaic = double(((Fl_Slider*)o)->value());

}


void ImpressionistUI::cb_mosaic_of_thumbnail_apply(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();
	pDoc->showMosaicOfThumbnail(pUI->getRatio(), pUI->getAlphaOfMosaic());

}


//-------------------------------------------------------------
// Brings up the paint dialog
// This is called by the UI when the brushes menu item
// is chosen
//-------------------------------------------------------------
void ImpressionistUI::cb_brushes(Fl_Menu_* o, void* v)
{
	whoami(o)->m_brushDialog->show();
}

//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();

	pDoc->clearCanvas();
}

//------------------------------------------------------------
// Causes the Impressionist program to exit
// Called by the UI when the quit menu item is chosen
//------------------------------------------------------------
void ImpressionistUI::cb_exit(Fl_Menu_* o, void* v)
{
	whoami(o)->m_mainWindow->hide();
	whoami(o)->m_brushDialog->hide();

}



//-----------------------------------------------------------
// Brings up an about dialog box
// Called by the UI when the about menu item is chosen
//-----------------------------------------------------------
void ImpressionistUI::cb_about(Fl_Menu_* o, void* v)
{
	// change this to 2021 XD
	fl_message("Impressionist FLTK version for COMP4411, Spring 2021");
}

//------- UI should keep track of the current for all the controls for answering the query from Doc ---------
//-------------------------------------------------------------
// Sets the type of brush to use to the one chosen in the brush 
// choice.  
// Called by the UI when a brush is chosen in the brush choice
//-------------------------------------------------------------
void ImpressionistUI::cb_brushChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int type = (int)v;


	pDoc->setBrushType(type);
}

// callback for line direction choice
void ImpressionistUI::cb_lineDirectionChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int choice = (int)v;

	pDoc->setLineDirectionChoice(choice);
}
//------------------------------------------------------------
// Clears the paintview canvas.
// Called by the UI when the clear canvas button is pushed
//------------------------------------------------------------
void ImpressionistUI::cb_clear_canvas_button(Fl_Widget* o, void* v)
{
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();

	pDoc->clearCanvas();
}


//-----------------------------------------------------------
// Updates the brush size to use from the value of the size
// slider
// Called by the UI when the size slider is moved
//-----------------------------------------------------------
void ImpressionistUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_nSize = int(((Fl_Slider*)o)->value());
}

// line brush width
void ImpressionistUI::cb_lineWidthSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_lineWidth = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_lineAngleSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_lineAngle = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_alphaSlides(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->alpha = double(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_levelSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->level = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_warpStrengthSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->warpStrength = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_swapOriginPaint(Fl_Menu_* o, void* v)
{
	ImpressionistDoc* pDoc = whoami(o)->getDocument();
	unsigned char* temp = pDoc->m_ucBitmap;
	pDoc->m_ucBitmap = pDoc->m_ucPainting;
	pDoc->m_ucPainting = temp;
	whoami(o)->m_paintView->refresh();
	whoami(o)->m_origView->refresh();
}
void ImpressionistUI::cb_undo(Fl_Menu_* o, void* v)
{
	whoami(o)->getDocument()->undo();
}

void ImpressionistUI::cb_auto_paint(Fl_Menu_* o, void* v)
{
	whoami(o)->m_paintView->setEventType(AUTO_PAINT);
	whoami(o)->m_paintView->setEventTrue();
	whoami(o)->m_paintView->refresh();
}
void ImpressionistUI::cb_paintly(Fl_Menu_* o, void* v)
{
	whoami(o)->m_paintlyDialog->show();
}
void ImpressionistUI::cb_show_image_choice(Fl_Menu_* o, void* v)
{
	int choice = (int)v;
	whoami(o)->m_origView->showImageChoice(choice);
}
void ImpressionistUI::cb_edge_clip(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->m_edgeClipping = bool(((Fl_Check_Button*)o)->value());
}

void ImpressionistUI::cb_colorChooserWindow(Fl_Menu_* o, void* v) {
	whoami(o)->m_ColorChooseWindow->show();
}

void ImpressionistUI::cb_colorChooser(Fl_Widget* o, void* v){
	((ImpressionistUI*)(o->user_data()))->colors[0] = ((Fl_Color_Chooser*)o)->r();
	((ImpressionistUI*)(o->user_data()))->colors[1] = ((Fl_Color_Chooser*)o)->g();
	((ImpressionistUI*)(o->user_data()))->colors[2] = ((Fl_Color_Chooser*)o)->b();
}

void ImpressionistUI::cb_BlurSharpChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	int choice = (int)v;

	pDoc->setBlurSharpBrushChoice(choice);
}



void ImpressionistUI::cb_edgeThresholdSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->threshold = int(((Fl_Slider*)o)->value());
}

void ImpressionistUI::cb_edgePaintingButton(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	ImpressionistDoc* pDoc = pUI->getDocument();

	pDoc->generateEdgeImage();
	pUI->m_origView->showImageChoice(SHOW_EDGE_IMAGE);
	
}

void ImpressionistUI::cb_paintlyThresholdSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->paintlyThresh = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_minStrokeSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->minStrokeLength = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_maxStrokeSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->maxStrokeLength = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_layerSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->layer = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_precisionSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->paintlyPrecision = int(((Fl_Slider*)o)->value());
}
// todo : add style choice
void ImpressionistUI::cb_layerRatioSlider(Fl_Widget* o, void* v)
{
	((ImpressionistUI*)(o->user_data()))->layerRatio = int(((Fl_Slider*)o)->value());
}
void ImpressionistUI::cb_paintlyDraw(Fl_Widget* o, void* v)
{
	ImpressionistDoc* pDoc = ((ImpressionistUI*)(o->user_data()))->getDocument();
	pDoc->m_pUI->m_paintView->setEventType(PAINTLY);
	pDoc->m_pUI->m_paintView->setEventTrue();
	pDoc->m_pUI->m_paintView->refresh();
}
void ImpressionistUI::cb_styleChoice(Fl_Widget* o, void* v)
{
	ImpressionistUI* pUI = ((ImpressionistUI*)(o->user_data()));
	int style = (int)v;
	switch (style)
	{
	case IMPRESSIONIST:
		pUI->setPaintlyThresh(10);
		pUI->setMaxStrokeLength(30);
		pUI->setMinStrokeLength(4);
		pUI->setLayer(5);
		pUI->setPaintlyPrecision(1);
		pUI->setLayerRatio(2);
		break;
	case EXPRESSIONIST:
		pUI->setPaintlyThresh(4);
		pUI->setMaxStrokeLength(50);
		pUI->setMinStrokeLength(30);
		pUI->setLayer(5);
		pUI->setPaintlyPrecision(1);
		pUI->setLayerRatio(2);
		break;
	case POINTILIST:
		pUI->setPaintlyThresh(8);
		pUI->setMaxStrokeLength(0);
		pUI->setMinStrokeLength(0);
		pUI->setLayer(4);
		pUI->setPaintlyPrecision(2);
		pUI->setLayerRatio(2);
		break;
	}

}


void ImpressionistUI::cb_multiResolution(Fl_Menu_* o, void* v)
{
	whoami(o)->m_paintView->setEventType(MULTIRESOLUTION);
	whoami(o)->m_paintView->setEventTrue();
	whoami(o)->m_paintView->refresh();
}

//---------------------------------- per instance functions --------------------------------------

//------------------------------------------------
// Return the ImpressionistDoc used
//------------------------------------------------
ImpressionistDoc* ImpressionistUI::getDocument()
{
	return m_pDoc;
}

//------------------------------------------------
// Draw the main window
//------------------------------------------------
void ImpressionistUI::show() {
	m_mainWindow->show();
	m_paintView->show();
	m_origView->show();
}

//------------------------------------------------
// Change the paint and original window sizes to 
// w by h
//------------------------------------------------
void ImpressionistUI::resize_windows(int w, int h) {
	m_paintView->size(w, h);
	m_origView->size(w, h);
}

//------------------------------------------------ 
// Set the ImpressionistDoc used by the UI to 
// communicate with the brushes 
//------------------------------------------------
void ImpressionistUI::setDocument(ImpressionistDoc* doc)
{
	m_pDoc = doc;

	m_origView->m_pDoc = doc;
	m_paintView->m_pDoc = doc;
}

//------------------------------------------------
// Return the brush size
//------------------------------------------------
int ImpressionistUI::getSize()
{
	return m_nSize;
}

//-------------------------------------------------
// Set the brush size
//-------------------------------------------------
void ImpressionistUI::setSize(int size)
{
	m_nSize = size;

	if (size <= 40)
		m_BrushSizeSlider->value(m_nSize);
}

int ImpressionistUI::getLineWidth() { return m_lineWidth; }
void ImpressionistUI::setLineWidth(int width) { m_lineWidth = width; }
int ImpressionistUI::getLineAngle() { return m_lineAngle; }
void ImpressionistUI::setLineAngle(int angle) { m_lineAngle = angle; }
void  ImpressionistUI::setMaxStrokeLength(int length) 
{ 
	maxStrokeLength = length;
	m_maxStrokeSlider->value(length);
};
void  ImpressionistUI::setMinStrokeLength(int length) 
{
	minStrokeLength = length;
	m_minStrokeSlider->value(length);
};
void  ImpressionistUI::setPaintlyThresh(int T)
{
	paintlyThresh = T;
	m_paintlyThresholdSlider->value(T);
};
void  ImpressionistUI::setLayer(int N)
{
	layer = N;
	m_layerSlider->value(N);
}
void  ImpressionistUI::setPaintlyPrecision(int N)
{
	paintlyPrecision = N;
	m_precisionSlider->value(N);
}
void  ImpressionistUI::setLayerRatio(int R)
{
	layerRatio = R;
	m_layerRatioSlider->value(R);
}


// Main menu definition
Fl_Menu_Item ImpressionistUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Image...",	FL_ALT + 'l', (Fl_Callback*)ImpressionistUI::cb_load_image },
		{"Load Another Image", NULL, (Fl_Callback*)ImpressionistUI::cb_load_another_image}, 
	    {"Load Edge Image", NULL, (Fl_Callback*)ImpressionistUI::cb_load_edge_image},
		{"Load Alpha Mapped Image", NULL, (Fl_Callback*)ImpressionistUI::cb_load_alpha_mapped_image},
		{"Load Alpha Image for Matting", NULL, (Fl_Callback*)ImpressionistUI::cb_load_alpha_image_for_matting},
		{"New Mural Image", NULL, (Fl_Callback*)ImpressionistUI::cb_new_mural_image},
		{"&Dissolve Image...", NULL, (Fl_Callback*)ImpressionistUI::cb_load_dissolve_image},
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_save_image, 0, FL_MENU_DIVIDER  },
		// devide
		{ "&Quit",			FL_ALT + 'q', (Fl_Callback*)ImpressionistUI::cb_exit },
		{ 0 },
	{"&Show", 0, 0, 0, FL_SUBMENU},
	    {"show origin image", NULL, (Fl_Callback*)ImpressionistUI::cb_show_image_choice, (void*)SHOW_ORIGIN_IMAGE },
	    {"show another image", NULL, (Fl_Callback*)ImpressionistUI::cb_show_image_choice, (void*)SHOW_ANOTHER_IMAGE},
	    {"show edge image", NULL, (Fl_Callback*)ImpressionistUI::cb_show_image_choice, (void*)SHOW_EDGE_IMAGE },
	    {0},
	{"Functions", 0, 0, 0, FL_SUBMENU},
		{ "&Brushes...",	FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_brushes },
		{ "&Colors...", FL_ALT + 'w', (Fl_Callback*)ImpressionistUI::cb_colorChooserWindow, 0, FL_MENU_DIVIDER },
		{ "&Clear Canvas", FL_ALT + 'c', (Fl_Callback*)ImpressionistUI::cb_clear_canvas},
		{ "&Swap Canvas", NULL, (Fl_Callback*)ImpressionistUI::cb_swapOriginPaint},
		{ "Auto paint", NULL, (Fl_Callback*)ImpressionistUI::cb_auto_paint},
	    { "Multiresolution", NULL, (Fl_Callback*)ImpressionistUI::cb_multiResolution},
		{ "Paintly", NULL, (Fl_Callback*)ImpressionistUI::cb_paintly},
	    { "&Undo", NULL, (Fl_Callback*)ImpressionistUI::cb_undo},
		{"&Added Faded Background...", NULL, (Fl_Callback*)ImpressionistUI::cb_faded_background_window},
		{"&Filter Kernel Design...", NULL, (Fl_Callback*)ImpressionistUI::cb_filter_kernel_design_window},

		{"&Load Video...", NULL, (Fl_Callback*)ImpressionistUI::cb_load_video},
	    {0},

		{"Mosaic of thumbnail", NULL, (Fl_Callback*)ImpressionistUI::cb_mosaic_of_thumbnail_window},
		{"Matting", NULL, (Fl_Callback*)ImpressionistUI::cb_matting},
	{0},
	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback*)ImpressionistUI::cb_about },
		{ 0 },

	{ 0 }
};

// Brush choice menu definition
Fl_Menu_Item ImpressionistUI::brushTypeMenu[NUM_BRUSH_TYPE + 1] = {
  {"Points",			FL_ALT + 'p', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_POINTS},
  {"Lines",				FL_ALT + 'l', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_LINES},
  {"Circles",			FL_ALT + 'c', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_CIRCLES},
  {"Scattered Points",	FL_ALT + 'q', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_SCATTERED_POINTS},
  {"Scattered Lines",	FL_ALT + 'm', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_SCATTERED_LINES},
  {"Scattered Circles",	FL_ALT + 'd', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_SCATTERED_CIRCLES},
  {"Pentagram",			FL_ALT + 'z', (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_PENTAGRAM},
  {"Blur or Sharpen",	0,			  (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_BLURORSHARPEN},
  {"Curve Brush", NULL, (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_CURVE},
  {"Alpha Mapped", NULL, (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_ALPHA_MAPPED},
  {"Warp", NULL, (Fl_Callback*)ImpressionistUI::cb_brushChoice, (void*)BRUSH_WARP},
	{0}
};

Fl_Menu_Item ImpressionistUI::lineDirectionChoiceMenu[NUM_DIRECTION_TYPE + 1] = {
	{"Slider/Right mouse", FL_ALT + 's', (Fl_Callback*)ImpressionistUI::cb_lineDirectionChoice, (void*)SLIDER_RIGHT_CLICK},
	{"Gradient", FL_ALT + 'g', (Fl_Callback*)ImpressionistUI::cb_lineDirectionChoice, (void*)GRADIENT},
	{"Brush Direction", FL_ALT + 'b', (Fl_Callback*)ImpressionistUI::cb_lineDirectionChoice, (void*)BRUSH_DIRECTION},
	{"Gradient of another image", NULL, (Fl_Callback*)ImpressionistUI::cb_lineDirectionChoice, (void*)ANOTHER_GRADIENT}
};

Fl_Menu_Item ImpressionistUI::BlurSharpChoiceMenu[NUM_BLURSHARP_BRUSH + 1] = {
	{"Blurring", 0, (Fl_Callback*)ImpressionistUI::cb_BlurSharpChoice, (void*)SLIDER_RIGHT_CLICK},
	{"Sharpening", 0, (Fl_Callback*)ImpressionistUI::cb_BlurSharpChoice, (void*)GRADIENT},
};
Fl_Menu_Item ImpressionistUI::styleMenu[NUM_STYLE] = {
	{"Impressionist", 0, (Fl_Callback*)ImpressionistUI::cb_styleChoice, (void*)IMPRESSIONIST},
	{"Expresionist", 0, (Fl_Callback*)ImpressionistUI::cb_styleChoice, (void*)EXPRESSIONIST},
	{"POINTILIST", 0, (Fl_Callback*)ImpressionistUI::cb_styleChoice, (void*)POINTILIST},
};
//----------------------------------------------------
// Constructor.  Creates all of the widgets.
// Add new widgets here
//----------------------------------------------------
ImpressionistUI::ImpressionistUI() {
	// Create the main window
	m_mainWindow = new Fl_Window(600, 300, "Impressionist");
	m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
	// install menu bar
	m_menubar = new Fl_Menu_Bar(0, 0, 600, 25);
	m_menubar->menu(menuitems);

	// Create a group that will hold two sub windows inside the main
	// window
	Fl_Group* group = new Fl_Group(0, 25, 600, 275);

	// install paint view window
	m_paintView = new PaintView(300, 25, 300, 275, "This is the paint view");//0jon
	m_paintView->box(FL_DOWN_FRAME);

	// install original view window
	m_origView = new OriginalView(0, 25, 300, 275, "This is the orig view");//300jon
	m_origView->box(FL_DOWN_FRAME); // down frame: FLTK box appearence type
	m_origView->deactivate();

	group->end();
	Fl_Group::current()->resizable(group); // toread

	m_mainWindow->end();

	// init values

	m_nSize = 10;

	// brush dialog definition /// brush dialog window called from mainwindow
	m_brushDialog = new Fl_Window(400, 325, "Brush Dialog");
	// Add a brush type choice to the dialog
	m_BrushTypeChoice = new Fl_Choice(50, 10, 150, 25, "&Brush");
	m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushTypeChoice->menu(brushTypeMenu);
	m_BrushTypeChoice->callback(cb_brushChoice);

	// add line direction choice
	m_BrushTypeChoice = new Fl_Choice(100, 50, 150, 25, "&Line direction");
	m_BrushTypeChoice->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushTypeChoice->menu(lineDirectionChoiceMenu);
	m_BrushTypeChoice->callback(cb_lineDirectionChoice);

	// add edge clipping checkbox
	m_EdgeClipButton = new Fl_Check_Button(260, 50, 150, 25, "Edge Clip");
	m_EdgeClipButton->value(m_edgeClipping);
	m_EdgeClipButton->user_data((void*)(this));
	m_EdgeClipButton->callback(cb_edge_clip);

	// clear canvas button
	m_ClearCanvasButton = new Fl_Button(240, 10, 150, 25, "&Clear Canvas");
	m_ClearCanvasButton->user_data((void*)(this));
	m_ClearCanvasButton->callback(cb_clear_canvas_button);

	// Add brush size slider to the dialog 
	m_BrushSizeSlider = new Fl_Value_Slider(10, 80, 300, 20, "Size");
	m_BrushSizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_BrushSizeSlider->type(FL_HOR_NICE_SLIDER);
	m_BrushSizeSlider->labelfont(FL_COURIER);
	m_BrushSizeSlider->labelsize(12);
	m_BrushSizeSlider->minimum(1);
	m_BrushSizeSlider->maximum(40);
	m_BrushSizeSlider->step(1);
	m_BrushSizeSlider->value(m_nSize);
	m_BrushSizeSlider->align(FL_ALIGN_RIGHT);
	m_BrushSizeSlider->callback(cb_sizeSlides);

	// add line width slider
	m_lineWidthSlider = new Fl_Value_Slider(10, 110, 300, 20, "Line Width");
	m_lineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_lineWidthSlider->type(FL_HOR_NICE_SLIDER);
	m_lineWidthSlider->labelfont(FL_COURIER);
	m_lineWidthSlider->labelsize(12);
	m_lineWidthSlider->minimum(1);
	m_lineWidthSlider->maximum(40);
	m_lineWidthSlider->step(1);
	m_lineWidthSlider->value(m_lineWidth);
	m_lineWidthSlider->align(FL_ALIGN_RIGHT);
	m_lineWidthSlider->callback(cb_lineWidthSlides);

	// add the line angle slider
	m_lineWidthSlider = new Fl_Value_Slider(10, 140, 300, 20, "Line Angle");
	m_lineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_lineWidthSlider->type(FL_HOR_NICE_SLIDER);
	m_lineWidthSlider->labelfont(FL_COURIER);
	m_lineWidthSlider->labelsize(12);
	m_lineWidthSlider->minimum(0);
	m_lineWidthSlider->maximum(359);
	m_lineWidthSlider->step(1);
	m_lineWidthSlider->value(m_lineAngle);
	m_lineWidthSlider->align(FL_ALIGN_RIGHT);
	m_lineWidthSlider->callback(cb_lineAngleSlides);

	// add the alpha value slider
	m_lineWidthSlider = new Fl_Value_Slider(10, 170, 300, 20, "alpha value");
	m_lineWidthSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_lineWidthSlider->type(FL_HOR_NICE_SLIDER);
	m_lineWidthSlider->labelfont(FL_COURIER);
	m_lineWidthSlider->labelsize(12);
	m_lineWidthSlider->minimum(0);
	m_lineWidthSlider->maximum(1);
	m_lineWidthSlider->step(0.01);
	m_lineWidthSlider->value(alpha);
	m_lineWidthSlider->align(FL_ALIGN_RIGHT);
	m_lineWidthSlider->callback(cb_alphaSlides);


	// add blurring & sharpening brush 
	m_BlurSharpTypeChooser = new Fl_Choice(110, 200, 150, 25, "&Blur or Sharpen");
	m_BlurSharpTypeChooser->user_data((void*)(this));	// record self to be used by static callback functions
	m_BlurSharpTypeChooser->menu(BlurSharpChoiceMenu);
	m_BlurSharpTypeChooser->callback(cb_BlurSharpChoice);

	m_BlurSharpLevelSlider = new Fl_Value_Slider(10, 230, 300, 20, "Level");
	m_BlurSharpLevelSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_BlurSharpLevelSlider->type(FL_HOR_NICE_SLIDER);
	m_BlurSharpLevelSlider->labelfont(FL_COURIER);
	m_BlurSharpLevelSlider->labelsize(12);
	m_BlurSharpLevelSlider->minimum(1);
	m_BlurSharpLevelSlider->maximum(10);
	m_BlurSharpLevelSlider->step(1);
	m_BlurSharpLevelSlider->value(level);
	m_BlurSharpLevelSlider->align(FL_ALIGN_RIGHT);
	m_BlurSharpLevelSlider->callback(cb_levelSlider);

	// add edge detecting function
	m_SharpThresholdSlider = new Fl_Value_Slider(10, 260, 180, 20, "Edge Threshold");
	m_SharpThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_SharpThresholdSlider->type(FL_HOR_NICE_SLIDER);
	m_SharpThresholdSlider->labelfont(FL_COURIER);
	m_SharpThresholdSlider->labelsize(12);
	m_SharpThresholdSlider->minimum(0);
	m_SharpThresholdSlider->maximum(500);
	m_SharpThresholdSlider->value(100);
	m_SharpThresholdSlider->step(1);
	m_SharpThresholdSlider->value(threshold);
	m_SharpThresholdSlider->align(FL_ALIGN_RIGHT);
	m_SharpThresholdSlider->callback(cb_edgeThresholdSlider);

	m_DrawEdgeButton = new Fl_Button(300, 260, 80, 20, "&Do it");
	m_DrawEdgeButton->user_data((void*)(this));
	m_DrawEdgeButton->callback(cb_edgePaintingButton);

	m_WarpStrengthSlider = new Fl_Value_Slider(10, 290, 180, 20, "Warp Strength");
	m_WarpStrengthSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_WarpStrengthSlider->type(FL_HOR_NICE_SLIDER);
	m_WarpStrengthSlider->labelfont(FL_COURIER);
	m_WarpStrengthSlider->labelsize(12);
	m_WarpStrengthSlider->minimum(1.0);
	m_WarpStrengthSlider->maximum(14.0);
	m_WarpStrengthSlider->step(0.1);
	m_WarpStrengthSlider->value(warpStrength);
	m_WarpStrengthSlider->align(FL_ALIGN_RIGHT);
	m_WarpStrengthSlider->callback(cb_warpStrengthSlider);


	
	m_brushDialog->end();

	// add color chooser
	m_ColorChooseWindow = new Fl_Window(230, 230, "Color Chooser");
	m_ColorChooseWindow->user_data((void*)(this));

	m_ColorChooser = new Fl_Color_Chooser(10, 10, 200, 200);
	m_ColorChooser->user_data((void*)(this));
	m_ColorChooser->rgb(1.0, 1.0, 1.0);
	m_ColorChooser->mode(0);
	m_ColorChooser->callback(cb_colorChooser);
	m_ColorChooser->end();

	m_ColorChooseWindow->end();

	// add faded background window
	m_fadedBackgroundWindow = new Fl_Window(260, 40, "Faded Background");
	m_fadedBackgroundWindow->user_data((void*)(this));


	m_fadeInSlider = new Fl_Value_Slider(10, 10, 180, 20, "Faded In");
	m_fadeInSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_fadeInSlider->type(FL_HOR_NICE_SLIDER);
	m_fadeInSlider->labelfont(FL_COURIER);
	m_fadeInSlider->labelsize(12);
	m_fadeInSlider->minimum(0.00);
	m_fadeInSlider->maximum(1.00);
	m_fadeInSlider->step(0.01);
	m_fadeInSlider->value(fadedRate);
	m_fadeInSlider->align(FL_ALIGN_RIGHT);
	m_fadeInSlider->callback(cb_faded_slider);

	m_fadedBackgroundWindow->end();

	// paintly dialog
	m_paintlyDialog = new Fl_Window(400, 325, "Paintly Dialog");
	
	// Add style choice
	m_styleChoice = new Fl_Choice(50, 10, 150, 25, "Style");
	m_styleChoice->user_data((void*)(this));	// record self to be used by static callback functions
	m_styleChoice->menu(styleMenu);
	m_styleChoice->callback(cb_styleChoice);

	// Draw button
	m_paintlyDrawButton = new Fl_Button(240, 10, 150, 25, "Draw");
	m_paintlyDrawButton->user_data((void*)(this));
	m_paintlyDrawButton->callback(cb_paintlyDraw);

	// Add threshold slider to the dialog 
	m_paintlyThresholdSlider = new Fl_Value_Slider(10, 80, 300, 20, "Thrshold");
	m_paintlyThresholdSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_paintlyThresholdSlider->type(FL_HOR_NICE_SLIDER);
	m_paintlyThresholdSlider->labelfont(FL_COURIER);
	m_paintlyThresholdSlider->labelsize(12);
	m_paintlyThresholdSlider->minimum(0);
	m_paintlyThresholdSlider->maximum(200);
	m_paintlyThresholdSlider->step(2);
	m_paintlyThresholdSlider->value(paintlyThresh);
	m_paintlyThresholdSlider->align(FL_ALIGN_RIGHT);
	m_paintlyThresholdSlider->callback(cb_paintlyThresholdSlider);

	// add min stroke
	m_minStrokeSlider = new Fl_Value_Slider(10, 110, 300, 20, "Min Stroke");
	m_minStrokeSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_minStrokeSlider->type(FL_HOR_NICE_SLIDER);
	m_minStrokeSlider->labelfont(FL_COURIER);
	m_minStrokeSlider->labelsize(12);
	m_minStrokeSlider->minimum(1);
	m_minStrokeSlider->maximum(50);
	m_minStrokeSlider->step(1);
	m_minStrokeSlider->value(minStrokeLength);
	m_minStrokeSlider->align(FL_ALIGN_RIGHT);
	m_minStrokeSlider->callback(cb_minStrokeSlider);

	// add max stroke
	m_maxStrokeSlider = new Fl_Value_Slider(10, 140, 300, 20, "Max stroke");
	m_maxStrokeSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_maxStrokeSlider->type(FL_HOR_NICE_SLIDER);
	m_maxStrokeSlider->labelfont(FL_COURIER);
	m_maxStrokeSlider->labelsize(12);
	m_maxStrokeSlider->minimum(1);
	m_maxStrokeSlider->maximum(50);
	m_maxStrokeSlider->step(1);
	m_maxStrokeSlider->value(maxStrokeLength);
	m_maxStrokeSlider->align(FL_ALIGN_RIGHT);
	m_maxStrokeSlider->callback(cb_maxStrokeSlider);

	// add layer slider
	m_layerSlider = new Fl_Value_Slider(10, 170, 300, 20, "#Layer");
	m_layerSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_layerSlider->type(FL_HOR_NICE_SLIDER);
	m_layerSlider->labelfont(FL_COURIER);
	m_layerSlider->labelsize(12);
	m_layerSlider->minimum(1);
	m_layerSlider->maximum(6);
	m_layerSlider->step(1);
	m_layerSlider->value(layer);
	m_layerSlider->align(FL_ALIGN_RIGHT);
	m_layerSlider->callback(cb_layerSlider);

	// add precision 
	m_precisionSlider = new Fl_Value_Slider(10, 200, 200, 20, "Precision(min brush size)");
	m_precisionSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_precisionSlider->type(FL_HOR_NICE_SLIDER);
	m_precisionSlider->labelfont(FL_COURIER);
	m_precisionSlider->labelsize(12);
	m_precisionSlider->minimum(1);
	m_precisionSlider->maximum(4);
	m_precisionSlider->step(1);
	m_precisionSlider->value(paintlyPrecision);
	m_precisionSlider->align(FL_ALIGN_RIGHT);
	m_precisionSlider->callback(cb_precisionSlider);

	// add layer ratio
	m_layerRatioSlider = new Fl_Value_Slider(10, 230, 300, 20, "Layer ratio");
	m_layerRatioSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_layerRatioSlider->type(FL_HOR_NICE_SLIDER);
	m_layerRatioSlider->labelfont(FL_COURIER);
	m_layerRatioSlider->labelsize(12);
	m_layerRatioSlider->minimum(2);
	m_layerRatioSlider->maximum(4);
	m_layerRatioSlider->step(1);
	m_layerRatioSlider->value(layerRatio);
	m_layerRatioSlider->align(FL_ALIGN_RIGHT);
	m_layerRatioSlider->callback(cb_layerRatioSlider);

	m_paintlyDialog->end();

	m_filterKernelDesignWindow = new Fl_Window(300, 230, "Color Chooser");
	m_filterKernelDesignWindow->user_data((void*)(this));

	m_filterSizeInput = new Fl_Int_Input(100, 10, 50, 30,"Filter Size");
	m_filterSizeInput->user_data((void*)(this));
	m_filterSizeInput->callback(cb_filter_size_input);

	m_filterWeightInput = new Fl_Input(100, 40, 180, 30, "Weight (split \nwith space)");
	m_filterWeightInput->user_data((void*)(this));
	m_filterWeightInput->callback(cb_filter_weight_input);

	m_filter_normalize_check_button = new Fl_Check_Button(10, 70, 150, 25, "Normalized");
	m_filter_normalize_check_button->value(normalized);
	m_filter_normalize_check_button->user_data((void*)(this));
	m_filter_normalize_check_button->callback(cb_filter_normalized);

	m_filter_apply_button = new Fl_Button(200, 70, 80, 20, "&Apply");
	m_filter_apply_button->user_data((void*)(this));
	m_filter_apply_button->callback(cb_filter_apply);

	m_filterKernelDesignWindow->end();


	m_MosaicWindow = new Fl_Window(350, 230, "Mosaic of Thumbnail");
	m_MosaicWindow->user_data((void*)(this));

	m_RatioSlider = new Fl_Value_Slider(10, 10, 180, 20, "Ratio of Thumbnail \n and Orignal Picture");
	m_RatioSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_RatioSlider->type(FL_HOR_NICE_SLIDER);
	m_RatioSlider->labelfont(FL_COURIER);
	m_RatioSlider->labelsize(12);
	m_RatioSlider->minimum(1);
	m_RatioSlider->maximum(15);
	m_RatioSlider->value(ratio);
	m_RatioSlider->step(1);
	m_RatioSlider->align(FL_ALIGN_RIGHT);
	m_RatioSlider->callback(cb_mosaic_of_thumbnail_ratioSlider);
	
	m_AlphaOfMosaicSlider = new Fl_Value_Slider(10, 40, 180, 20, "Alpha");
	m_AlphaOfMosaicSlider->user_data((void*)(this));	// record self to be used by static callback functions
	m_AlphaOfMosaicSlider->type(FL_HOR_NICE_SLIDER);
	m_AlphaOfMosaicSlider->labelfont(FL_COURIER);
	m_AlphaOfMosaicSlider->labelsize(12);
	m_AlphaOfMosaicSlider->minimum(0.00);
	m_AlphaOfMosaicSlider->maximum(1.00);
	m_AlphaOfMosaicSlider->value(alphaOfMosaic);
	m_AlphaOfMosaicSlider->step(0.01);
	m_AlphaOfMosaicSlider->align(FL_ALIGN_RIGHT);
	m_AlphaOfMosaicSlider->callback(cb_mosaic_of_thumbnail_alphaSlider);

	m_MosaicApplyButton = new Fl_Button(10, 70, 80, 20, "&Apply");
	m_MosaicApplyButton->user_data((void*)(this));
	m_MosaicApplyButton->callback(cb_mosaic_of_thumbnail_apply);

	m_MosaicWindow->end();

}
