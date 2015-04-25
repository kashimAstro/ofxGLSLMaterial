#include "ofApp.h"

int counter = 0;
float speedanim;
float animationPosition;
bool bAnimate;

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);

    w=ofGetScreenWidth();
    h=ofGetScreenHeight();

    ssao.init(w,h);

    imgcapsule="images/10-tessellation.jpg";/* image base shader */
    model.loadModel("models/omino.dae",true);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    if(!bAnimate) {
           model.setPausedForAllAnimations(true);
    }
    speedanim=30;

    materiale.initMaterial(12,materiale.GENERIC,imgcapsule,"");

    ofDisableArbTex();
    ofFbo::Settings settings;
    settings.numSamples = 8;
    settings.useDepth = true;
    settings.width = w;
    settings.height = h;
    fboscene.allocate(settings);

    gui.setup();
    gui.add(ssaoAlpha.set("alpha ssao",0.2f,0.f,1.f));
    gui.add(ssaoNoise.set("ssao conf",0.14f,0.f,1.f));
    gui.setPosition(ofVec3f(420,10,0));
    ssaoNoise.addListener(this,&ofApp::confSSAO);

    bg.loadImage("images/bg.jpg");
    ssao.setNoiseMap(0.f,0.13f);
}

void ofApp::confSSAO(float & value){
    ssao.setNoiseMap(0.f,value);
}

void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    model.update();
    animationPosition = ofMap(counter,0,speedanim,0.0,1.0);
    counter++;
    if(counter>speedanim) counter=0;
    model.setPositionForAllAnimations(animationPosition);
  
    ssao.setDark(ssaoAlpha);
   
    fboscene.begin();
    ofClear(100,100,100,255);
    bg.draw(0,0,w,h);

    /* shadow fake */
    ofPushMatrix();
    ofPushStyle();
    ofTranslate(w/2,h/2+350,-270);
    ofRotateY(50);
    ofRotateZ(-165);
    ofSetColor(0,0,0,110);
    		model.drawFaces();
    ofPopStyle();
    ofPopMatrix();
    /**/

    materiale.begin(&cam,ofGetElapsedTimef(),0.f,false,0.f);
		ofTranslate(w/2,h/2+350,-270);
		ofRotateY(45);
	        model.drawFaces();
    materiale.end();
    fboscene.end();
    ssao.setOutputScene(fboscene);
}

void ofApp::draw(){
    
    ssao.draw();

    materiale.drawPicker();

    ssao.getNoiseMap().draw(255,0);
    gui.draw();
}
int j=0;
void ofApp::keyPressed(int key){
        if(key == 'q')
            cam.disableMouseInput();
        if(key == 'w')
            cam.enableMouseInput();
        if(key == 'f')
            ofToggleFullscreen();
	if(key == ' ')
	    active=!active;
	if(key == OF_KEY_RIGHT){
	    if(j>materiale.getNumMaterial())
		j=0;
	    ofLog()<<j;
	    materiale.initMaterial(j,materiale.GENERIC,imgcapsule,"");
	    j++;
	}


}
