#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

/*void main(){
	vec3 r = reflect( e, n );
//	r = e - 2. * dot( n, e ) * n;
	float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
	vec2 uv = r.xy / m + .5;

	uv.y += (cos((uv.y + (time * 0.04)) * 295.0) * 0.0019) +
		(cos((uv.y + (time * 0.1)) * 10.0) * 0.002);

    
	uv.x += (sin((uv.y + (time * 0.07)) * 105.0) * 0.0029) +
		(sin((uv.y + (time * 0.1)) * 15.0) * 0.002);

	vec4 col = texture2D(texture,uv);
	gl_FragColor = col;
}*/
#define TAU 6.28318530718
#define MAX_ITER 5

void main( void ) {
	float time = time * .5;
	vec3 r = reflect( e, n );
	//r = e - 2. * dot( n, e ) * n;
	float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
	vec2 sp = r.xy / m + .5;

	vec2 p = sp*TAU - 20.0;
	vec2 i = p;
	float c = 1.0;
	float inten = .05;

	for (int n = 0; n < MAX_ITER; n++) 
	{
		float t = time * (1.0 - (3.5 / float(n+1)));
		i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
		c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
	}
	c /= float(MAX_ITER);
	c = 1.55-sqrt(c);
	vec3 colour = vec3(pow(abs(c), 6.0));
	gl_FragColor = vec4(clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0), 1.0);
}
