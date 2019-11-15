/*
 * Copyright © 2012-2015 Graham Sellers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*
 * FROM NOW ON, THIS IS MYYYYY PROJECT
 * Stopped on page: 24: Drawing Our First Triangle.
 */

#include <sb7.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>

class simpleclear_app : public sb7::application
{
public:
	void init()
	{
		static const char title[] = "OpenGL SuperBible - Simple Clear";

		sb7::application::init();

		memcpy(info.title, title, sizeof(title));
	}

	void read_directory(const std::string& name)
	{
		std::string pattern(name);
		pattern.append("\\*");
		WIN32_FIND_DATA data;
		HANDLE hFind;
		if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
			do {
				OutputDebugString(data.cFileName);
				OutputDebugString("");
			} while (FindNextFile(hFind, &data) != 0);
			FindClose(hFind);
		}
	}

	void startup() {
		//// print working directory
		//char pBuf[256];
		//size_t len = sizeof(pBuf);
		//int bytes = GetModuleFileName(NULL, pBuf, len);

		//// print files in current dir
		//read_directory(".");

		rendering_program = compile_shaders();

		// useless for now, just need to have at least one VAO (Vertex Array Object) so glsl allows us to call draw-vertex functions
		glCreateVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);

		// tell OpenGL to draw us HUGE POINTS :)
		glPointSize(40.0f);
	}

	void shutdown() {
		// delete it all!
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteVertexArrays(1, &vertex_array_object); // why twice?
	}

	virtual void render_simpleclear(double currentTime)
	{
		const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);
	}

	virtual void render(double currentTime) {
		// execute shaders and actually draw on screen!

		// fill buffer with red
		const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		//const GLfloat color[] = { 0.0f, 0.2f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		glUseProgram(rendering_program); // use our program object for rendering
		//glDrawArrays(GL_POINTS, 0, 1); // call our drawing command -- draw 1 point for every vertex array -- so just 1 point
		glDrawArrays(GL_TRIANGLES, 0, 3); // draw triangle using 3 VAOs, starting at the 0th one (our only one!)
	}


private:
	GLuint rendering_program;
	GLuint vertex_array_object;

	GLuint compile_shaders(void) {
		GLuint vertex_shader;
		GLuint fragment_shader;
		GLuint program;

		std::ifstream vshader_file("../build/simple.vs.glsl");
		std::ifstream fshader_file("../build/simple.fs.glsl");

		if (!vshader_file.is_open()) {
			OutputDebugString("could not open vshader file\n");
			exit(1);
		}

		if (!fshader_file.is_open()) {
			OutputDebugString("could not open fshader file\n");
			exit(1);
		}

		std::string vshader_src((std::istreambuf_iterator<char>(vshader_file)), std::istreambuf_iterator<char>());
		std::string fshader_src((std::istreambuf_iterator<char>(fshader_file)), std::istreambuf_iterator<char>());

		static const GLchar * vshader_src_ptr = vshader_src.c_str();
		static const GLchar * fshader_src_ptr = fshader_src.c_str();

		// Create and compile vertex shader
		vertex_shader = glCreateShader(GL_VERTEX_SHADER); // create empty shader
		glShaderSource(vertex_shader, 1, &vshader_src_ptr, NULL); // set shader source code
		glCompileShader(vertex_shader); // compile shader

		// Create and compile fragment shader
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, &fshader_src_ptr, NULL);
		glCompileShader(fragment_shader);

		// Create program, attach shaders to it, and link it
		program = glCreateProgram(); // create (empty?) program
		glAttachShader(program, vertex_shader);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program); // link together all attached shaders

		// Delete the shaders as the program has them now
		// ME: I think we just delete the reference; then when the program deletes its references, references drop to 0 and it gets ACTUALLY deleted.
		glDeleteShader(vertex_shader);
		glDeleteShader(fragment_shader);

		return program;
	}
};

DECLARE_MAIN(simpleclear_app)
