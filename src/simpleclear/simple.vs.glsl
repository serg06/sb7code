#version 450 core

// shader starts executing here
void main(void)
{
	// hard-coded triangle for now
	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
                                     vec4(-0.25, -0.25, 0.5, 1.0),
                                     vec4(0.25, 0.25, 0.5, 1.0));

	// index point using gl_VertexID, the VAO index (?) passed to glDrawArrays, incrementing by 1 for count (glDrawArrays param) iterations.
	gl_Position = vertices[gl_VertexID];
}
