#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

void main(void) {
	vec3 rr = reflect( e, n );
	rr = e - 2. * dot( n, e ) * n;
	float mm = 2. * sqrt( pow( rr.x, 2. ) + pow( rr.y, 2. ) + pow( rr.z + 1., 2. ) );
	vec2 pos = rr.xy / mm + .5;

	float dist = length(pos) / 1.0;

	vec3 color = vec3(0.0);
	float ang = atan(pos.y, pos.x) / 3.14159265;
	if(mod(ang + dist + time / 8.0, 0.2) < 0.1 || dist < 0.05) color = vec3(0.0, 0.5, 1.0);
	else if(mod(ang - dist, 0.2) < 0.1) color = vec3(0.0, 0.4, 0.8);
	gl_FragColor = vec4(vec3(color), 1.0 );

}

