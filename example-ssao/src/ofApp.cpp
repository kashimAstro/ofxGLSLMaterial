#include "ofApp.h"

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
    model.setPausedForAllAnimations(true);

    materiale.initMaterial(13,materiale.GENERIC,imgcapsule,"");
    materiale1.initMaterial(16,materiale.GENERIC,imgcapsule,"");
    materiale2.initMaterial(21,materiale.GENERIC,imgcapsule,"");

    ofDisableArbTex();
    ofFbo::Settings settings;
    settings.numSamples = 8;
    settings.useDepth = true;
    settings.width = w;
    settings.height = h;
    fboscene.allocate(settings);

    gui.setup();
    gui.add(ssaoAlpha.set("alpha ssao",0.15,0.,1.));
    gui.add(ssaoNoise.set("ssao conf",0.14,0.,1.));
    gui.add(speedAnim.set("speed anim",20,1,100));

    gui.setPosition(ofVec3f(420,10,0));
    ssaoNoise.addListener(this,&ofApp::confSSAO);

    bg.loadImage("images/img1.jpg");
    ssao.setNoiseMap(0.f,0.13f);
}

void ofApp::confSSAO(float & value){
    ssao.setNoiseMap(0.f,value);
}

void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    model.update();
    animationPosition = ofMap(counter,0,speedAnim,0.0,1.0);
    counter++;
    if(counter>speedAnim) counter=1;
    model.setPositionForAllAnimations(animationPosition);
  
    ssao.setDark(ssaoAlpha);
   
    fboscene.begin();
    ofClear(100,100,100,255);

    bg.draw(0,0,w,h);

    ofPushMatrix();
    materiale.begin(&cam,materiale.getColorPicker(),ofGetElapsedTimef(),0.0f,true,0.f);
		ofTranslate(w/2+300,h/2+400,-270);
	        model.drawFaces();
    materiale.end();
    ofPopMatrix();

    ofPushMatrix();
    materiale1.begin(&cam,materiale.getColorPicker(),ofGetElapsedTimef(),0.4f,true,0.f);
		ofTranslate(w/2-80,h/2+400,-270);
	        model.drawFaces();
    materiale1.end();
    ofPopMatrix();

    ofPushMatrix();
    materiale2.begin(&cam,materiale.getColorPicker(),ofGetElapsedTimef(),1.3f,true,0.f);
		ofTranslate(w/2-500,h/2+400,-270);
	        model.drawFaces();
    materiale2.end();
    ofPopMatrix();

    fboscene.end();
    ssao.setOutputScene(fboscene);
}

void ofApp::draw(){
    
    ssao.draw();

    materiale.drawPicker();

    ssao.getNoiseMap().draw(255,0);
    gui.draw();
}

int l,g,j=0;
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
	if(key == OF_KEY_UP){
	    if(l>materiale1.getNumMaterial())
		l=0;
	    ofLog()<<l;
	    materiale1.initMaterial(l,materiale1.GENERIC,imgcapsule,"");
	    l++;
	}
	if(key == OF_KEY_LEFT){
	    if(g>materiale2.getNumMaterial())
		g=0;
	    ofLog()<<g;
	    materiale2.initMaterial(g,materiale2.GENERIC,imgcapsule,"");
	    g++;
	}


}
