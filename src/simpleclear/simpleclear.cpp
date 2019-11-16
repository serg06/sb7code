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
#include <list> 
#include <vector> 
#include <iterator> 
#include <tuple> 


#define M_PI 3.14159265358979323846

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
		rendering_program = compile_shaders();

		// useless for now, just need to have at least one VAO (Vertex Array Object) so glsl allows us to call draw-vertex functions
		glCreateVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);

		// big points
		glPointSize(15.0f);

		// set polygon mode for everything to lines -- i.e. just draw outlines
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void shutdown() {
		// delete it all!
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
		glDeleteVertexArrays(1, &vertex_array_object); // why twice?
	}
	
	// execute shaders and actually draw on screen!
	virtual void render(double currentTime) {
		// fill buffer with red
		const GLfloat color[] = { (float)sin(currentTime) * 0.5f + 0.5f, (float)cos(currentTime) * 0.5f + 0.5f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, color);

		// use our program object for rendering
		glUseProgram(rendering_program);

		// set input vertex attributes for vertex shader
		const GLfloat offset[] = { (float)sin(currentTime) * 0.5f, (float)cos(currentTime) * -0.6f, 0.0f, 0.0f };
		glVertexAttrib4fv(0, offset); // 0 = offset
		
		const GLfloat shape_color[] = { (float)sin(currentTime+M_PI) * 0.5f + 0.5f, (float)cos(currentTime+M_PI) * 0.5f + 0.5f, 0.0f, 1.0f };
		//vec4(0.0, 0.8, 1.0, 1.0);
		glVertexAttrib4fv(1, shape_color); // 1 = vs_color

		// draw triangle
		glDrawArrays(GL_PATCHES, 0, 3); // draw triangle using 3 VAOs, starting at the 0th one (our only one!)
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;

	GLuint compile_shaders(void) {
		GLuint program;
		std::list <std::tuple<std::string, GLenum>> shader_fnames;
		std::list <GLuint> shaders; // store compiled shaders

		// list of shader names to include in program
		shader_fnames.push_back(std::make_tuple("../src/simpleclear/simple.vs.glsl", GL_VERTEX_SHADER));
		shader_fnames.push_back(std::make_tuple("../src/simpleclear/simple.tcs.glsl", GL_TESS_CONTROL_SHADER));
		shader_fnames.push_back(std::make_tuple("../src/simpleclear/simple.tes.glsl", GL_TESS_EVALUATION_SHADER));
		shader_fnames.push_back(std::make_tuple("../src/simpleclear/simple.fs.glsl", GL_FRAGMENT_SHADER));
		shader_fnames.push_back(std::make_tuple("../src/simpleclear/simple.gs.glsl", GL_GEOMETRY_SHADER));

		// for each input shader
		for (const std::tuple <std::string, GLenum> &shader_fname : shader_fnames) 
		{
			// extract shader info
			const std::string fname = std::get<0>(shader_fname);
			const GLenum shadertype = std::get<1>(shader_fname);

			// load shader src
			std::ifstream shader_file(fname);

			if (!shader_file.is_open()) {
				OutputDebugString("could not open shader file: ");
				OutputDebugString(fname.c_str());
				OutputDebugString("\n");
				exit(1);
			}

			const std::string shader_src((std::istreambuf_iterator<char>(shader_file)), std::istreambuf_iterator<char>());
			const GLchar * shader_src_ptr = shader_src.c_str();

			// Create and compile shader
			const GLuint shader = glCreateShader(shadertype); // create empty shader
			glShaderSource(shader, 1, &shader_src_ptr, NULL); // set shader source code
			glCompileShader(shader); // compile shader

			// CHECK IF COMPILATION SUCCESSFUL
			GLint status = GL_TRUE;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
			if (status == GL_FALSE)
			{
				GLint logLen;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
				std::vector <char> log(logLen);
				GLsizei written;
				glGetShaderInfoLog(shader, logLen, &written, log.data());

				OutputDebugString("compilation error with shader ");
				OutputDebugString(fname.c_str());
				OutputDebugString(":\n\n");
				OutputDebugString(log.data());
				OutputDebugString("\n");
				exit(1);
			}

			// Close file, save shader for later
			shader_file.close();
			shaders.push_back(shader);
		}

		// Create program, attach shaders to it, and link it
		program = glCreateProgram(); // create (empty?) program

		// attach shaders
		for (const GLuint &shader : shaders) {
			glAttachShader(program, shader);
		}

		glLinkProgram(program); // link together all attached shaders

		// CHECK IF LINKING SUCCESSFUL
		GLint status = GL_TRUE;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint logLen;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
			std::vector <char> log(logLen);
			GLsizei written;
			glGetProgramInfoLog(program, logLen, &written, log.data());

			OutputDebugString("linking error with program:\n\n");
			OutputDebugString(log.data());
			OutputDebugString("\n");
			exit(1);
		}

		// Delete the shaders as the program has them now
		for (const GLuint &shader : shaders) {
			glDeleteShader(shader);
		}

		return program;
	}

	std::list <std::ifstream> wew(void) {
		// open all our shader files and return them in a list
	}
};

DECLARE_MAIN(simpleclear_app)
