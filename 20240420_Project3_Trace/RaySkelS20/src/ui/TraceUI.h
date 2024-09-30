//
// rayUI.h
//
// The header file for the UI part
//

#ifndef __rayUI_h__
#define __rayUI_h__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Button.H>

#include <FL/fl_file_chooser.H>		// FLTK file chooser

#include "TraceGLWindow.h"

class TraceUI {
public:
	TraceUI();

	// The FLTK widgets
	Fl_Window*			m_mainWindow;
	Fl_Menu_Bar*		m_menubar;

	Fl_Slider*			m_sizeSlider;
	Fl_Slider*			m_depthSlider;
	Fl_Slider*			m_constantAttenuationSlider;
	Fl_Slider*			m_linearAttenuationSlider;
	Fl_Slider*			m_quadraticAttenuationSlider;
	Fl_Slider*			m_ambientLightSlider;
	Fl_Slider*			m_threshSlider;
	// Fl_Slider*			m_intensityScaleSlider;
	// Fl_Slider*			m_distanceScaleSlider;

	Fl_Button*			m_buildPhotonMapButton;
	Fl_Light_Button*	m_enablePhotonMapButton;
	Fl_Slider*			m_numPhotonSlider;
	Fl_Slider*			m_numNeighbourSlider;
	Fl_Slider*			m_intensitySlider;

	Fl_Button*			m_renderButton;
	Fl_Button*			m_stopButton;
	Fl_Button*			m_glossyButton;

	TraceGLWindow*		m_traceGlWindow;

	// member functions
	void show();

	void		setRayTracer(RayTracer *tracer);

	int			getSize();
	int			getDepth();
	double		getConstantAttenuation();
	double		getLinearAttenuation();
	double		getQuadraticAttenuation();
	double		getAmbientLight();
	vec3f		getThresh();
	bool		getGlossy();
	// int			getIntensityScale();
	// double		getDistanceScale();

private:
	RayTracer*	raytracer;

	int			m_nSize;
	int			m_nDepth;
	double		m_dConstantAttenuation;
	double		m_dLinearAttenuation;
	double		m_dQuadraticAttenuation;
	double		m_dAmbientLight;
	vec3f		m_vthresh;
	bool		m_bGlossy;
	// int			m_nIntensityScale;
	// double		m_dDistanceScale;

// static class members
	static Fl_Menu_Item menuitems[];

	static TraceUI* whoami(Fl_Menu_* o);

	static void cb_load_scene(Fl_Menu_* o, void* v);
	static void cb_save_image(Fl_Menu_* o, void* v);
	static void cb_exit(Fl_Menu_* o, void* v);
	static void cb_about(Fl_Menu_* o, void* v);

	static void cb_exit2(Fl_Widget* o, void* v);

	static void cb_sizeSlides(Fl_Widget* o, void* v);
	static void cb_depthSlides(Fl_Widget* o, void* v);
	static void cb_constantAttenuationSlides(Fl_Widget* o, void* v);
	static void cb_linearAttenuationSlides(Fl_Widget* o, void* v);
	static void cb_quadraticAttenuationSlides(Fl_Widget* o, void* v);
	static void cb_ambientLightSlides(Fl_Widget* o, void* v);
	static void cb_threshSlides(Fl_Widget* o, void* v);
	// static void cb_intensityScaleSlides(Fl_Widget* o, void* v);
	// static void cb_distanceScaleSlides(Fl_Widget* o, void* v);

	static void cb_buildPhotonMap(Fl_Widget* o, void* v);
	static void cb_enablePhotonMap(Fl_Widget* o, void* v);
	static void cb_photonNumSlides(Fl_Widget* o, void* v);
	static void cb_neighbourNumSlides(Fl_Widget* o, void* v);
	static void cb_intensitySlides(Fl_Widget* o, void* v);

	static void cb_render(Fl_Widget* o, void* v);
	static void cb_stop(Fl_Widget* o, void* v);
	static void cb_glossy(Fl_Widget* o, void* v);
};

#endif
