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
  vec2 position = rr.xy / mm + .5;

  vec2 p = (position - 0.5) * 35.;
  float angle = time;
  vec2 direction = vec2(cos(angle), sin(angle));

  float brightness = cos(dot(p * sin(angle * 0.25), direction) + sin(distance(direction, tan(p))));
  gl_FragColor.rgb = vec3(.2, brightness, cos(brightness));
  gl_FragColor.a = 0.95;
}
