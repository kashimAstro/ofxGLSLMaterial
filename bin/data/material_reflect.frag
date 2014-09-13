uniform sampler2D colorMap;
uniform sampler2D envMap;

void main (void)
{
//	vec4 color = texture2D( colorMap, gl_TexCoord[0].st);
	vec4 env = texture2D( envMap, gl_TexCoord[1].st);

	gl_FragColor = /*color +*/ env*0.6;
}
