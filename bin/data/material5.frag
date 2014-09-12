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
        vec2 p = r.xy / m + .5;

	vec2 lightPos = vec2(
		(1.2 + sin(time)) * 0.4 * p.x,
		(1.2 + cos(time)) * 0.4 * p.y
	);
	float bumpStrength = 16.0;
	float bumpRadius1 = 0.1; 
	float bumpRadius2 = 0.25; 
	float lightStrength = 1.6;
	float lightRadius = 0.7;
	float refDist = 1.2 * p.x / 500.0;
		
	
	vec2 vecToLight = gl_FragCoord.xy - lightPos;
	float distToLight = length(vecToLight.xy / p.xy);
	vec2 dirToLight = normalize(vecToLight);
	vec2 curPos = gl_FragCoord.xy / p.xy;
	vec2 refPos = curPos.xy - (refDist * dirToLight.xy / p.xy);
	
	vec4 curSample = texture2D(texture, curPos);
	vec4 refSample = texture2D(texture, refPos);
	
	float curLumin = (curSample.x + curSample.y + curSample.z) * 0.33;
	float refLumin = (refSample.x + refSample.y + refSample.z) * 0.33;
	
	float directionBrightness =
		lightStrength - 
		(
			step(bumpRadius1, distToLight) * 
			clamp((distToLight - bumpRadius1) / (bumpRadius2 - bumpRadius1), 0.0, 1.0) *
			step(curLumin, refLumin) * 
			clamp((refLumin - curLumin) * bumpStrength, 0.0, 1.0)
		);
	
	float distanceBrightness = 1.0 - (distToLight / lightRadius);
	
	gl_FragColor.xyz = directionBrightness * distanceBrightness * curSample.xyz;
	gl_FragColor.w = curSample.w;
}
