#version 450 core

layout (location = 0) in vec4 position;

layout (std140, binding = 0) uniform UNI_IN
{
    mat4 mv_matrix; // takes up 16 bytes
    mat4 proj_matrix; // takes up 16 bytes
} uni;

// shader starts executing here
void main(void)
{
	gl_Position = uni.proj_matrix * uni.mv_matrix * position;
}
