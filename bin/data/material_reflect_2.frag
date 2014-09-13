uniform sampler2D frontMap;
uniform sampler2D backMap;

varying vec3 R;

void main (void)
{
	vec4 output_color;
	
	vec3 vR = normalize(R);
	
	// Select the front or back env map according to the sign of vR.z.
	if(vR.z>0.0)
	{
		// calculate the forward paraboloid map texture coordinates	
		vec2 frontUV;
		frontUV = (vR.xy / (2.0*(1.0 + vR.z))) + 0.5;
		output_color = texture2D( frontMap, frontUV );
	}
	else
	{
		// calculate the backward paraboloid map texture coordinates
		vec2 backUV;
		backUV = (vR.xy / (2.0*(1.0 - vR.z))) + 0.5;
		output_color = texture2D( backMap, backUV );
	}
	
	gl_FragColor = output_color;
}
