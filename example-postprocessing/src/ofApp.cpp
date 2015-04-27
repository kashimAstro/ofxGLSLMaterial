#include "ofApp.h"

void ofApp::setup(){
    ofSetFrameRate(60);
    ofBackground(215,160,0);

    w=ofGetScreenWidth();
    h=ofGetScreenHeight();

    post.init(w,h);
    post.createPass<FxaaPass>()->setEnabled(true);
    post.createPass<DofPass>()->setEnabled(true);
    post.createPass<SSAOPass>()->setEnabled(true);

    imgcapsule="images/10-tessellation.jpg";/* image base shader */
    model.loadModel("models/omino.dae",true);
    model.setLoopStateForAllAnimations(OF_LOOP_NORMAL);
    model.playAllAnimations();
    model.setPausedForAllAnimations(true);

    materiale.initMaterial(21,materiale.GENERIC,imgcapsule,"");
    materiale1.initMaterial(21,materiale.GENERIC,imgcapsule,"");
    materiale2.initMaterial(21,materiale.GENERIC,imgcapsule,"");

    ofDisableArbTex();
    ofFbo::Settings settings;
    settings.numSamples = 8;
    settings.useDepth = true;
    settings.width = w;
    settings.height = h;
    fboscene.allocate(settings);

    gui.setup();
    gui.add(speedAnim.set("speed anim",20,1,100));
    gui.setPosition(ofVec3f(420,10,0));

    bg.loadImage("images/img1.jpg");
}

void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    model.update();
    animationPosition = ofMap(counter,0,speedAnim,0.0,1.0);
    counter++;
    if(counter>speedAnim) counter=1;
    model.setPositionForAllAnimations(animationPosition);
}

void ofApp::draw(){
    
    post.begin(cam);
//  cam.begin();
    ofTranslate(-w/2,10,0);
    ofPushMatrix();
    materiale.begin(&cam,ofColor(255,0,0),ofGetElapsedTimef(),0.0f,false,0.f);
		ofTranslate(w/2+450,h/2+400,-270);
	        model.drawFaces();
    materiale.end();
    ofPopMatrix();

    ofPushMatrix();
    materiale1.begin(&cam,ofColor(0,255,0),ofGetElapsedTimef(),1.25f,true,240.f);
		ofTranslate(w/2+100,h/2+400,-270);
	        model.drawFaces();
    materiale1.end();
    ofPopMatrix();

    ofPushMatrix();
    materiale2.begin(&cam,ofColor(0,0,255),ofGetElapsedTimef(),1.3f,false,0.f);
		ofTranslate(w/2-300,h/2+400,-270);
	        model.drawFaces();
    materiale2.end();
    ofPopMatrix();
//  cam.end();

    post.end();


    materiale.drawPicker();

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
