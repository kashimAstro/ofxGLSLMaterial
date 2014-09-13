#include "ofAppGLFWWindow.h"
#include "ofMain.h"
#include "ofxAssimpModelLoader.h"

class ofApp : public ofBaseApp {

  public:

    ofImage img;
    ofImage env;
    ofImage env1;
    ofShader shader;
    ofEasyCam cam;
    ofxAssimpModelLoader model;

    bool reflect  = false;
    bool sreflect = false;
    bool cube     = false;

    void setup() {
        ofEnableDepthTest();
        ofDisableArbTex();

        img.loadImage("img1.jpg");
        env.loadImage("env.jpg");
        env1.loadImage("env1.jpg");


        shader.load("material.vert","material.frag");
	model.loadModel("wolf.dae", true);
    }

    void update(){

    }
    
    void draw() {
        ofBackgroundGradient(64, 0);

	if(reflect || sreflect) 
		env.draw(0,0,env.getWidth()/8,env.getHeight()/8);
	if(sreflect) 
		env1.draw(env.getWidth()/8,0,env1.getWidth()/8,env1.getHeight()/8);

	ofEnableDepthTest();
        cam.begin();
        shader.begin();

	if(sreflect){
//        	shader.setUniform3f("CameraPos",cam.getGlobalPosition().x, cam.getGlobalPosition().y, cam.getGlobalPosition().z);
//		shader.setUniformMatrix4f("ModelWorld4x4",cam.getGlobalTransformMatrix());
        	shader.setUniform3f("CameraPos",cam.getPosition().x, cam.getPosition().y, cam.getPosition().z);
		shader.setUniformMatrix4f("ModelWorld4x4",cam.getLocalTransformMatrix());
	        shader.setUniformTexture("frontMap",  env,1);
	        shader.setUniformTexture("backMap",  env1,2);
	}

	if(reflect){
        	shader.setUniformTexture("colorMap",env1,1);
	        shader.setUniformTexture("envMap",  env,2);
	}else if(sreflect==false){
        	shader.setUniformTexture("texture", img, 1);
        	shader.setUniform1f("time", ofGetElapsedTimef());
	}

	if(cube)
		ofDrawBox(200);
	else {
		ofTranslate(0,-150,0);
		ofRotateX(-90);
		ofRotateY(-90);
		ofRotateZ(45);
		model.drawFaces();
	}

        shader.end();
        cam.end();
	ofDisableDepthTest();

        ofDrawBitmapString(ofToString((int) ofGetFrameRate()), 10, 20);
    }

    void keyPressed(int key) {
	if(key == '1') { model.loadModel("wolf.dae"); cube=false; }
	if(key == '2') { model.loadModel("elephant.dae"); cube=false; }
	if(key == '3') { model.loadModel("alduin.obj"); cube=false; }
	if(key == '4') cube=!cube;


	if(key == ' ') ofToggleFullscreen();

	if(key == 'q') { shader.load("material.vert","material.frag"); reflect=false; sreflect=false; }
	if(key == 'w') { shader.load("material.vert","material1.frag"); reflect=false; sreflect=false; }
	if(key == 'e') { shader.load("material.vert","material2.frag"); reflect=false; sreflect=false; }
	if(key == 'r') { shader.load("material.vert","material3.frag"); reflect=false; sreflect=false; }
	if(key == 't') { shader.load("material.vert","material4.frag"); reflect=false; sreflect=false; }
	if(key == 'y') { shader.load("material.vert","material5.frag"); reflect=false; sreflect=false; }

	if(key == 'u') { shader.load("material.vert","material6.frag"); reflect=false; sreflect=false; }
	if(key == 'i') { shader.load("material.vert","material7.frag"); reflect=false; sreflect=false; }
	if(key == 'o') { shader.load("material.vert","material8.frag"); reflect=false; sreflect=false; }
	if(key == 'p') { shader.load("material.vert","material9.frag"); reflect=false; sreflect=false; }
	if(key == 'a') { shader.load("material.vert","material10.frag"); reflect=false; sreflect=false; }
	if(key == 's') { shader.load("material.vert","material11.frag"); reflect=false; sreflect=false; }
	if(key == 'd') { shader.load("material.vert","material12.frag"); reflect=false; sreflect=false; }
	if(key == 'f') { shader.load("material.vert","material13.frag"); reflect=false; sreflect=false; }
	if(key == 'g') { shader.load("material.vert","material14.frag"); reflect=false; sreflect=false; }
	if(key == 'h') { shader.load("material.vert","material15.frag"); reflect=false; sreflect=false; }
	if(key == 'j') { shader.load("material.vert","material16.frag"); reflect=false; sreflect=false; }
	if(key == 'k') { shader.load("material.vert","material17.frag"); reflect=false; sreflect=false; }
	if(key == 'l') { shader.load("material.vert","material18.frag"); reflect=false; sreflect=false; }
	if(key == 'z') { shader.load("material.vert","material19.frag"); reflect=false; sreflect=false; }
	if(key == 'x') { shader.load("material.vert","material20.frag"); reflect=false; sreflect=false; }

	if(key == 'c') { shader.load("material_reflect.vert","material_reflect.frag"); reflect=true; sreflect=false; }
	if(key == 'v') { shader.load("material_reflect_1.vert","material_reflect_1.frag"); reflect=true; sreflect=false; }
	if(key == 'b') { shader.load("material_reflect_2.vert","material_reflect_2.frag"); sreflect=true; reflect=false; }
    }
};

int main() {
	ofAppGLFWWindow window;
	ofSetupOpenGL(&window, 1024, 768, OF_WINDOW);
	ofRunApp(new ofApp());
}
