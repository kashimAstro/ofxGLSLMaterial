varying vec3 normal, lightDir, eyeVec;

void main()
{	
	gl_Position = ftransform();		
	
	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeVec = -vVertex;
	
	vec3 u = normalize( vVertex );
	normal = normalize( gl_NormalMatrix * gl_Normal );
	vec3 r = reflect( u, normal );
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	gl_TexCoord[0].s = r.x/m + 0.5;
	gl_TexCoord[0].t = r.y/m + 0.5;
}
