#include "ofMain.h"
#include "glsl.h"

vector<string> glsl::getShaderReflectMaterial(){
    vector<string> storage;

    /* vertex and fragment simple reflect image */
    string simpleRefVertMat = STRINGIFY(
            void main(){
                gl_Position = ftransform();
                gl_TexCoord[0] = gl_MultiTexCoord0;
                vec3 u = normalize( vec3(gl_ModelViewMatrix * gl_Vertex) );
                vec3 n = normalize( gl_NormalMatrix * gl_Normal );
                vec3 r = reflect( u, n );
                float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
                gl_TexCoord[1].s = r.x/m + 0.5;
                gl_TexCoord[1].t = r.y/m + 0.5;
            }
    );

    string simpleRefFragtMat = STRINGIFY(
            uniform sampler2D colorMap;
            uniform sampler2D texture;
            void main (void) {
                vec4 env = texture2D( texture, gl_TexCoord[1].st);
                gl_FragColor = /*colorMap*/ env*0.9;
            }
    );
    storage.push_back(simpleRefVertMat);//0
    storage.push_back(simpleRefFragtMat);//1
    /* simple reflect */

    /* vertex double image reflect */
    string doubleRefVertMat = STRINGIFY(
            uniform mat4 ModelWorld4x4;
            uniform vec3 CameraPos;
            varying vec3 R;
            mat3 GetLinearPart( mat4 m ) {
                mat3 result;
                result[0][0] = m[0][0];
                result[0][1] = m[0][1];
                result[0][2] = m[0][2];
                result[1][0] = m[1][0];
                result[1][1] = m[1][1];
                result[1][2] = m[1][2];
                result[2][0] = m[2][0];
                result[2][1] = m[2][1];
                result[2][2] = m[2][2];
                return result;
            }
            void main() {
                gl_Position = ftransform();
                mat3 ModelWorld3x3 = GetLinearPart( ModelWorld4x4 );
                vec4 WorldPos = ModelWorld4x4 *  gl_Vertex;
                vec3 N = normalize( ModelWorld3x3 * gl_Normal );
                vec3 E = normalize( WorldPos.xyz - CameraPos.xyz );
                R = reflect( E, N );
            }
    );

    string doubleRefFragMat  = STRINGIFY(
            uniform sampler2D frontMap;
            uniform sampler2D backMap;
            varying vec3 R;
            void main (void){
                    vec4 output_color;
                    vec3 vR = normalize(R);
                    if(vR.z>0.0){
                            vec2 frontUV;
                            frontUV = (vR.xy / (2.0*(1.0 + vR.z))) + 0.5;
                            output_color = texture2D( frontMap, frontUV );
                    } else {
                            vec2 backUV;
                            backUV = (vR.xy / (2.0*(1.0 - vR.z))) + 0.5;
                            output_color = texture2D( backMap, backUV );
                    }
                    gl_FragColor = output_color;
            }
    );
    storage.push_back(doubleRefVertMat);//2
    storage.push_back(doubleRefFragMat);//3
    /* end double reflect */

    /* vertex and fragment light reflect image */
    string lRefVertMat  = STRINGIFY(
            \n#version 120\n
            varying vec3 normal;
            varying vec3 lightDir;
            varying vec3 eyeVec;
            void main(){
                gl_Position = ftransform();
                vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
                lightDir = vec3(gl_LightSource[0].position.xyz-vVertex);
                eyeVec = -vVertex;
                vec3 u = normalize( vVertex );
                normal = normalize( gl_NormalMatrix * gl_Normal );
                vec3 r = reflect( u, normal );
                float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
                gl_TexCoord[0].s = r.x/m + 0.5;
                gl_TexCoord[0].t = r.y/m + 0.5;
            }
    );
    string lRefFragMat  = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            varying vec3 normal;
            varying vec3 lightDir;
            varying vec3 eyeVec;
            void main (void) {
                    vec4 final_color = (gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient);
                    final_color+=(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);
                    vec3 N = normalize(normal);
                    vec3 L = normalize(lightDir);
                    float lambertTerm = dot(N,L);
                    if(lambertTerm > 0.0){
                            final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
                            final_color += texture2D( texture, gl_TexCoord[0].st) * lambertTerm;
                    }
                    gl_FragColor = final_color;
                    gl_FragColor.a = 0.9;
            }
    );
    storage.push_back(lRefVertMat);//4
    storage.push_back(lRefFragMat);//5
    /* end light reflect */
    return storage;
}

vector<string> glsl::getShaderSimpleMaterial(){
    vector<string> storage;

    /*uniform sampler2D colormap;
    uniform sampler2D bumpmap;
    varying vec2  TexCoord;
    uniform int maxHeight;

    void main(void) {
        TexCoord = gl_MultiTexCoord0.st;
        vec4 bumpColor = texture2D(bumpmap, TexCoord);
        float df = 0.30*bumpColor.x + 0.59*bumpColor.y + 0.11*bumpColor.z;
        vec4 newVertexPos = vec4(gl_Normal * df * float(maxHeight), 0.0) + gl_Vertex;
        gl_Position = gl_ModelViewProjectionMatrix * newVertexPos;
    }*/

    /* vertex global material */
    string globalVertMat = STRINGIFY(
            \n#version 120\n
            varying vec3 e;
            varying vec3 n;
            uniform sampler2D bumpmap;
            varying vec2  TexCoord;
            uniform float maxHeight;

            void main() {

                TexCoord = gl_MultiTexCoord0.st;

                e = normalize( vec3( gl_ModelViewMatrix * vec4( gl_Vertex.xyz,1.0 ) ) );
                n = normalize( gl_NormalMatrix * gl_Normal );

                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 p = r.xy / m + .5;

                vec4 bumpColor = texture2D(bumpmap, p);
                float df = 0.30*bumpColor.x + 0.59*bumpColor.y + 0.11*bumpColor.z;
                vec4 newVert = vec4(gl_Normal * df * maxHeight, 0.0) + gl_Vertex;
                gl_Position = /*gl_ProjectionMatrix * gl_ModelViewMatrix */gl_ModelViewProjectionMatrix * newVert;
            }
    );
    storage.push_back(globalVertMat);//0

    /* material 1 */
    string fMat1 = STRINGIFY(
            \n#version 120\n
            \n#define TAU 6.28318530718\n
            \n#define MAX_ITER 5\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            void main( void ) {
                float time = time * .5;
                vec3 r = reflect( e, n );
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 sp = r.xy / m + .5;
                vec2 p = sp*TAU - 20.0;
                vec2 i = p;
                float c = 1.0;
                float inten = .05;
                for (int n = 0; n < MAX_ITER; n++){
                    float t = time * (1.0 - (3.5 / float(n+1)));
                    i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
                    c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
                }
                c /= float(MAX_ITER);
                c = 1.55-sqrt(c);
                vec3 colour = vec3(pow(abs(c), 6.0))*colors;
                gl_FragColor = vec4(clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0), 1.0);
            }
    );
    storage.push_back(fMat1);//1

    /* material 2 */
    string fMat2 = STRINGIFY(
        \n#version 120\n
        uniform sampler2D texture;
        uniform sampler2D texture1;
        uniform float time;
        uniform vec3 colors;
        varying vec3 e;
        varying vec3 n;
        vec2 doit( in vec2 p, in float off, float amp ) {
            float f = 0.0;
            float a = 0.0;
            for( int i=0; i<10; i++ ) {
                float h = float(i)/10.0;
                float g = texture2D( texture, vec2(0.01+h*0.5, 0.25)).x;
                float k = 1.0 + 0.4*g*g;
                vec2 q;
                q.x = sin(time*0.015+0.67*g*(1.0+amp) + off + float(i)*121.45) * 0.5 + 0.5;
                q.y = cos(time*0.016+0.63*g*(1.0+amp) + off + float(i)*134.76) * 0.5 + 0.5;
                vec2 d = p - q;
                float at = 1.0/(0.01+dot(d,d));
                f += k*0.1*at;
                a += 0.5 + 0.5*sin(2.0*atan(d.y,d.x));//*at;
            }
            return vec2(f,a);
        }
        void main(void){
            vec3 r = reflect( e, n );
            r = e - 2. * dot( n, e ) * n;
            float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
            vec2 p = r.xy / m + .5;
            float isTripy = smoothstep( 86.5, 87.5,time )-smoothstep( 100.5, 108.0, time );
            vec2 ref = doit( p, 0.0, isTripy );
            float b = ref.x;
            vec3 col = texture2D( texture,vec2(pow(0.25*ref.x,0.25), 0.5)).xyz*texture2D( texture,vec2(0.1*pow(ref.y,1.2), 0.6)).xyz;
            col = sqrt(col)*2.0;
            vec3 col2 = col;
            col2 = 4.0*col2*(1.0-col2);
            col2 = 4.0*col2*(1.0-col2);
            col2 = 4.0*col2*(1.0-col2);
            col = mix( col, col2, isTripy );
            float useLights = 0.5 + 1.5*smoothstep( 45.0, 45.2,time );
            col += useLights*0.5*pow( b*0.1, 4.0 ) * pow( texture2D( texture, vec2(0.1,0.25) ).x, 2.0 );
            gl_FragColor = vec4( col*colors, 1.0 );;
        }
    );
    storage.push_back(fMat2);//2

    /* material 3 */
    string fMat3 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            void main(void) {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, time*2. ) + pow( r.z + 1., 2. ) );
                vec2 uv = r.xy / m + .5;
                float d = length(uv);
                vec2 st = 0.2*vec2((0.071*time+d),  sin(0.073*time-d));
                vec3 col = texture2D( texture, st ).xyz;
                float w = col.x;
                col *= 1.0 - texture2D( texture, 0.4*uv + 0.1*col.xy  ).xyy;
                col *= w*2.0;
                col *= 1.0 + .5*d*time;
                gl_FragColor = vec4(col*colors,1.0);
            }
    );
    storage.push_back(fMat3);//3

    /* material 4 */
    string fMat4 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            void main(void) {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 uv = r.xy / m + .5;
                float d = length(uv);
                vec2 st = uv*0.1 + 0.2*vec2(cos(0.071*time+d), sin(0.073*time-d));
                vec3 col = texture2D( texture, st ).xyz;
                float w = col.x;
                col *= 1.0 - texture2D( texture, 0.4*uv + 0.1*col.xy  ).xyy;
                col *= w*2.0;
                col *= 1.0 + 2.0*d;
                gl_FragColor = vec4(col*colors,1.0);
            }
    );
    storage.push_back(fMat4);//4

    /* material 5 */
    string fMat5 = STRINGIFY(
            \n#define POINTS 60\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            float t = time;
            float dist2(vec2 P0, vec2 P1) { vec2 D=P1-P0; return dot(D,D); }
            float hash (float i) { return 2.*fract(sin(i*7467.25)*1e5) - 1.; }
            vec2  hash2(float i) { return vec2(hash(i),hash(i-.1)); }
            vec4  hash4(float i) { return vec4(hash(i),hash(i-.1),hash(i-.3),hash(i+.1)); }
            vec2 P(float i) {
                vec4 c = hash4(i);
                return vec2(cos(t*c.x-c.z)+.5*cos(2.765*t*c.y+c.w),( sin(t*c.y-c.w)+.5*sin(1.893*t*c.x+c.z) )/1.5);
            }
            void main(void) {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 uv = r.xy / mm + .5;
                float m =  .1*t/6.283;
                float my = .5*pow(.5*(1.-cos(.1*t)),3.);
                int MODE = 2;
                float fMODE = (1.-cos(6.283*m))/2.;
                const int R = 1;
                float v=0.; vec2 V=vec2(0.);
                for (int i=1; i<POINTS; i++) {
                    vec2 p = P(float(i));
                    for (int y=-R; y<=R; y++) {
                        for (int x=-R; x<=R; x++) {
                            vec2 d = p+2.*vec2(float(x),float(y)) -uv;
                            float r2 = dot(d,d);
                            r2 = clamp(r2,5e-2*my,1e3);
                            V +=  d / r2;
                        }
                    }
                }
                v = length(V);
                v *= 1./(9.*float(POINTS));
                v = clamp(v,0.,.1);
                v *= 2.+fMODE;
                //if (MODE==0) gl_FragColor = vec4(.2*v)*vec4(1,2,2,2);
                //if (MODE==1) gl_FragColor = vec4(.5+.5*sin(2.*v));
                /*if (MODE==2)*/
                gl_FragColor = vec4(vec3(sin(v),sin(v/2.),sin(v/4.))*colors,1.);
            }
    );
    storage.push_back(fMat5);//5

    /* material 6 */
    string fMat6 = STRINGIFY(
        \n#version 120\n
        uniform sampler2D texture;
        uniform sampler2D texture1;
        uniform float time;
        uniform vec3 colors;
        varying vec3 e;
        varying vec3 n;
        void main( void ) {
            vec3 r = reflect( e, n );
            r = e - 2. * dot( n, e ) * n;
            float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
            vec2 uv = r.xy / m + .5;
            vec2 e = 1.0/uv;
            float am1 = 0.5 + 0.5*sin( time );
            float am2 = 0.5 + 0.5*cos( time );
            for( int i=0; i<50; i++ ) {
                float h  = dot( texture2D(texture, uv,               -100.0).xyz, vec3(0.333) );
                float h1 = dot( texture2D(texture, uv+vec2(e.x,0.0), -100.0).xyz, vec3(0.333) );
                float h2 = dot( texture2D(texture, uv+vec2(0.0,e.y), -100.0).xyz, vec3(0.333) );
                vec2 g = 0.001*vec2( (h1-h), (h2-h) )/e;
                vec2 f = g.yx*vec2(-1.0,1.0);
                g = mix( g, f, am1 );
                uv -= 0.01*g*am2;
            }
            vec3 col = texture2D(texture, uv).xyz;
            col *= 2.0;
            gl_FragColor = vec4(col*colors, 1.0);
        }
    );
    storage.push_back(fMat6);//6

    /* material 7 */
    string fMat7 = STRINGIFY(
            \n#version 120\n
            \n#define SQRT2	1.41421356237\n
            \n#define SQRT3	1.73205080757\n
            \n#define SIZE 	10\n
            \n#define HASH_MAGNITUDE (6.0 / (SQRT2 + 1.0) - 1.0)\n
            \n#define KERNEL 2\n
            \n#define BORDER\n
            //#define CENTER
            \n#define DISTANCE\n
            \n#define STRIPS\n
            \n#define HEXAGONAL\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            float hash (in int index) {
                float x = float (index);
                return HASH_MAGNITUDE * 0.5 * sin (sin (x) * x + sin (x * x) * time);
            }
            vec2 pointInCell (in ivec2 cell) {
                int index = cell.x + cell.y * SIZE;
                vec2 point = vec2 (cell);
                \n#ifdef HEXAGONAL\n
                point.x += fract (point.y * 0.5) - 0.25;
                \n#endif\n
                return point + vec2 (hash (index), hash (index + 1)) * (0.5 + 0.5 * sin (time * 0.5));
            }
            void main () {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 p = float (SIZE) *r.xy / m + .5;
                \n#ifdef HEXAGONAL\n
                p.y /= SQRT3 * 0.5;
                \n#endif\n
                ivec2 pCell = ivec2 (floor (p + 0.5));
                float dMin = HASH_MAGNITUDE + 1.0;
                vec2 pqMin;
                ivec2 minCell;
                for (int y = -KERNEL; y <= KERNEL; ++y) {
                    for (int x = -KERNEL; x <= KERNEL; ++x) {
                        ivec2 qCell = pCell + ivec2 (x, y);
                        vec2 pq = pointInCell (qCell) - p;
                        \n#ifdef HEXAGONAL\n
                        pq.y *= SQRT3 * 0.5;
                        \n#endif\n
                        float d = dot (pq, pq);
                        if (d < dMin) {
                            dMin = d;
                            pqMin = pq;
                            minCell = qCell;
                        }
                    }
                }
                int col = minCell.x + minCell.y * SIZE;
                vec4 color = 0.6 + vec4 (hash (col), hash (col + 1), hash (col + 2), 1.0) * 0.8 / HASH_MAGNITUDE;
                \n#ifdef CENTER\n
                dMin = sqrt (dMin);
                \n#else\n
                dMin = HASH_MAGNITUDE + 1.0;
                \n#endif\n
                \n#ifdef BORDER\n
                for (int y = -KERNEL; y <= KERNEL; ++y) {
                    for (int x = -KERNEL; x <= KERNEL; ++x) {
                        ivec2 qCell = pCell + ivec2 (x, y);
                        if (qCell != minCell) {
                            vec2 pq = pointInCell (qCell) - p;
                            \n#ifdef HEXAGONAL\n
                            pq.y *= SQRT3 * 0.5;
                            \n#endif\n
                            dMin = min (dMin, dot (0.5 * (pqMin + pq), normalize (pq - pqMin)));
                        }
                    }
                }
                \n#endif\n
                color *= smoothstep (0.02, 0.1, dMin);
                \n#ifdef DISTANCE\n
                color *= 0.9 + 0.1 * sin (dMin * 40.0);
                \n#endif\n
                \n#ifdef STRIPS\n
                float strip = float (col);
                float stripCos = cos (strip);
                float stripSin = sin (strip);
                strip = mix (1.0, sin (40.0 * (pqMin.x * stripCos - pqMin.y * stripSin)), mod (strip * 0.5, 2.0));
                strip *= sin (40.0 * (pqMin.x * stripSin + pqMin.y * stripCos));
                color *= 0.8 + 0.2 * strip;
                \n#endif\n
                gl_FragColor = vec4(color.rgb*colors,1.0);
            }
    );
    storage.push_back(fMat7);//7

    /* material 8 */
    string fMat8 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            float noise(float s) {
                return fract(cos(s*11345.123)*123113.123);
            }
            float Texture(vec2 p) {
                vec3 c=vec3(0);
                float m=noise(p.x);
                float q=floor(p.x*10.);
                p.y+=(floor(noise(q)*1.7)*1.5-1.)*time*0.2;
                p=mod(p+floor(m*10.)/10.,0.1)-0.05;
                float l=pow(1.-length(p),111.93)*1.21;
                return l;
            }
            void main(void){
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 p = r.xy / m + .5;
                p=tan(p*cos(time*0.2)*13.4);
                vec3 c=vec3(0);
                float d=.01/length(p*p);
                d=cos(time*0.5+d)-tan(d);
                p.x+=0.12*cos(d*50.1+time);
                p.y+=0.13*sin(d*50.1+time);
                float a=(atan(p.x,p.y)*0.23)+time*0.05;
                c.x=Texture(vec2(a+time*0.,d))*(15.*length(p)*length(p));
                c.y=Texture(vec2(a*2.1+time*-0.,d*3.3))*(5.*length(p)*length(p));
                c.z=Texture(vec2(a*3.1+time*0.,d*5.3))*(20.*length(p)*length(p));
                c+=vec3(c.z+c.y,c.z+c.x,c.y+c.x)*0.2;
                gl_FragColor = vec4( c*colors, 1.0 );
            }
    );
    storage.push_back(fMat8);//8

    /* material 9 */
    string fMat9 = STRINGIFY(
        \n#version 120\n
        uniform sampler2D texture;
        uniform sampler2D texture1;
        uniform float time;
        uniform vec3 colors;
        varying vec3 e;
        varying vec3 n;
        float lu = 0.06;
        float fmid = 0.2;
        float fwhite = 0.5;
        void main(void){
            vec3 r = reflect( e, n );
            r = e - 2. * dot( n, e ) * n;
            float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
            vec2 uv = r.xy / m + .5;
            vec4 col = texture2D(texture, uv);
            col *= fmid/(lu+0.001);
            col *= (1.0+(col/(fwhite*fwhite)));
            col -= 5.0;
            col = max(col, 0.0);
            col /= (10.0+col);
            gl_FragColor = vec4(col.rgb*colors, 1);
        }
    );
    storage.push_back(fMat9);//9

    /* material 10 */
    string fMat10 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            void main( void ) {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 p = r.xy / mm + .5;
                float pi = 3.141592653589793;
                float theta = pi / 0.1;
                \n#define time time + atan(p.x, p.y)*2.0\n
                mat2 m = mat2(cos(theta), -sin(theta), sin(theta), cos(theta));
                p = p * 2.0 - 1.0;
                p.x *= r.x / r.y;
                p = m * p;
                vec2 f = fract(p * 5.0);
                f = 2.0 * f - 1.0;
                float df = distance(f, vec2(0.0, 0.0));
                df = 3.0*df*df - 2.0*df*df*df;
                float dp = max(1.5 - distance(p, vec2(0.0, 0.0)), 0.0);
                dp = 3.0*dp*dp - 2.0*dp*df*df;
                float from = 0.3 + sin(dp * pi * 0.5 + time * 0.5) * 0.75;
                float to = from + 2.00;
                float d = smoothstep(from, to, df);
                float col = 3.0;
                col = d;
                gl_FragColor = vec4( vec3( col*colors ), 1.0 );
            }
    );
    storage.push_back(fMat10);//10

    /* material 11 */
    string fMat11 = STRINGIFY(
        \n#version 120\n
        \n#define iterations 10\n
        \n#define formuparam 0.530\n

        \n#define volsteps 12\n
        \n#define stepsize 0.2\n
        \n#define zoom   0.800\n
        \n#define tile   0.850\n
        \n#define speed  0.00001\n
        \n#define brightness 0.0015\n
        \n#define darkmatter 0.400\n
        \n#define distfading 0.760\n
        \n#define saturation 0.800\n

        uniform sampler2D texture;
        uniform sampler2D texture1;
        uniform float time;
        uniform vec3 colors;
        \n#define time tan(time)\n
        varying vec3 e;
        varying vec3 n;

        void main(void) {
            vec3 r = reflect( e, n );
            r = e - 2. * dot( n, e ) * n;
            float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
            vec2 uv = r.xy / mm + .5;
            vec3 dir=vec3(uv*zoom,tan(.1)*time);
            float a2=speed+.5;
            float a1=0.0;
            mat2 rot1=mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
            mat2 rot2=rot1;
            dir.xz*=rot1;
            dir.xy*=rot2;
            vec3 from=vec3(0.,0.,0.);
            from.xz*=rot1;
            from.xy*=rot2;
            float s=.4;
            float fade=.2;
            vec3 v=vec3(0.8);
            for (int r=0; r<volsteps; r++) {
                vec3 p=from+s*dir*.5;
                p = abs(vec3(tile)-mod(p,vec3(tile*2.)));
                float pa;
                float a=pa=0.;
                for (int i=0; i<iterations; i++) {
                    p=abs(p)/dot(p,p)-formuparam;
                    a+=abs(length(p)-pa);
                    pa=length(p);
                }
                float dm=max(0.,darkmatter-a*a*tan(.001));
                a*=a*a*2.;
                if (r>3) fade*=1.-dm;
                v+=fade;
                v+=vec3(s,s*s,s*s*s*s)*a*brightness*fade;
                fade*=distfading;
                s+=stepsize;
            }
            v=mix(vec3(length(v)),v,saturation);
            gl_FragColor = vec4(v*.01*colors,1.);
        }
    );
    storage.push_back(fMat11);//11

    /* material 12 */
    string fMat12 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            const int NUM_POINTS = 30;
            vec2 points[NUM_POINTS];
            vec2 seedx = vec2(0.4,0.5);
            vec2 seedy = vec2(0.1,0.8);
            float rand(vec2 co){
                return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
            }
            void main( void ) {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 position = r.xy / mm + .5;
                for (int f=0;f<NUM_POINTS;f++) {
                    //points[f]=vec2(float(f),float(f));
                    points[f]=vec2(rand(seedx),rand(seedy));
                    seedx = points[f];
                    seedy = seedy+points[f];
                }
                float minDist = 1000.;
                float dist;
                int closest;
                vec2 closestPoint;
                for (int f=0;f<NUM_POINTS;f++) {
                    dist = abs(length(position - points[f]));
                    if (dist < minDist) {
                        minDist = dist;
                        closest = f;
                        closestPoint=points[f];
                    }
                }
                gl_FragColor = vec4(vec3(1.-pow(10.*abs(length(position - closestPoint)),.3)),1.);
                gl_FragColor += vec4(vec3(float(closest)/float(NUM_POINTS),.5,fract(float(closest)*4./float(NUM_POINTS)))*colors,1.);
                //gl_FragColor = vec4( vec3(1.), 1.0 );
            }
    );
    storage.push_back(fMat12);//12

    /* material 13 */
    string fMat13 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            vec2 Hash2(vec2 p) {
                float r = 523.0*sin(dot(p, vec2(37.3158, 53.614)));
                return vec2(fract(17.12354 * r), fract(23.15865 * r));
            }
            vec2 Noise( in vec2 x ) {
                vec2 p = floor(x);
                vec2 f = fract(x);
                f = f*f*(3.0-2.0*f);
                return mix(Hash2(p), Hash2(p+1.0), f);
            }
            vec2 HashMove2(vec2 p) {
                return Noise(p+time*.1);
            }
            float Cells(in vec2 p, in float time) {
                vec2 f = fract(p);
                p = floor(p);
                float d = 1.0e10;
                for (int xo = -1; xo <= 1; xo++) {
                    for (int yo = -1; yo <= 1; yo++) {
                        vec2 g = vec2(xo, yo);
                        vec2 tp = g + .5 + sin(time * 2.0 + 6.2831 * HashMove2(p + g)) - f;
                        d = min(d, dot(tp, tp));
                    }
                }
                return sqrt(d);
            }
            void main(void) {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 uv = r.xy / mm + .5;
                float ti = time;
                vec4 col = vec4(colors.r,colors.g,colors.b,.0);
                float amp = 1.0;
                float size = 6.0 * (abs(fract(time*.01-.5)-.5)*50.0+1.0);
                for (int i = 0; i < 10; i++){
                    float c = 1.0-Cells(uv * size-size*.5, ti);
                    c = smoothstep(0.6+amp*.25, 1., c);
                    col += amp * vec4(.8, .23, .255,1.) * c;
                    amp *= .88;
                    ti -= .04;
                }
                gl_FragData[0] = col;
            }
    );
    storage.push_back(fMat13);//13

    /* material 14 */
    string fMat14 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            void main(void){
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 uv = r.xy / mm + .5;
                vec3 color = vec3(0.8 + 0.2*uv.y);
                for( int i=0; i<80; i++ ) {
                    float pha =      sin(float(i)*546.13+1.0)*0.5 + 0.5;
                    float siz = pow( sin(float(i)*651.74+5.0)*0.5 + 0.5, 4.0 );
                    float pox =      sin(float(i)*321.55+4.1) * r.x / r.y;
                    float rad = 0.1 + 0.5*siz;
                    vec2  pos = vec2( pox, -1.0-rad + (2.0+2.0*rad)*mod(pha+0.1*time*(0.2+0.8*siz),1.0));
                    float dis = length( uv - pos );
                    vec3  col = mix( vec3(0.94,0.3,0.0), vec3(0.1,0.4,0.8), 0.5+0.5*sin(float(i)*1.2+1.9));
                    //col+= 8.0*smoothstep( rad*0.95, rad, dis );
                    float f = length(uv-pos)/rad;
                    f = sqrt(clamp(1.0-f*f,0.0,1.0));
                    color -= col.zyx *(1.0-smoothstep( rad*0.95, rad, dis )) * f;
                }
                color *= sqrt(1.5-0.5*length(uv));
                gl_FragColor = vec4(color*colors,1.0);
            }
    );
    storage.push_back(fMat14);//14

    /* material 15 */
    string fMat15 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            vec3 hsv2rgb(vec3 c){
                  vec4 K = vec4(1.0, 1.0 / 3.0, 1.0 / 3.0, 3.0);
                  vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
                  return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
            }
            void main(void){
                  vec3 r = reflect( e, n );
                  r = e - 2. * dot( n, e ) * n;
                  float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                  vec2 p = r.xy / mm + .5;
                  vec2 pos = mod(p * 5.0, 2.0 )-1.0;
                  vec3 c = vec3(0.0);
                  vec3 circles = vec3(0.0);
                  vec3 color = vec3(0.0);
                  for(int i = 0; i < 300; i++){
                    c = hsv2rgb(vec3(length(p - time / 1000.0 * float(i)), 0.7, 0.9));
                    float dist_squared = dot(pos * pos, pos * pos);
                    float t = 1.0 - length(dist_squared);
                    t = pow(t, 4.0);
                    circles = (dist_squared < 0.99) ? vec3(t): vec3(0.0, 0.0, 0.0);
                    color = min(c, circles);
                  }
                  gl_FragColor = vec4(color*colors, 1.0);
            }
    );
    storage.push_back(fMat15);//15

    /* material 16 */
    string fMat16 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            vec2 rotate(vec2 p, float a){
                return vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));
            }
            float rand(float n){
                return fract(sin(n) * 43758.5453123);
            }
            vec2 rand2(in vec2 p){
                return fract(vec2(sin(p.x * 274.32 + p.y * 454.077 + time), cos(p.x * 191.32 + p.y * 490.077 + time)));
            }
            float noise1(float p){
                float fl = floor(p);
                float fc = fract(p);
                return mix(rand(fl), rand(fl + 1.0), fc);
            }
            float voronoi(in vec2 x){
                vec2 p = floor(x);
                vec2 f = fract(x);
                vec2 res = vec2(9.0);
                for(int j = -1; j <= 1; j ++) {
                    for(int i = -1; i <= 1; i ++){
                        vec2 b = vec2(i, j);
                        vec2 r = vec2(b) - f + rand2(p + b);
                        float d = max(abs(r.x), abs(r.y));
                        if(d < res.x) {
                            res.y = res.x;
                            res.x = d;
                        }
                        else if(d < res.y) {
                            res.y = d;
                        }
                    }
                }
                return res.y - res.x;
            }
            float flicker = noise1(time * 1.0) * 0.8 + 0.;
            void main(void) {
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 uv = r.xy / m + .5;
                vec2 suv = uv;
                float v = 0.0;
                //v = 1.0 - length(uv) * 1.3;
                //uv *= 0.6 + sin(time * 0.1) * 0.4;
                uv = rotate(uv, sin(0.0 * 0.3) * 1.0);
                //uv += time * 0.4;
                float a = 0.6;
                float f = 1.0;
                for(int i = 0; i < 3; i ++){
                    float v1 = voronoi(uv * f + 5.0);
                    float v2 = 0.0;
                    if(i > 0){
                        v2 = voronoi(uv * f * 0.5 + 50.0 + time);
                        float va = 0.0;
                        float vb = 0.0;
                        va = 1.0 - smoothstep(0.0, 0.11, v1);
                        vb = 1.0 - smoothstep(0.0, 0.98, v2);
                        v += a * pow(va * (0.5 + vb), 2.0);
                    }
                    v1 = 1.0 - smoothstep(0.0, 0.3, v1);
                    v2 = a * (noise1(v1 * 5.5 + 0.1));
                    if(i == 0)
                        v += v2 * flicker;
                    else
                        v += v2;
                    f *= 3.0;
                    a *= 0.7;
                }
                v *= exp(-0.6 * length(suv)) * 1.2;
                //vec3 cexp = texture2D(texture, uv * 0.001).xyz * 3.0 + texture2D(texture, uv * 0.01).xyz;//vec3(1.0, 2.0, 4.0);
                vec3 cexp = vec3(2.0, 1.0, 1.0);
                cexp *= 1.3;
                vec3 col = vec3(pow(v, cexp.x), pow(v, cexp.y), pow(v, cexp.z)) * 2.0;
                gl_FragColor = vec4(col*colors, 1.0);
            }
    );
    storage.push_back(fMat16);//16

    /* material 17 */
    string fMat17 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            vec2 rotate(vec2 p, float a) { return vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));  }
            float rand(float n) {  return fract(sin(n) * 43758.5453123); }
            vec2 rand2(in vec2 p) { return fract(vec2(sin(p.x * 591.32 + p.y * 154.077 + time), cos(p.x * 391.32 + p.y * 49.077 + time))); }
            float noise1(float p) {
                float fl = floor(p);
                float fc = fract(p);
                return mix(rand(fl), rand(fl + 1.0), fc);
            }
            float voronoi(in vec2 x){
                vec2 p = floor(x);
                vec2 f = fract(x);
                vec2 res = vec2(8.0);
                for(int j = -1; j <= 1; j ++){
                    for(int i = -1; i <= 1; i ++){
                        vec2 b = vec2(i, j);
                        vec2 r = vec2(b) - f + rand2(p + b);
                        float d = sqrt(abs(r.x*r.x) + abs(r.y*r.y));
                        if(d < res.x) {
                            res.y = res.x;
                            res.x = d;
                        } else if(d < res.y) {
                            res.y = d;
                        }
                    }
                }
                return res.y - res.x;
            }
            float flicker = noise1(time * 2.0) * 0.8 + 0.4;
            void main(void){
                vec3 r = reflect( e, n );
                r = e - 2. * dot( n, e ) * n;
                float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                vec2 uv = r.xy / m + .5;
                vec2 suv = uv;
                float v = 0.0;
                v = 1.0 - length(uv) * 1.3;
                float a = 0.6;
                float f = 3.0;
                for(int i = 0; i < 4; i ++) {
                    float v1 = voronoi(uv * f + 5.0);
                    float v2 = 0.0;
                    if(i > 0){
                        v2 = voronoi(uv * f * 0.5 + 50.0 + time);
                        float va = 0.0;
                        float vb = 0.0;
                        va = 1.0 - smoothstep(0.0, 0.1, v1);
                        vb = 1.0 - smoothstep(0.0, 0.08, v2);
                        v += a * pow(va * (0.5 + vb), 2.0);
                    }
                    v1 = 1.0 - smoothstep(0.0, 0.3, v1);
                    v2 = a * (noise1(v1 * 0.5 + 0.1));
                    if(i == 0)
                        v += v2 * flicker;
                    else
                        v += v2;
                    f *= 3.0;
                    a *= 0.7;
                }
                v *= exp(-0.6 * length(suv)) * 1.2;
                vec3 cexp = vec3(1.0, 2.0, 4.0);
                cexp *= 1.3;
                vec3 col = vec3(pow(v, cexp.x), pow(v, cexp.y), pow(v, cexp.z)) * 2.0;
                gl_FragColor = vec4(col*colors, 1.0);
            }
    );
    storage.push_back(fMat17);//17

    /* material 18 */
    string fMat18 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            float hash2e(vec2 uv) { return fract( sin(time*0.00001+15.23 * uv.x + 165.36 * uv.y) * 43578.24);}
            void main( void ) {
                vec3 rr = reflect( e, n );
                rr = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( rr.x, 2. ) + pow( rr.y, 2. ) + pow( rr.z + 1., 2. ) );
                vec2 uv = rr.xy / mm + .5;
                float c = hash2e(floor(uv * 10.0));
                gl_FragColor = vec4( c*colors.r, c*colors.g, c*colors.b, 1.0 );
            }
    );
    storage.push_back(fMat18);//18

    /* material 19 */
    string fMat19 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            \n#define PI 3.1415927\n
            \n#define PI2 (PI * 2.0)\n
            void main( void ) {
                  vec3 rr = reflect( e, n );
                  rr = e - 2. * dot( n, e ) * n;
                  float mm = 2. * sqrt( pow( rr.x, 2. ) + pow( rr.y, 2. ) + pow( rr.z + 1., 2. ) );
                  vec2 position = rr.xy / mm + .5;
                  vec2 p = position * sin(time) * 50.0;
                  float r = length(p);
                  float a = atan(p.y, p.x) + PI;
                  float d = r - a + PI2;
                  float n = PI2 * float(int(d / PI2));
                  float da = a + n;
                  float pos = 0.07 * sin(time / 10.) * da * da + time;
                  float rand = sin(floor(pos));
                  gl_FragColor = vec4(fract(rand *vec3(10.0, 1000.0, 100.0))*colors, 1.0);
            }
    );
    storage.push_back(fMat19);//19

    /* material 20 */
    string fMat20 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            void main(void) {
                  vec3 rr = reflect( e, n );
                  rr = e - 2. * dot( n, e ) * n;
                  float mm = 2. * sqrt( pow( rr.x, 2. ) + pow( rr.y, 2. ) + pow( rr.z + 1., 2. ) );
                  vec2 position = rr.xy / mm + .5;
                  vec2 p = (position - 0.5) * 35.;
                  float angle = time;
                  vec2 direction = vec2(cos(angle), sin(angle));
                  float brightness = cos(dot(p * sin(angle * 0.25), direction) + sin(distance(direction, tan(p))));
                  gl_FragColor.rgb = vec3(.2, brightness, cos(brightness))*colors;
                  gl_FragColor.a = 0.95;
            }
    );
    storage.push_back(fMat20);//20

    /* material 21 */
    string fMat21 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            void main(void) {
                vec3 rr = reflect( e, n );
                rr = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( rr.x, 2. ) + pow( rr.y, 2. ) + pow( rr.z + 1., 2. ) );
                vec2 pos = rr.xy / mm + .5;
                float dist = length(pos) / 1.0;
                vec4 color = vec4(0.0);
                float ang = atan(pos.y, pos.x) / 3.14159265;
                if(mod(ang + dist + time / 8.0, 0.2) < 0.1 || dist < 0.05)
                        color = vec4(colors,1.0);
                else if(mod(ang - dist, 0.2) < 0.1)
                        color = vec4(0.0, 0.0, 0.0,0.2);
                gl_FragColor = vec4(color);
            }
    );
    storage.push_back(fMat21);//21

    /* material 22 */
    string fMat22 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            vec2 rotate(vec2 p, float a) { return vec2(p.x * cos(a) - p.y * sin(a), p.x * sin(a) + p.y * cos(a));  }
            float rand(float n) { return fract(sin(n) * 43758.5453123); }
            vec2 rand2(in vec2 p) { return fract(vec2(sin(p.x * 591.32 + p.y * 154.077), cos(p.x * 391.32 + p.y * 49.077))); }
            float noise1(float p) {
                float fl = floor(p);
                float fc = fract(p);
                return mix(rand(fl), rand(fl + 1.0), fc);
            }
            float voronoi(in vec2 x) {
                vec2 p = floor(x);
                vec2 f = fract(x);
                vec2 res = vec2(8.0);
                for(int j = -1; j <= 1; j ++) {
                    for(int i = -1; i <= 1; i ++){
                        vec2 b = vec2(i, j);
                        vec2 r = vec2(b) - f + rand2(p + b);
                        float d = max(abs(r.x), abs(r.y));
                        if(d < res.x) {
                            res.y = res.x;
                            res.x = d;
                        } else if(d < res.y) {
                            res.y = d;
                        }
                    }
                }
                return res.y - res.x;
            }
            float flicker = noise1(time * 2.0) * 0.8 + 0.4;
            void main(void) {
                vec3 rr = reflect( e, n );
                rr = e - 2. * dot( n, e ) * n;
                float mm = 2. * sqrt( pow( rr.x, 2. ) + pow( rr.y, 2. ) + pow( rr.z + 1., 2. ) );
                vec2 uv = rr.xy / mm + .5;
                vec2 suv = uv;
                uv.x *= uv.x / uv.y;
                float v = 0.0;
                uv = rotate(uv, sin(0.0 * 0.3) * 1.0);
                float a = 0.6;
                float f = 1.0;
                for(int i = 0; i < 3; i ++) {
                    float v1 = voronoi(uv * f + 5.0);
                    float v2 = 0.0;
                    if(i > 0) {
                        v2 = voronoi(uv * f * 0.5 + 50.0 + time);
                        float va = 0.0;
                        float vb = 0.0;
                        va = 1.0 - smoothstep(0.0, 0.1, v1);
                        vb = 1.0 - smoothstep(0.0, 0.08, v2);
                        v += a * pow(va * (0.5 + vb), 2.0);
                    }
                    v1 = 1.0 - smoothstep(0.0, 0.3, v1);
                    v2 = a * (noise1(v1 * 5.5 + 0.1));
                    if(i == 0)
                        v += v2 * flicker;
                    else
                        v += v2;

                    f *= 3.0;
                    a *= 0.7;
                }
                v *= exp(-0.6 * length(suv)) * 1.2;
                vec3 cexp = vec3(3.0, 1.0, 3.0);
                cexp *= 1.3;
                vec3 col = vec3(pow(v, cexp.x), pow(v, cexp.y), pow(v, cexp.z)) * 2.0;
                gl_FragColor = vec4(col*colors, 1.0);
            }
    );
    storage.push_back(fMat22);//22

    /* material 23 */
    string fMat23 = STRINGIFY(
            \n#version 120\n
            uniform sampler2D texture;
            uniform sampler2D texture1;
            uniform float time;
            uniform vec3 colors;
            varying vec3 e;
            varying vec3 n;
            vec4 hexagon( vec2 p ){
                    vec2 q = vec2( p.x*2.0*0.5773503, p.y + p.x*0.5773503 );
                    vec2 pi = floor(q);
                    vec2 pf = fract(q);
                    float v = mod(pi.x + pi.y, 3.0);
                    float ca = step(1.0,v);
                    float cb = step(2.0,v);
                    vec2  ma = step(pf.xy,pf.yx);
                    float e = dot( ma, 1.0-pf.yx + ca*(pf.x+pf.y-1.0) + cb*(pf.yx-2.0*pf.xy) );
                    p = vec2( q.x + floor(0.5+p.y/1.5), 4.0*p.y/3.0 )*0.5 + 0.5;
                    float f = length( (fract(p) - 0.5)*vec2(1.0,0.85) );
                    return vec4( pi + ca - cb*ma, e, f );
            }
            float hash1( vec2  p ) { float n = dot(p,vec2(127.1,311.7) ); return fract(sin(n)*43758.5453); }
            float noise( in vec3 x ){
                vec3 p = floor(x);
                vec3 f = fract(x);
                f = f*f*(3.0-2.0*f);
                vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
                vec2 rg = texture2D( texture, (uv+0.5)/256.0, -100.0 ).yx;
                return mix( rg.x, rg.y, f.z );
            }
            void main( void ) {
                    vec3 r = reflect( e, n );
                    r = e - 2. * dot( n, e ) * n;
                    float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
                    vec2 pos = 4. * r.xy / m + .5;
                    pos *= 1.0 + 0.3*length(pos);
                    //
                    vec4 h = hexagon(8.0*pos + 0.5*time);
                    float n = noise( vec3(0.3*h.xy+time*0.1,time) );
                    vec3 col = 0.15 + 0.15*hash1(h.xy+1.2)*vec3(1.0);
                    col *= smoothstep( 0.10, 0.11, h.z );
                    //col *= smoothstep( 0.10, 0.11, h.w );
                    col *= 1.0 + 0.15*sin(40.0*h.z);
                    col *= 0.75 + 0.5*h.z*n;
                    //
                    h = hexagon(6.0*pos + 0.4);
                    n = noise( vec3(0.3*h.xy*0.1,h.y) );
                    vec3 colb = 1.0 + 0.8*sin( hash1(h.xy)*1.5 + 2.0 + vec3(1.0,.0,1.0) );
                    colb *= smoothstep( 0.10, 0.11,h.z);
                    colb *= 1.0 + 0.15*sin(40.0*h.z);
                    colb *= 0.75 + 0.5*h.z*n;
                    h = hexagon(6.0*(pos+0.1*vec2(-1.3,1.0)) + 0.6);
                    col *= 1.0-0.8*smoothstep(0.45,0.451,noise( vec3(0.3*h.xy+0.1,time) ));
                    col = mix( col, colb, smoothstep(0.45,0.451,n) );
                    col *= pow( 16.0*pos.x*(1.0-pos.x)*pos.y*(1.0-pos.y), 0.1 );
                    gl_FragColor = vec4( col*colors, 1.0 );
            }
    );
    storage.push_back(fMat23);//23

    return storage;
}

vector<string> glsl::normalMap(){
    vector<string> storage;
    string frag = STRINGIFY(
        varying vec3 normal;
        void main(){
            vec3 N = normalize(normal);
            gl_FragColor = vec4((N + vec3(1., 1., 1.)) / 2.0,1.0);
        }
    );
    string vert = STRINGIFY(
        varying vec3 normal;
            void main(){
                normal = gl_NormalMatrix * gl_Normal;
                gl_Position = ftransform();
            }
    );
    storage.push_back(vert);
    storage.push_back(frag);
    return storage;
}
/* end */
