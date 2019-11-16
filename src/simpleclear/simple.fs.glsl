#version 450 core

//// Declare VS_OUT as an input interface block
//// Same interface block name is all that matters :)
//in VS_OUT
//{
//    vec4 color;     // Send color to the next stage
//} fs_in;

out vec4 color; // output variables are sent to window/screen

float rand(vec2 co);

void main(void)
{
	//color = vs_color;
	//color = vec4(0.0, 0.0, 0.0, 1.0);

	//vec2 seed = gl_FragCoord.xy;
	//vec2 seed2 = fract(seed + vec2(0.25, 0.25));
	//vec2 seed3 = fract(seed + vec2(0.35, 0.35));
	
	//color = vec4(rand(seed), rand(seed2), rand(seed3), 1.0);

	// The beauty of this algorithm, is that super-nearby pixels are similar in color!	
	color = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,
                cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,
                sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y * 0.15),
                1.0);
}

float rand(vec2 co) {
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

