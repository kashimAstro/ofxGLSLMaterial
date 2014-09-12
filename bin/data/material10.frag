#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

const int NUM_POINTS = 50;

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
	gl_FragColor += vec4(float(closest)/float(NUM_POINTS),.5,fract(float(closest)*4./float(NUM_POINTS)),1.);
	//gl_FragColor = vec4( vec3(1.), 1.0 );

}

