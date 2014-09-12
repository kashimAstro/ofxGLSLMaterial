#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

void main( void ) {
	vec3 r = reflect( e, n );
	r = e - 2. * dot( n, e ) * n;
        float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
        vec2 p = r.xy / mm + .5;

	float pi = 3.141592653589793;
	float theta = pi / 0.1;
	#define time time + atan(p.x, p.y)*2.0
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

	gl_FragColor = vec4( vec3( col ), 1.0 );

}
