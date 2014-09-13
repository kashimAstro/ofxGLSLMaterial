#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

float hash2e(vec2 uv) {
	return fract( sin(time*0.00001+15.23 * uv.x + 165.36 * uv.y) * 43578.24);
}
void main( void ) {

	//vec2 uv = ( gl_FragCoord.xy / resolution.xy );
	//uv.x *= resolution.x / resolution.y;

	vec3 rr = reflect( e, n );
	rr = e - 2. * dot( n, e ) * n;
        float mm = 2. * sqrt( pow( rr.x, 2. ) + pow( rr.y, 2. ) + pow( rr.z + 1., 2. ) );
        vec2 uv = rr.xy / mm + .5;
	float c = hash2e(floor(uv * 10.0));
	gl_FragColor = vec4( c, c, c, 1.0 );
}
