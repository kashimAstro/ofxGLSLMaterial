#include "ofMain.h"
#include "glsl.h"
#include "colorPicker.h"

class ofxGLSLMaterial : public ofBaseApp {

    public:
        enum MODE{
            REFLECT_1 = 1,
            REFLECT_2 = 2,
            REFLECT_3 = 3,
            GENERIC   = 4,
            NORMALMAP = 5,
        };

        glsl g;
        ofImage simpleRef;
        ofImage lightRef;
        ofImage doubleRefA;
        ofImage doubleRefB;
        ofImage noisemap;
        bool simpleRefHide;
        bool lightRefHide;
        bool doubleRefHide;

        ofShader shader;
        ofCamera *camera;
        ofImage noise;
        colorPicker pick;

        ofxGLSLMaterial(){ }
        ~ofxGLSLMaterial(){ }

        void prepareShaderProgram(string vert, string frag){
                shader.setupShaderFromSource(GL_VERTEX_SHADER, vert);
                shader.setupShaderFromSource(GL_FRAGMENT_SHADER, frag);
                shader.linkProgram();
        }

        int getNumMaterial(){
            return g.getShaderSimpleMaterial().size()-1;
        }

        void prepareNoiseMap(int wn = 150, int hn = 150, int d=30, float s=(ofGetFrameNum()/50.0)) {
            /* start noise map */
            noise.allocate(wn,hn,OF_IMAGE_GRAYSCALE);
            for (int y=0; y<hn; y++) {
                for (int x=0; x<wn; x++) {
                        float a = x ;
                        float b = y ;
                        float c = s;
                        float n = ofNoise(a,b,c) * 255;
                        float color = n>d ? ofMap(n,200,255,0,255) : 0;
                        noise.getPixels()[y*wn+x] = color;
                }
            }
            noise.update();//reloadTexture();
            /* end noise */
        }

        ofImage getNoiseMap() {
            return noise;
        }

        void initMaterial(int index, MODE _m, string Aim, string Bim="") {
            prepareNoiseMap();
            pick.init();

            MODE m =_m;
            if( m == REFLECT_1 && Aim != ""){
                prepareShaderProgram(g.getShaderReflectMaterial()[0], g.getShaderReflectMaterial()[1]);//0,1
                ofDisableArbTex();
                simpleRef.load(Aim);
                ofLog()<<"simple reflect";
                simpleRefHide=true;
                lightRefHide=false;
                doubleRefHide=false;
            }
            if( m == REFLECT_3 && Aim != "" && Bim != "" ){
                prepareShaderProgram(g.getShaderReflectMaterial()[2], g.getShaderReflectMaterial()[3]);//2,3
                ofDisableArbTex();
                doubleRefA.load(Aim);
                doubleRefB.load(Bim);
                ofLog()<<"double image reflect";
                doubleRefHide=true;
                simpleRefHide=false;
                lightRefHide=false;
            }
            if( m == REFLECT_2 && Aim != ""){
                prepareShaderProgram(g.getShaderReflectMaterial()[4], g.getShaderReflectMaterial()[5]);//4,5
                ofDisableArbTex();
                lightRef.load(Aim);
                ofLog()<<"light image reflect";
                lightRefHide=true;
                simpleRefHide=false;
                doubleRefHide=false;
            }

            if( m == GENERIC && Aim != ""){
                if(index==0)
                    index=1;
                prepareShaderProgram(g.getShaderSimpleMaterial()[0], g.getShaderSimpleMaterial()[index]);//0,1
                ofDisableArbTex();
                simpleRef.load(Aim);
                simpleRefHide=true;
                doubleRefHide=false;
                lightRefHide=false;
                ofLog()<<"Material -> index:"<<index<<" image-path::"<<Aim;
            }

            if( m == NORMALMAP ){
                if(index==0)
                    index=1;
                prepareShaderProgram(g.normalMap()[0], g.normalMap()[index]);//0,1
                ofDisableArbTex();
                simpleRefHide=false;
                doubleRefHide=false;
                lightRefHide=false;
                ofLog()<<"normal map"<<endl;
            }

            ofAddListener(ofEvents().mouseDragged, this, &ofxGLSLMaterial::mouse);
        }

        void mouse(ofMouseEventArgs & mouse) {
            pick.mpress(mouse.x,mouse.y);
        }

        void drawPicker() {
            pick.draw(ofPoint(0,0,0));
        }

	ofColor getColorPicker(){
		return pick.getColor();
	}

        void begin(ofCamera *cam, ofColor _pik, float times = ofGetElapsedTimef(), float xmax=0.f, bool noisep=false, int disturb=30) {
            camera=cam;
            if(noisep) {
                prepareNoiseMap(150, 150, disturb);
            }
            shader.begin();
            if(doubleRefHide){
                shader.setUniform3f("CameraPos",camera->getPosition().x, camera->getPosition().y, camera->getPosition().z);
                shader.setUniformMatrix4f("ModelWorld4x4",camera->getLocalTransformMatrix());
                shader.setUniformTexture("frontMap",  doubleRefA.getTexture(), 1);
                shader.setUniformTexture("backMap",   doubleRefB.getTexture(), 2);
            }
            else if(lightRefHide){
                shader.setUniformTexture("texture",    lightRef.getTexture(),   1);
            }
            else if(simpleRefHide){
                shader.setUniformTexture("texture",   simpleRef.getTexture(),  1);
                shader.setUniformTexture("bumpmap",   noise.getTexture(),  2);
                shader.setUniform1f("maxHeight",      xmax);
                shader.setUniform1f("time",           times);
                shader.setUniform3f("colors",         ofMap(_pik.r,0,255,0.f,1.f),ofMap(_pik.g,0,255,0.f,1.f),ofMap(_pik.b,0,255,0.f,1.f));
            }
            ofEnableDepthTest();
            /**/
       }

       void end(){
            ofDisableDepthTest();
            shader.end();
       }
};
