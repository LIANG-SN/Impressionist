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
#include <FL/Fl_Color_Chooser.H>
#include <FL/FL_Text_Display.H>
#include <FL/Fl_Int_Input.H>
#include <FL/Fl_Input.H>

#include "Impressionist.h"
#include "OriginalView.h"
#include "PaintView.h"

#include "ImpBrush.h"
#include <string>

class ImpressionistUI {
public:
	ImpressionistUI();

	// The FLTK widgets
	Fl_Window* m_mainWindow;
	Fl_Menu_Bar* m_menubar;

	PaintView* m_paintView;
	OriginalView* m_origView;

	Fl_Window* m_fadedBackgroundWindow;
	Fl_Slider* m_fadeInSlider;


	// for brush dialog
	Fl_Window* m_brushDialog;
	Fl_Choice* m_BrushTypeChoice;

	Fl_Slider* m_BrushSizeSlider;
	Fl_Button* m_ClearCanvasButton;

	Fl_Slider* m_lineWidthSlider;
	Fl_Slider* m_lineAngleSlider;
	Fl_Choice* m_lineDirectionChoice;

	Fl_Check_Button* m_EdgeClipButton;
	Fl_Window* m_ColorChooseWindow;
	Fl_Color_Chooser* m_ColorChooser;


	Fl_Choice* m_BlurSharpTypeChooser;
	Fl_Slider* m_BlurSharpLevelSlider;
	Fl_Slider* m_SharpThresholdSlider;
	Fl_Button* m_DrawEdgeButton;

	// text window
	Fl_Text_Buffer* m_textBuff;
	Fl_Text_Display* m_textDisplay;

	Fl_Window*			m_filterKernelDesignWindow;
	Fl_Int_Input*		m_filterSizeInput;
	Fl_Input*			m_filterWeightInput;
	Fl_Button*			m_filter_apply_button;
	Fl_Check_Button*	m_filter_normalize_check_button;

	// paintly Dialog
	Fl_Window* m_paintlyDialog;
	Fl_Choice* m_styleChoice;

	Fl_Button* m_paintlyDrawButton;

	Fl_Slider* m_paintlyThresholdSlider;
	Fl_Slider* m_minStrokeSlider;
	Fl_Slider* m_maxStrokeSlider;
	Fl_Slider* m_layerSlider;
	Fl_Slider* m_precisionSlider;
	Fl_Slider* m_layerRatioSlider;

	// Member functions
	void			setDocument(ImpressionistDoc* doc);
	ImpressionistDoc* getDocument();

	void				show();
	void				resize_windows(int w, int h);

	void				print(std::string s);
	// Interface to get attribute

	int					getSize();
	void				setSize(int size);
	int					getLineWidth();
	void 				setLineWidth(int width);
	int					getLineAngle();
	void 				setLineAngle(int angle);
	double				getAlpha() { return alpha; }
	void				setAlpha(double alpha) { this->alpha = alpha;  }
	bool				getEdgeClipping() { return m_edgeClipping; }
	double				getRed() { return colors[0]; };
	double				getGreen() { return colors[1]; };
	double				getBlue() { return colors[2]; };
	int					getLevel() { return level; };
	int					getThreshold() { return threshold; };
	double				getFadedRate() { return fadedRate; };

	int					getFilterSize() { return filterSize; };
	double*				getFilterWeight() { return filterWeight; };
	bool				getNormalized() { return normalized; };
	
	// paintly params
	int					getMaxStrokeLength() { return maxStrokeLength; };
	int					getMinStrokeLength() { return minStrokeLength; };
	float				getCurveFilter() { return curveFilter; };

	float				getPaintlyGridSize() { return paintlyGridSize; };
	float				getBlurFactor() { return blurFactor; };
	int					getPaintlyThresh() { return paintlyThresh; };
	int					getLayer() { return layer; };
	int					getPaintlyPrecision() { return paintlyPrecision; };
	int					getLayerRatio() { return layerRatio; };

	void				setMaxStrokeLength(int length);
	void				setMinStrokeLength(int length);
	void				setPaintlyThresh(int T);
	void				setLayer(int N);
	void				setPaintlyPrecision(int N);
	void				setLayerRatio(int R);

private:

	ImpressionistDoc* m_pDoc;		// pointer to document to communicate with the document

	// All attributes here (attributes of the brush)
	int		m_nSize;
	int		m_lineWidth{ 1 };
	int		m_lineAngle{ 0 };
	double  alpha{ 1 };
	bool    m_edgeClipping{ 1 };
	double	colors[3]{ 1.0,1.0,1.0 };
	int		level{ 1 };
	int		threshold{ 100 };
	double  fadedRate{ 0.00 };

	// paintly params
	int		maxStrokeLength{ 16 };
	int		minStrokeLength{ 2 };
	float     curveFilter{ 1 };
	float   paintlyGridSize{ 1 };
	float   blurFactor{ 1 };
	int     paintlyThresh{ 8 };
	int		layer{ 5 };
	int		paintlyPrecision{ 1 };
	int		layerRatio{ 2 };

	
	int		filterSize{ 1 };
	double*	filterWeight=NULL;
	bool	normalized{ 1 };
	//const char* 


	// Static class members
	static Fl_Menu_Item		menuitems[];
	static Fl_Menu_Item		brushTypeMenu[NUM_BRUSH_TYPE + 1];
	static Fl_Menu_Item		lineDirectionChoiceMenu[NUM_DIRECTION_TYPE + 1];
	static Fl_Menu_Item		BlurSharpChoiceMenu[NUM_BLURSHARP_BRUSH + 1];
	static Fl_Menu_Item		styleMenu[NUM_STYLE];

	static ImpressionistUI* whoami(Fl_Menu_* o);

	// All callbacks here.  Callbacks are declared 
	// static
	static void	cb_load_image(Fl_Menu_* o, void* v);
	static void cb_load_another_image(Fl_Menu_* o, void* v);
	static void cb_load_edge_image(Fl_Menu_* o, void* v);
	static void cb_load_alpha_mapped_image(Fl_Menu_* o, void* v);
	static void cb_new_mural_image(Fl_Menu_* o, void* v);
	static void	cb_save_image(Fl_Menu_* o, void* v);
	static void cb_load_dissolve_image(Fl_Menu_* o, void* v);
	static void	cb_brushes(Fl_Menu_* o, void* v);
	static void cb_faded_background_window(Fl_Menu_* o, void* v);
	static void cb_faded_slider(Fl_Widget* o, void* v);

	static void cb_filter_kernel_design_window(Fl_Menu_* o, void* v);
	static void cb_filter_size_input(Fl_Widget* o, void* v);
	static void cb_filter_weight_input(Fl_Widget* o, void* v);
	static void cb_filter_apply(Fl_Widget* o, void* v);
	static void cb_filter_normalized(Fl_Widget* o, void* v);


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
	static void cb_BlurSharpChoice(Fl_Widget* o, void* v);
	static void cb_levelSlider(Fl_Widget* o, void* v);
	static void cb_swapOriginPaint(Fl_Menu_* o, void* v);
	static void cb_undo(Fl_Menu_* o, void* v);
	static void cb_auto_paint(Fl_Menu_* o, void* v);
	static void cb_paintly(Fl_Menu_* o, void* v);
	static void cb_show_image_choice(Fl_Menu_* o, void* v);
	static void	cb_edge_clip(Fl_Widget* o, void* v);
	static void cb_colorChooserWindow(Fl_Menu_* o, void* v);
	static void cb_colorChooser(Fl_Widget* o, void* v);
	static void cb_edgeThresholdSlider(Fl_Widget* o, void* v);
	static void cb_edgePaintingButton(Fl_Widget* o, void* v);
	static void cb_multiResolution(Fl_Menu_* o, void* v);
	
	// paintly
	static void cb_paintlyThresholdSlider(Fl_Widget* o, void* v);
	static void cb_minStrokeSlider(Fl_Widget* o, void* v);
	static void cb_maxStrokeSlider(Fl_Widget* o, void* v);
	static void cb_layerSlider(Fl_Widget* o, void* v);
	static void cb_precisionSlider(Fl_Widget* o, void* v);
	static void cb_styleChoice(Fl_Widget* o, void* v);
	static void cb_layerRatioSlider(Fl_Widget* o, void* v);
	static void cb_paintlyDraw(Fl_Widget* o, void* v);

};

#endif
