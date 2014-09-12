#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

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

  gl_FragColor = vec4(color, 1.0);
}
