#version 450 core

//// Declare VS_OUT as an input interface block
//// Same interface block name is all that matters :)
//in VS_OUT
//{
//    vec4 color;     // Send color to the next stage
//} fs_in;

out vec4 color; // output variables are sent to window/screen

void main(void)
{
	//color = vs_color;
	color = vec4(0.0, 0.0, 0.0, 1.0);
}
