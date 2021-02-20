//
// impressionistUI.h
//
// The header file for the UI part
//

#ifndef ImpressionistUI_h
#define ImpressionistUI_h

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/fl_file_chooser.H>		// FLTK file chooser
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Light_Button.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window* m_mainWindow;
	Fl_Menu_Bar* m_menubar;

	PaintView* m_paintView;
	OriginalView* m_origView;

	// for brush dialog
	Fl_Window* m_brushDialog;
	Fl_Choice* m_BrushTypeChoice;

	Fl_Slider* m_BrushSizeSlider;
	Fl_Button* m_ClearCanvasButton;

	Fl_Slider* m_lineWidthSlider;
	Fl_Slider* m_lineAngleSlider;
	Fl_Choice* m_lineDirectionChoice;
	// Member functions
	void				setDocument(ImpressionistDoc* doc);
	ImpressionistDoc* getDocument();

	void				show();
	void				resize_windows(int w, int h);

	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	int					getLineWidth();
	void 				setLineWidth(int width);
	int					getLineAngle();
	void 				setLineAngle(int angle);
	double				getAlpha() { return alpha; }
	void				setAlpha(double alpha) { this->alpha = alpha; }

private:
	ImpressionistDoc* m_pDoc;		// pointer to document to communicate with the document

	// All attributes here (attributes of the brush)
	int		m_nSize;
	int		m_lineWidth{ 1 };
	int		m_lineAngle{ 0 };
	double  alpha{ 1 };

	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item		lineDirectionChoiceMenu[NUM_DIRECTION_TYPE + 1];

	static ImpressionistUI* whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void cb_load_another_image(Fl_Menu_* o, void* v);
	static void cb_load_edge_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void cb_load_dissolve_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void	cb_clear_canvas(Fl_Menu_* o, void* v);
	static void	cb_exit(Fl_Menu_* o, void* v);
	static void	cb_about(Fl_Menu_* o, void* v);
	static void	cb_brushChoice(Fl_Widget* o, void* v);
	static void cb_lineDirectionChoice(Fl_Widget* o, void* v);
	static void	cb_clear_canvas_button(Fl_Widget* o, void* v);
	static void	cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_lineWidthSlides(Fl_Widget* o, void* v);
	static void cb_lineAngleSlides(Fl_Widget* o, void* v);
	static void cb_alphaSlides(Fl_Widget* o, void* v);
	static void cb_swapOriginPaint(Fl_Menu_* o, void* v);
	static void cb_undo(Fl_Menu_* o, void* v);
	static void cb_auto_paint(Fl_Menu_* o, void* v);
	static void cb_show_image_choice(Fl_Menu_* o, void* v);
};

#endif
