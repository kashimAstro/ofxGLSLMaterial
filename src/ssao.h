#include "ofMain.h"

#define STRINGIFY(A) #A

class SSAO {

public:
        float disorder;
        float dark;
        int wNoise;
        int hNoise;
        ofPoint resolution;
        ofFbo scene;
        ofFbo outScene;
        ofImage noiseMap;
        ofShader ssaoShader;

        SSAO(){
            string ssaoFrag = STRINGIFY(
                    \n#version 120\n
                    uniform sampler2D tex0;
                    uniform sampler2D tex1;
                    uniform float dark;
                    uniform vec2 resolution;
                    uniform vec2 tRes1;
                    void main (void){
                        vec2 uv = gl_TexCoord[0].st/resolution.xy;
                        float zr = 1.0-texture2D( tex0, uv ).x;
                        float ao = 0.0;
                        //
                        for( int i=0; i<12; i++ ){
                                vec2 off = -1.0 + 2.0*texture2D( tex1, (gl_TexCoord[0].xy + 23.71*float(i))/tRes1.xy ).xz;
                                float z = 1.0-texture2D( tex0, (gl_TexCoord[0].xy + floor(off*16.0))/resolution.xy ).x;
                                ao += clamp( (zr-z)/0.1, 0.0, dark);
                        }
                        //
                        ao = clamp( 1.0 - ao/8.0, 0.0, 8.0 );
                        vec3 col = vec3(ao);
                        col *= texture2D( tex0, uv ).xyz;
                        gl_FragColor = vec4(col,1.0);
                    }
            );
            string ssaoVert = STRINGIFY(
                    \n#version 120\n
                    void main() {
                        gl_Position = ftransform();
                        gl_TexCoord[0] = gl_MultiTexCoord0;
                    }
            );
            ssaoShader.setupShaderFromSource(GL_VERTEX_SHADER, ssaoVert);
            ssaoShader.setupShaderFromSource(GL_FRAGMENT_SHADER, ssaoFrag);
            ssaoShader.linkProgram();

            //ofDisableArbTex();
            resolution.set(ofGetScreenWidth(),ofGetScreenHeight());
            dark=0.4;
        }

        ~SSAO(){

        }

        void prepareNoiseMap(int wn=50,int hn=50,float d=35.7, float s=(ofGetFrameNum()/50.0)) {
            noiseMap.allocate(wn,hn,OF_IMAGE_GRAYSCALE);
            for (int y=0; y<hn; y++) {
                for (int x=0; x<wn; x++) {
                    float a = x ;
                    float b = y ;
                    float c = s;
                    float n = ofNoise(a,b,c) * 255;
                    float color = n>d ? ofMap(n,200,255,0,255) : 0;
                    noiseMap.getPixels()[y*wn+x] = color;
                }
            }
            noiseMap.reloadTexture();
            wNoise=wn;
            hNoise=hn;
        }

        void init(int WScene,int HScene){
            scene.allocate(WScene,HScene);
            prepareNoiseMap();
        }

        void setNoiseMap(float d=35.7, float s=(ofGetFrameNum()/50.0)) {
            int wn=wNoise;
            int hn=hNoise;
            for (int y=0; y<hn; y++) {
                for (int x=0; x<wn; x++) {
                    float a = x ;
                    float b = y ;
                    float c = s;
                    float n = ofNoise(a,b,c) * 255;
                    float color = n>d ? ofMap(n,200,255,0,255) : 0;
                    noiseMap.getPixels()[y*wn+x] = color;
                }
            }
            noiseMap.reloadTexture();
        }

        void setResolution(ofPoint _resolution){
            resolution=_resolution;
        }

        void setDark(float _dark){
            dark=_dark;
        }

        void setOutputScene(ofFbo img){
            outScene=img;
        }

        void draw(){
            ssaoShader.begin();
			ssaoShader.setUniformTexture("tex0",outScene.getTextureReference(),0);
			ssaoShader.setUniformTexture("tex1",noiseMap.getTextureReference(),1);
			ssaoShader.setUniform2f("resolution",resolution.x,resolution.y);
			ssaoShader.setUniform2f("tRes1",noiseMap.width,noiseMap.height);
			ssaoShader.setUniform1f("dark",dark);
                scene.draw(0,0);
            ssaoShader.end();
        }
};
