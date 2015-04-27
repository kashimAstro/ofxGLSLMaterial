#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGLSLMaterial.h"
#include "ofxPostProcessing.h"

class ofApp : public ofBaseApp {
	public:
		ofEasyCam cam;
		int w,h;
		bool active;

		ofxGLSLMaterial materiale;
		ofxGLSLMaterial materiale1;
		ofxGLSLMaterial materiale2;


		ofxAssimpModelLoader model;
		string imgcapsule;
		ofFbo fboscene;
		ofImage bg;
		ofxPanel gui;
		ofParameter<float> ssaoAlpha;
		ofParameter<float> speedAnim;

		int counter = 1;
		float animationPosition;


		void setup();
		void draw();
		void update();
		void keyPressed(int key);
		ofxPostProcessing post;
};
