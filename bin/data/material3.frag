uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

#define POINTS 100
float t = time;

float dist2(vec2 P0, vec2 P1) { vec2 D=P1-P0; return dot(D,D); }
float hash (float i) { return 2.*fract(sin(i*7467.25)*1e5) - 1.; }
vec2  hash2(float i) { return vec2(hash(i),hash(i-.1)); }
vec4  hash4(float i) { return vec4(hash(i),hash(i-.1),hash(i-.3),hash(i+.1)); }
	
vec2 P(float i) {
	vec4 c = hash4(i);
	return vec2(   cos(t*c.x-c.z)+.5*cos(2.765*t*c.y+c.w),
				 ( sin(t*c.y-c.w)+.5*sin(1.893*t*c.x+c.z) )/1.5	 );
}

void main(void)
{
        vec3 r = reflect( e, n );
	r = e - 2. * dot( n, e ) * n;
        float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
        vec2 uv = r.xy / mm + .5;

	float m =  .1*t/6.283;
	float my = .5*pow(.5*(1.-cos(.1*t)),3.);
	int MODE = 0;
	float fMODE = (1.-cos(6.283*m))/2.;

	const int R = 1;
	
	float v=0.; vec2 V=vec2(0.);
	for (int i=1; i<POINTS; i++) { 
		vec2 p = P(float(i));
		for (int y=-R; y<=R; y++) 
			for (int x=-R; x<=R; x++) {
				vec2 d = p+2.*vec2(float(x),float(y)) -uv; 
				float r2 = dot(d,d);
				r2 = clamp(r2,5e-2*my,1e3);
				V +=  d / r2; 
			}
		}
	
	v = length(V);
	v *= 1./(9.*float(POINTS));
//	v = clamp(v,0.,.1);
	
	v *= 2.+100.*fMODE;
	if (MODE==0) gl_FragColor = vec4(.2*v)*vec4(1,2,2,2);// +smoothstep(.05,.0,abs(v-5.*my))
	if (MODE==1) gl_FragColor = vec4(.5+.5*sin(2.*v));
	if (MODE==2) gl_FragColor = vec4(sin(v),sin(v/2.),sin(v/4.),1.);
}
