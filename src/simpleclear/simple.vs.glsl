#version 450 core

// add an 'offset' input vertex attribute (idx 0)
layout (location = 0) in vec4 offset;

// add color input
layout (location = 1) in vec4 color;

// Screw declaring all fragment shader output variables separately - let's do 'em together I guess!
// Declare VS_OUT as an output interface block
// Same interface block name is all that matters :)
out VS_OUT
{
    vec4 color;     // Send color to the next stage
} vs_out;

// shader starts executing here
void main(void)
{
	// hard-coded triangle for now
	const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),
                                     vec4(-0.25, -0.25, 0.5, 1.0),
                                     vec4(0.25, 0.25, 0.5, 1.0));

	// index point using gl_VertexID, the VAO index (?) passed to glDrawArrays, incrementing by 1 for count (glDrawArrays param) iterations.
	gl_Position = vertices[gl_VertexID] + offset;

	vs_out.color = color;
}
