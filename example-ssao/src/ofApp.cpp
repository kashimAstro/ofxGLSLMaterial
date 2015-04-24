#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(0);

    w=ofGetScreenWidth();
    h=ofGetScreenHeight();

    ssao.init(w,h);
    ssao.setDark(0.6f);
    ssao.setNoiseMap(0.f,0.13f);

    imgcapsule="images/skin2.jpg";/* image base shader */
    model.loadModel("models/wolf.dae",true);
    materiale.initMaterial(12,materiale.GENERIC,imgcapsule,"");

    ofDisableArbTex();
    fboscene.allocate(w,h);
    gui.setup();
    gui.add(ssaoConf.set("ssao conf",ofVec3f(0.6f,0.f,0.13f),ofVec3f(0.f),ofVec3f(1.f)));
    gui.setPosition(ofVec3f(420,10,0));
    ssaoConf.addListener(this,&ofApp::confSSAO);
}

void ofApp::confSSAO(ofVec3f & value){
    ssao.setDark(value.x);
    ssao.setNoiseMap(value.y,value.z);
}

void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    
    fboscene.begin();
    ofClear(100,100,100,255);
    materiale.begin(&cam,0.54f,0.f,false,0.f);
		ofTranslate(w/2,h/2+250,0);
		ofRotateY(90);
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

void ofApp::keyPressed(int key){
        if(key == 'q')
            cam.disableMouseInput();
        if(key == 'w')
            cam.enableMouseInput();
        if(key == 'f')
            ofToggleFullscreen();
	if(key == ' ')
	    active=!active;
}
