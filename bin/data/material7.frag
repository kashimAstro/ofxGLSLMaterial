#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

float lu = 0.06;
float fmid = 0.2;
float fwhite = 0.5;

void main(void)
{
	vec3 r = reflect( e, n );
        r = e - 2. * dot( n, e ) * n;
        float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
        vec2 uv = r.xy / m + .5;
	
	//gl_FragColor = vec4(uv,0.5+0.5*sin(time),1.0);
	vec4 col = texture2D(texture, uv);
	col *= fmid/(lu+0.001);
	col *= (1.0+(col/(fwhite*fwhite)));
	col -= 5.0;
	col = max(col, 0.0);
	col /= (10.0+col);
	gl_FragColor = vec4(col.rgb, 1);
}
