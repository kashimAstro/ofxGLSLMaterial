#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

#define PI 3.1415927
#define PI2 (PI * 2.0)

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

  gl_FragColor = vec4(fract(rand *vec3(10.0, 1000.0, 100.0)), 1.0);
}

