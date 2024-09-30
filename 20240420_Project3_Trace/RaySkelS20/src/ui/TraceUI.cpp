//
// TraceUI.h
//
// Handles FLTK integration and other user interface tasks
//
#include <stdio.h>
#include <time.h>
#include <string.h>

#include <FL/fl_ask.h>

#include "TraceUI.h"
#include "../RayTracer.h"

static bool done;

//------------------------------------- Help Functions --------------------------------------------
TraceUI* TraceUI::whoami(Fl_Menu_* o)	// from menu item back to UI itself
{
	return ( (TraceUI*)(o->parent()->user_data()) );
}

//--------------------------------- Callback Functions --------------------------------------------
void TraceUI::cb_load_scene(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* newfile = fl_file_chooser("Open Scene?", "*.ray", NULL );

	if (newfile != NULL) {
		char buf[256];

		if (pUI->raytracer->loadScene(newfile)) {
			sprintf(buf, "Ray <%s>", newfile);
			done=true;	// terminate the previous rendering
		} else{
			sprintf(buf, "Ray <Not Loaded>");
		}

		pUI->m_mainWindow->label(buf);
	}
}

void TraceUI::cb_save_image(Fl_Menu_* o, void* v) 
{
	TraceUI* pUI=whoami(o);
	
	char* savefile = fl_file_chooser("Save Image?", "*.bmp", "save.bmp" );
	if (savefile != NULL) {
		pUI->m_traceGlWindow->saveImage(savefile);
	}
}

void TraceUI::cb_exit(Fl_Menu_* o, void* v)
{
	TraceUI* pUI=whoami(o);

	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_exit2(Fl_Widget* o, void* v) 
{
	TraceUI* pUI=(TraceUI *)(o->user_data());
	
	// terminate the rendering
	done=true;

	pUI->m_traceGlWindow->hide();
	pUI->m_mainWindow->hide();
}

void TraceUI::cb_about(Fl_Menu_* o, void* v) 
{
	fl_message("RayTracer Project, FLTK version for CS 341 Spring 2002. Latest modifications by Jeff Maurer, jmaurer@cs.washington.edu");
}

void TraceUI::cb_sizeSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI=(TraceUI*)(o->user_data());
	
	pUI->m_nSize=int( ((Fl_Slider *)o)->value() ) ;
	int	height = (int)(pUI->m_nSize / pUI->raytracer->aspectRatio() + 0.5);
	pUI->m_traceGlWindow->resizeWindow( pUI->m_nSize, height );
}

void TraceUI::cb_depthSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth=int( ((Fl_Slider *)o)->value() ) ;
}

void TraceUI::cb_constantAttenuationSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_dConstantAttenuation = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_linearAttenuationSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_dLinearAttenuation = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_quadraticAttenuationSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_dQuadraticAttenuation = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_ambientLightSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_dAmbientLight = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_threshSlides(Fl_Widget* o, void* v)
{
	float value = float(((Fl_Slider*)o)->value());
	((TraceUI*)(o->user_data()))->m_vthresh = vec3f(value, value, value);
}

void TraceUI::cb_glossy(Fl_Widget* o, void* v) {
	if (((TraceUI*)(o->user_data()))->m_bGlossy == true) {
		((Fl_Light_Button*)o)->value(false);
		((TraceUI*)(o->user_data()))->m_bGlossy = false;
	}
	else {
		((Fl_Light_Button*)o)->value(true);
		((TraceUI*)(o->user_data()))->m_bGlossy = true;
	}
}

/*
void TraceUI::cb_intensityScaleSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_distanceScaleSlides(Fl_Widget* o, void* v)
{
	((TraceUI*)(o->user_data()))->m_nDepth = int(((Fl_Slider*)o)->value());
}
*/

void TraceUI::cb_buildPhotonMap(Fl_Widget* o, void* v)
{
	TraceUI* pUI = (TraceUI*)(o->user_data());
	if (!pUI->raytracer->sceneLoaded()) {
		fl_alert("Scene not loaded.");
	}
	else {
		pUI->raytracer->buildPhotonMap();
		fl_alert("Finish photon map construction.");
	}
}

void TraceUI::cb_enablePhotonMap(Fl_Widget* o, void* v)
{
	TraceUI* pUI = (TraceUI*)(o->user_data());
	pUI->raytracer->m_bCaustics = bool(((Fl_Light_Button*)o)->value());
}

void TraceUI::cb_photonNumSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI = (TraceUI*)(o->user_data());
	pUI->raytracer->m_nPhotons = 1000 * (int(((Fl_Slider*)o)->value()));
}

void TraceUI::cb_neighbourNumSlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI = (TraceUI*)(o->user_data());
	pUI->raytracer->k = (int(((Fl_Slider*)o)->value()));
}

void TraceUI::cb_intensitySlides(Fl_Widget* o, void* v)
{
	TraceUI* pUI = (TraceUI*)(o->user_data());
	pUI->raytracer->m_intensity = int(((Fl_Slider*)o)->value());
}

void TraceUI::cb_render(Fl_Widget* o, void* v)
{
	char buffer[256];

	TraceUI* pUI=((TraceUI*)(o->user_data()));
	
	if (pUI->raytracer->sceneLoaded()) {
		int width=pUI->getSize();
		int	height = (int)(width / pUI->raytracer->aspectRatio() + 0.5);
		pUI->m_traceGlWindow->resizeWindow( width, height );

		pUI->m_traceGlWindow->show();

		pUI->raytracer->traceSetup(width, height, pUI->getDepth(), pUI->getThresh());
		
		// Save the window label
		const char *old_label = pUI->m_traceGlWindow->label();

		// start to render here	
		done=false;
		clock_t prev, now;
		prev=clock();
		
		pUI->m_traceGlWindow->refresh();
		Fl::check();
		Fl::flush();

		for (int y=0; y<height; y++) {
			for (int x=0; x<width; x++) {
				if (done) break;
				
				// current time
				now = clock();

				// check event every 1/2 second
				if (((double)(now-prev)/CLOCKS_PER_SEC)>0.5) {
					prev=now;

					if (Fl::ready()) {
						// refresh
						pUI->m_traceGlWindow->refresh();
						// check event
						Fl::check();

						if (Fl::damage()) {
							Fl::flush();
						}
					}
				}

				pUI->raytracer->tracePixel( x, y );
		
			}
			if (done) break;

			// flush when finish a row
			if (Fl::ready()) {
				// refresh
				pUI->m_traceGlWindow->refresh();

				if (Fl::damage()) {
					Fl::flush();
				}
			}
			// update the window label
			sprintf(buffer, "(%d%%) %s", (int)((double)y / (double)height * 100.0), old_label);
			pUI->m_traceGlWindow->label(buffer);
			
		}
		done=true;
		pUI->m_traceGlWindow->refresh();

		// Restore the window label
		pUI->m_traceGlWindow->label(old_label);		
	}
}

void TraceUI::cb_stop(Fl_Widget* o, void* v)
{
	done=true;
}

void TraceUI::show()
{
	m_mainWindow->show();
}

void TraceUI::setRayTracer(RayTracer *tracer)
{
	raytracer = tracer;
	m_traceGlWindow->setRayTracer(tracer);
}

int TraceUI::getSize()
{
	return m_nSize;
}

int TraceUI::getDepth()
{
	return m_nDepth;
}

double TraceUI::getConstantAttenuation() {
	return m_dConstantAttenuation;
}

double TraceUI::getLinearAttenuation() {
	return m_dLinearAttenuation;
}

double TraceUI::getQuadraticAttenuation() {
	return m_dQuadraticAttenuation;
}

double TraceUI::getAmbientLight() {
	return m_dAmbientLight;
}

vec3f TraceUI::getThresh() {
	return m_vthresh;
}

bool TraceUI::getGlossy() {
	return m_bGlossy;
}

/*
int TraceUI::getIntensityScale() {
	return m_nIntensityScale;
}

double TraceUI::getDistanceScale() {
	return m_dDistanceScale;
}
*/
// menu definition
Fl_Menu_Item TraceUI::menuitems[] = {
	{ "&File",		0, 0, 0, FL_SUBMENU },
		{ "&Load Scene...",	FL_ALT + 'l', (Fl_Callback *)TraceUI::cb_load_scene },
		{ "&Save Image...",	FL_ALT + 's', (Fl_Callback *)TraceUI::cb_save_image },
		{ "&Exit",			FL_ALT + 'e', (Fl_Callback *)TraceUI::cb_exit },
		{ 0 },

	{ "&Help",		0, 0, 0, FL_SUBMENU },
		{ "&About",	FL_ALT + 'a', (Fl_Callback *)TraceUI::cb_about },
		{ 0 },

	{ 0 }
};

TraceUI::TraceUI() {
	// init.
	m_nDepth = 0;
	m_nSize = 150;

	m_dConstantAttenuation = 0.25;
	m_dLinearAttenuation = 0.25;
	m_dQuadraticAttenuation = 0.50;
	m_dAmbientLight = 0.20;
	m_vthresh = vec3f(0.0, 0.0, 0.0);
	// m_nIntensityScale = 1;
	// m_dDistanceScale = 1.87;

	m_mainWindow = new Fl_Window(100, 40, 350, 330, "Ray <Not Loaded>");
		m_mainWindow->user_data((void*)(this));	// record self to be used by static callback functions
		// install menu bar
		m_menubar = new Fl_Menu_Bar(0, 0, 350, 25);
		m_menubar->menu(menuitems);

		// install slider depth
		m_depthSlider = new Fl_Value_Slider(10, 30, 180, 20, "Depth");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
        m_depthSlider->labelfont(FL_COURIER);
        m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nDepth);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_depthSlides);

		// install slider size
		m_sizeSlider = new Fl_Value_Slider(10, 55, 180, 20, "Size");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
        m_sizeSlider->labelfont(FL_COURIER);
        m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(64);
		m_sizeSlider->maximum(512);
		m_sizeSlider->step(1);
		m_sizeSlider->value(m_nSize);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_sizeSlides);

		// install slider constant attenuation
		m_depthSlider = new Fl_Value_Slider(10, 80, 180, 20, "Attenuation, Constant");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
		m_depthSlider->labelfont(FL_COURIER);
		m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0.00);
		m_depthSlider->maximum(1.00);
		m_depthSlider->step(0.01);
		m_depthSlider->value(m_dConstantAttenuation);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_constantAttenuationSlides);

		// install slider linear attenuation
		m_sizeSlider = new Fl_Value_Slider(10, 105, 180, 20, "Attenuation, Linear");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
		m_sizeSlider->labelfont(FL_COURIER);
		m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(0.00);
		m_sizeSlider->maximum(1.00);
		m_sizeSlider->step(0.01);
		m_sizeSlider->value(m_dLinearAttenuation);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_linearAttenuationSlides);

		// install slider quadratic attenuation
		m_depthSlider = new Fl_Value_Slider(10, 130, 180, 20, "Attenuation, Quadratic");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
		m_depthSlider->labelfont(FL_COURIER);
		m_depthSlider->labelsize(12);
		m_depthSlider->minimum(0.00);
		m_depthSlider->maximum(1.00);
		m_depthSlider->step(0.01);
		m_depthSlider->value(m_dQuadraticAttenuation);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_quadraticAttenuationSlides);

		// install slider ambient light
		m_sizeSlider = new Fl_Value_Slider(10, 155, 180, 20, "Ambient Light");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
		m_sizeSlider->labelfont(FL_COURIER);
		m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(0.00);
		m_sizeSlider->maximum(1.00);
		m_sizeSlider->step(0.01);
		m_sizeSlider->value(m_dAmbientLight);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_ambientLightSlides);

		// install slider thresh
		m_threshSlider = new Fl_Value_Slider(10, 180, 180, 20, "Threshold");
		m_threshSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_threshSlider->type(FL_HOR_NICE_SLIDER);
		m_threshSlider->labelfont(FL_COURIER);
		m_threshSlider->labelsize(12);
		m_threshSlider->minimum(0.0f);
		m_threshSlider->maximum(1.0f);
		m_threshSlider->step(0.01f);
		m_threshSlider->value(m_vthresh[0]);
		m_threshSlider->align(FL_ALIGN_RIGHT);
		m_threshSlider->callback(cb_threshSlides);
		/*
		// install slider intensity scale
		m_depthSlider = new Fl_Value_Slider(10, 180, 180, 20, "Intensity Scale");
		m_depthSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_depthSlider->type(FL_HOR_NICE_SLIDER);
		m_depthSlider->labelfont(FL_COURIER);
		m_depthSlider->labelsize(12);
		m_depthSlider->minimum(1);
		m_depthSlider->maximum(10);
		m_depthSlider->step(1);
		m_depthSlider->value(m_nIntensityScale);
		m_depthSlider->align(FL_ALIGN_RIGHT);
		m_depthSlider->callback(cb_intensityScaleSlides);

		// install slider distance scale
		m_sizeSlider = new Fl_Value_Slider(10, 205, 180, 20, "Distance Scale (Log10)");
		m_sizeSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_sizeSlider->type(FL_HOR_NICE_SLIDER);
		m_sizeSlider->labelfont(FL_COURIER);
		m_sizeSlider->labelsize(12);
		m_sizeSlider->minimum(-0.99);
		m_sizeSlider->maximum(3.00);
		m_sizeSlider->step(0.01);
		m_sizeSlider->value(m_dDistanceScale);
		m_sizeSlider->align(FL_ALIGN_RIGHT);
		m_sizeSlider->callback(cb_distanceScaleSlides);
		*/

		m_buildPhotonMapButton = new Fl_Button(10, 210, 120, 25, "Build Photon Map");
		m_buildPhotonMapButton->user_data(this);
		m_buildPhotonMapButton->callback(cb_buildPhotonMap);

		m_enablePhotonMapButton = new Fl_Light_Button(140, 210, 180, 25, "Enable Photon Mapping");
		m_enablePhotonMapButton->user_data(this);
		m_enablePhotonMapButton->value(0);
		m_enablePhotonMapButton->callback(cb_enablePhotonMap);

		m_numPhotonSlider = new Fl_Value_Slider(10, 240, 180, 20, "Number of Photons");
		m_numPhotonSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_numPhotonSlider->type(FL_HOR_NICE_SLIDER);
		m_numPhotonSlider->labelfont(FL_COURIER);
		m_numPhotonSlider->labelsize(12);
		m_numPhotonSlider->minimum(10);
		m_numPhotonSlider->maximum(100);
		m_numPhotonSlider->step(1);
		m_numPhotonSlider->value(50);
		m_numPhotonSlider->align(FL_ALIGN_RIGHT);
		m_numPhotonSlider->callback(cb_photonNumSlides);

		m_numNeighbourSlider = new Fl_Value_Slider(10, 265, 180, 20, "Number of Neighbours");
		m_numNeighbourSlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_numNeighbourSlider->type(FL_HOR_NICE_SLIDER);
		m_numNeighbourSlider->labelfont(FL_COURIER);
		m_numNeighbourSlider->labelsize(12);
		m_numNeighbourSlider->minimum(10);
		m_numNeighbourSlider->maximum(100);
		m_numNeighbourSlider->step(1);
		m_numNeighbourSlider->value(20);
		m_numNeighbourSlider->align(FL_ALIGN_RIGHT);
		m_numNeighbourSlider->callback(cb_neighbourNumSlides);

		m_intensitySlider = new Fl_Value_Slider(10, 290, 180, 20, "Number of Lights");
		m_intensitySlider->user_data((void*)(this));	// record self to be used by static callback functions
		m_intensitySlider->type(FL_HOR_NICE_SLIDER);
		m_intensitySlider->labelfont(FL_COURIER);
		m_intensitySlider->labelsize(12);
		m_intensitySlider->minimum(1);
		m_intensitySlider->maximum(10);
		m_intensitySlider->step(1);
		m_intensitySlider->value(1);
		m_intensitySlider->align(FL_ALIGN_RIGHT);
		m_intensitySlider->callback(cb_intensitySlides);

		m_renderButton = new Fl_Button(240, 27, 70, 25, "&Render");
		m_renderButton->user_data((void*)(this));
		m_renderButton->callback(cb_render);

		m_stopButton = new Fl_Button(240, 55, 70, 25, "&Stop");
		m_stopButton->user_data((void*)(this));
		m_stopButton->callback(cb_stop);

		m_glossyButton = new Fl_Light_Button(10, 205, 70, 25, "Glossy");
		m_glossyButton->user_data((void*)(this));
		m_glossyButton->set();
		m_glossyButton->callback(cb_glossy);

		m_mainWindow->callback(cb_exit2);
		m_mainWindow->when(FL_HIDE);
    m_mainWindow->end();

	// image view
	m_traceGlWindow = new TraceGLWindow(100, 150, m_nSize, m_nSize, "Rendered Image");
	m_traceGlWindow->end();
	m_traceGlWindow->resizable(m_traceGlWindow);
}