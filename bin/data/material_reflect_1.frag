uniform sampler2D envMap;
varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	vec4 final_color = 
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient);

	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		// Diffuse color.
		final_color += gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * lambertTerm;
		
		// Specular color.
		final_color += texture2D( envMap, gl_TexCoord[0].st) * lambertTerm;
	}
		
	gl_FragColor = final_color;
	gl_FragColor.a   = 0.9;
}
