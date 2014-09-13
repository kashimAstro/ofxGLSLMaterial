uniform mat4 ModelWorld4x4;
uniform vec3 CameraPos;

varying vec3 R;

mat3 GetLinearPart( mat4 m )
{
	mat3 result;
	
	result[0][0] = m[0][0]; 
	result[0][1] = m[0][1]; 
	result[0][2] = m[0][2]; 

	result[1][0] = m[1][0]; 
	result[1][1] = m[1][1]; 
	result[1][2] = m[1][2]; 
	
	result[2][0] = m[2][0]; 
	result[2][1] = m[2][1]; 
	result[2][2] = m[2][2]; 
	
	return result;
}		
			   
void main()
{	
	gl_Position = ftransform();		
	
	mat3 ModelWorld3x3 = GetLinearPart( ModelWorld4x4 );
	
	// find world space position.
	vec4 WorldPos = ModelWorld4x4 *  gl_Vertex;	
	
	// find world space normal.
	vec3 N = normalize( ModelWorld3x3 * gl_Normal ); 
	
	// find world space eye vector.
	vec3 E = normalize( WorldPos.xyz - CameraPos.xyz );	
	
	// calculate the reflection vector in world space.
	R = reflect( E, N ); 
}
