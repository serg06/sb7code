#version 450 core

// set qualifiers:
//   triangles:     tesselation mode = triangles
//   equal_spacing: vertices are equally spaced
//   cw:            clockwise triangles
layout (triangles, equal_spacing, cw) in;

void main(void)
{
    gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position +
                   gl_TessCoord.y * gl_in[1].gl_Position +
                   gl_TessCoord.z * gl_in[2].gl_Position);
}
