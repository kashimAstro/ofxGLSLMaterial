#version 120

uniform sampler2D texture;
uniform sampler2D texture1;
uniform float time;

varying vec3 e;
varying vec3 n;

#define iterations 14
#define formuparam 0.530
#define time tan(time)
#define volsteps 18
#define stepsize 0.2

#define zoom   0.800
#define tile   0.850
#define speed  0.00001

#define brightness 0.0015
#define darkmatter 0.400
#define distfading 0.760
#define saturation 0.800

void main(void)
{
	vec3 r = reflect( e, n );
	r = e - 2. * dot( n, e ) * n;
        float mm = 2. * sqrt( pow( r.x, 2. ) + pow( r.y, 2. ) + pow( r.z + 1., 2. ) );
        vec2 uv = r.xy / mm + .5;

	vec3 dir=vec3(uv*zoom,tan(.1)*time);
	
	float a2=speed+.5;
	float a1=0.0;
	mat2 rot1=mat2(cos(a1),sin(a1),-sin(a1),cos(a1));
	mat2 rot2=rot1;//mat2(cos(a2),sin(a2),-sin(a2),cos(a2));
	dir.xz*=rot1;
	dir.xy*=rot2;
	
	vec3 from=vec3(0.,0.,0.);
	from+=vec3((tan(.05),.15,-2.));
	
	from.xz*=rot1;
	from.xy*=rot2;
	
	float s=.4,fade=.2;
	vec3 v=vec3(0.8);
	for (int r=0; r<volsteps; r++) {
		vec3 p=from+s*dir*.5;
		p = abs(vec3(tile)-mod(p,vec3(tile*2.)));
		float pa,a=pa=0.;
		for (int i=0; i<iterations; i++) { 
			p=abs(p)/dot(p,p)-formuparam; 
			a+=abs(length(p)-pa);
			pa=length(p);
		}
		float dm=max(0.,darkmatter-a*a*tan(.001));
		a*=a*a*2.;
		if (r>3) fade*=1.-dm;
		//v+=vec3(dm,dm*.5,0.);
		v+=fade;
		v+=vec3(s,s*s,s*s*s*s)*a*brightness*fade;
		fade*=distfading; 
		s+=stepsize;
	}
	v=mix(vec3(length(v)),v,saturation); //color adjust
	gl_FragColor = vec4(v*.01,1.);	
	
}
