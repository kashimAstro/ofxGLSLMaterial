#include "ofAppGLFWWindow.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp {

  public:

    ofImage img;
    ofVboMesh mesh;
    ofShader shader;
    ofEasyCam cam;
    ofxAssimpModelLoader model;
    
    void setup() {
        ofEnableDepthTest();
        ofDisableArbTex();
        img.loadImage("img.jpg");
        shader.load("material.vert","material.frag");
	model.loadModel("wolf.dae", true);
    }
    
    void draw() {
        ofBackgroundGradient(64, 0);
        cam.begin();

        shader.begin();
        shader.setUniformTexture("texture", img, 1);
        shader.setUniform1f("time", ofGetElapsedTimef());
	ofTranslate(0,-150,0);
	ofRotateX(-90);
	ofRotateY(-90);
	ofRotateZ(45);


	model.drawFaces();

        shader.end();

        cam.end();
        ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    }

    void keyPressed(int key) {
	if(key == '1') model.loadModel("wolf.dae");
	if(key == '2') model.loadModel("elephant.dae");

	if(key == ' ') ofToggleFullscreen();

	if(key == 'q') shader.load("material.vert","material.frag");
	if(key == 'w') shader.load("material.vert","material1.frag");
	if(key == 'e') shader.load("material.vert","material2.frag");
	if(key == 'r') shader.load("material.vert","material3.frag");
	if(key == 't') shader.load("material.vert","material4.frag");
	if(key == 'y') shader.load("material.vert","material6.frag");
	if(key == 'u') shader.load("material.vert","material7.frag");
	if(key == 'i') shader.load("material.vert","material8.frag");
	if(key == 'o') shader.load("material.vert","material9.frag");
	if(key == 'p') shader.load("material.vert","material10.frag");
	if(key == 'a') shader.load("material.vert","material11.frag");
	if(key == 's') shader.load("material.vert","material12.frag");
	if(key == 'd') shader.load("material.vert","material13.frag");
	if(key == 'f') shader.load("material.vert","material14.frag");
	if(key == 'g') shader.load("material.vert","material15.frag");
    }
};

int main() {
	ofAppGLFWWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp(new ofApp());
}
