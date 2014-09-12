#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

void main(void)
{
    vec3 r = reflect( e, n );
    r = e - 2. * dot( n, e ) * n;
    float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
    vec2 uv = r.xy / m + .5;

	float d = length(uv);
	vec2 st = uv*0.1 + 0.2*vec2(cos(0.071*time+d),
								sin(0.073*time-d));

    vec3 col = texture2D( texture, st ).xyz;
    float w = col.x;
	col *= 1.0 - texture2D( texture, 0.4*uv + 0.1*col.xy  ).xyy;
	col *= w*2.0;
	
	col *= 1.0 + 2.0*d;
	gl_FragColor = vec4(col,1.0);
}


