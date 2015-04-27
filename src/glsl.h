#include "ofMain.h"

#define STRINGIFY(A) #A

class glsl {
    public:
        vector<string> getShaderReflectMaterial();
        vector<string> getShaderSimpleMaterial();
        vector<string> normalMap();
};
