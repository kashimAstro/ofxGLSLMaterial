#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

float noise(float s)
{
	return fract(cos(s*11345.123)*123113.123);
}

float Texture(vec2 p)
{
	vec3 c=vec3(0);
	float m=noise(p.x);
	float q=floor(p.x*10.);
	p.y+=(floor(noise(q)*1.7)*1.5-1.)*time*0.2;
	p=mod(p+floor(m*10.)/10.,0.1)-0.05;
	float l=pow(1.-length(p),111.93)*1.21;
	
	return l;
}

void main(void)
{
    vec3 r = reflect( e, n );
    r = e - 2. * dot( n, e ) * n;
    float m = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
    vec2 p = r.xy / m + .5;

	p=tan(p*cos(time*0.2)*13.4);
	vec3 c=vec3(0);
	float d=.01/length(p*p); 
	d=cos(time*0.5+d)-tan(d); 

	p.x+=0.12*cos(d*50.1+time);
	p.y+=0.13*sin(d*50.1+time);
	float a=(atan(p.x,p.y)*0.23)+time*0.05;
	c.x=Texture(vec2(a+time*0.,d))*(15.*length(p)*length(p));
	c.y=Texture(vec2(a*2.1+time*-0.,d*3.3))*(5.*length(p)*length(p));
	c.z=Texture(vec2(a*3.1+time*0.,d*5.3))*(20.*length(p)*length(p));
	c+=vec3(c.z+c.y,c.z+c.x,c.y+c.x)*0.2;
	gl_FragColor = vec4( c, 1.0 );
	
	
}

