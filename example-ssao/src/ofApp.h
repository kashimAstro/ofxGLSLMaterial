#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGLSLMaterial.h"
#include "ofxSSAOFake.h"

class ofApp : public ofBaseApp {
	public:
		ofEasyCam cam;
		int w,h;
		bool active;

		ofxGLSLMaterial materiale;
		ofxAssimpModelLoader model;
		string imgcapsule;
		ofFbo fboscene;
		ofxPanel gui;
		ofParameter<ofVec3f> ssaoConf;

		void setup();
		void draw();
		void update();
void confSSAO(ofVec3f & value);
		void keyPressed(int key);
		ofxSSAOFake ssao;
};
