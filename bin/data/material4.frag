#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;


void main( void )
{
	vec3 r = reflect( e, n );
        r = e - 2. * dot( n, e ) * n;
        float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
        vec2 uv = r.xy / m + .5;
	
	vec2 e = 1.0/uv;
	float am1 = 0.5 + 0.5*sin( time );
	float am2 = 0.5 + 0.5*cos( time );
	
	for( int i=0; i<50; i++ )
	{
		float h  = dot( texture2D(texture, uv,               -100.0).xyz, vec3(0.333) );
		float h1 = dot( texture2D(texture, uv+vec2(e.x,0.0), -100.0).xyz, vec3(0.333) );
		float h2 = dot( texture2D(texture, uv+vec2(0.0,e.y), -100.0).xyz, vec3(0.333) );
		vec2 g = 0.001*vec2( (h1-h), (h2-h) )/e;
		vec2 f = g.yx*vec2(-1.0,1.0);
		g = mix( g, f, am1 );
		uv -= 0.01*g*am2;
	}
	
	vec3 col = texture2D(texture, uv).xyz;
        col *= 2.0;
	gl_FragColor = vec4(col, 1.0);
}

