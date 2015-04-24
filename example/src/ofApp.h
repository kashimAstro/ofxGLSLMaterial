#include "ofMain.h"
#include "ofxGui.h"
#include "ofxAssimpModelLoader.h"
#include "ofxGLSLMaterial.h"

class ofApp : public ofBaseApp {
	public:
		ofEasyCam cam;
		int w,h;

		ofxGLSLMaterial materiale;
		ofxAssimpModelLoader model;
		ofxPanel gui;
		ofParameter<int> selectIndex;
	        ofParameter<float> maxh;
		ofParameter<bool> double_reflect;
		ofParameter<bool> light_reflect;
		ofParameter<bool> simple_reflect;
	        ofParameter<bool> normal_map;
	        ofParameter<float> sliderTime;
		ofParameter<bool> timechange;
		ofParameter<bool> noisestop;
		ofParameter<int> disturbo;
		ofParameter<string> label;
		ofParameter<ofColor> bg;

		bool hide;
		string imgcapsule,imgcapsule2;

		void setup();
		void selectMaterial(int & value);
		void noisemap(int & value);
		void doubleRef(bool & value);
		void lightRef(bool & value);
		void simpleRef(bool & value);
		void normalMap(bool & value);
		void changeshape(bool & value);

		void darkMatterConf(float & value);
		void noiseMatterConf(float & value);



		void update();
		void draw();
		void keyPressed(int key);
};
