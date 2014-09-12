#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

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
    
	for (int xo = -1; xo <= 1; xo++)
	{
		for (int yo = -1; yo <= 1; yo++)
		{
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
    vec3 col = vec3(0.0);
	
    float amp = 1.0;
    float size = 4.0 * (abs(fract(time*.01-.5)-.5)*50.0+1.0);
    
    for (int i = 0; i < 32; i++)
    {
        float c = 1.0-Cells(uv * size-size*.5, ti);
        c = smoothstep(0.6+amp*.25, 1., c);
        col += amp * vec3(.8, .23, .225) * c;
        amp *= .88;
        ti -= .04;
    }
	gl_FragData[0] = vec4(min(col, 1.0), 1.0);
}

