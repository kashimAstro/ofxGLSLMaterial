#include "ofMain.h"

class colorPicker {
	public:
	ofImage colorPicker;
	ofParameter<ofColor> colors;
	ofPoint pos;
	ofImage p;
	float w,h,cx,cy;

	ofImage getPicker(float _w=250,float _h=250){
	    float w=_w;
	    float h=_h;
	    float cx = w/2;
	    float cy = h/2;

	    p.allocate(w,h,OF_IMAGE_COLOR);
	    for (float y=0; y<h; y++) {
	        for (float x=0; x<w; x++) {
	            float angle = atan2(y-cy,x-cy)+PI;
	            float dist = ofDist(x,y,cx,cy);
	            float hue = angle/TWO_PI*255;
	            float sat = ofMap(dist,0,w/4,0,255,true);
	            float bri = ofMap(dist,w/4,w/2,255,0,true);
		    p.setColor(x,y,ofColor::fromHsb(hue,sat,bri));
	        }
	    }
	    p.update();//reloadTexture();
	    return p;
	}

	void init(){
		ofImage i = getPicker();
		colorPicker=i;
	}

	void draw(ofPoint _pos){
		pos=_pos;
		ofTranslate(pos);
		ofPushMatrix();

		ofPushStyle();
		   ofSetColor(colors.get());
		   ofRect(0,colorPicker.getHeight(),colorPicker.getWidth(),40);
		ofPopStyle();

		ofPushStyle();
                   colorPicker.draw(0,0);
		ofPopStyle();

		ofPopMatrix();
	}

	ofColor getColor(){
		return colors.get();
	}

	void mpress(int x, int y){
		//unsigned char *pixels=colorPicker.getPixels();
		unsigned char *pixels=colorPicker.getPixels().getData();
		if(x<=colorPicker.getWidth()+pos.x && y<=colorPicker.getHeight()+pos.y ){
			int index = y*colorPicker.getWidth()*3 + x*3;
			int red = pixels[index];
			int green = pixels[index+1];
			int blue = pixels[index+2];
			colors.set(ofColor(red,green,blue));
			ofLog()<<colors.get();
		}
	}
};
/*end*/
