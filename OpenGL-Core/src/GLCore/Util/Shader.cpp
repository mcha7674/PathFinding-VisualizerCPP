#include "glpch.h"
#include "Shader.h"

#include <fstream>

namespace GLCore::Utils {

	static std::string ReadFileAsString(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize((size_t)in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			LOG_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	GLuint Shader::CompileShader(GLenum type, const std::string& source)
	{
		GLuint shader = glCreateShader(type);

		const GLchar* sourceCStr = source.c_str();
		glShaderSource(shader, 1, &sourceCStr, 0);

		glCompileShader(shader);

		GLint isCompiled = 0;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(shader);

			LOG_ERROR("{0}", infoLog.data());
		}

		return shader;
	}

	Shader* Shader::FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		Shader* shader = new Shader();
		shader->LoadFromGLSLTextFiles(vertexShaderPath, fragmentShaderPath);
		return shader;
	}
	
	void Shader::LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
	{
		std::string vertexSource = ReadFileAsString(vertexShaderPath);
		std::string fragmentSource = ReadFileAsString(fragmentShaderPath);

		GLuint program = glCreateProgram();
		int glShaderIDIndex = 0;
			
		GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
		glAttachShader(program, vertexShader);
		GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);
		glAttachShader(program, fragmentShader);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			LOG_ERROR("{0}", infoLog.data());
		}
		
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		m_RendererID = program;
	}

	// use / activate the ShaderUniforms!!
	void Shader::use() const { glUseProgram(m_RendererID); } // BIND
	// deactivate
	void Shader::unuse() const { glUseProgram(0); } // UNBIND

	int Shader::GetUniformLocation(const std::string& name)
	{
		// if location exists in cache, do not assign location again, simply return cached location and name
		// From Docs: Return: if the specified key value is found, or unordered_map::end if the specified key
		if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		{
			return m_UniformLocationCache[name]; // return cached location
		}

		// If Location is Not cached, get location from OpenGL:
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		if (location == -1)
		{
			std::cout << "Warning: Uniform '" << name << "' doesn't exist!" << std::endl;
		}
		// Cache the newley acquired location:
		m_UniformLocationCache[name] = location;

		return location;
	}


	// Utility uniform functions //
	void Shader::SetUniform1i(const std::string& name, bool value)
	{
		glUniform1i(GetUniformLocation(name), (int)value);
	}

	void Shader::SetUniform1i(const std::string& name, int value)
	{
		glUniform1i(GetUniformLocation(name), value);
	}

	void Shader::SetUniform1f(const std::string& name, float value)
	{
		glUniform1f(GetUniformLocation(name), value);
	}

	void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
	{
		glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
	}

	void Shader::SetUniformMatrix4fv(const std::string& name, const glm::mat4 matrix)
	{
		// 2nd arg (the 1) = the count. How many matrices we are providing
		// 3rd arg = whether to transpose matrix from column major to row major. GLM uses so keep column major
		// the reason we stor the matrix the way we do is because we just store a reference to the first element, The rest is store sequentially in mem
		glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
		// we can replace the last arg with glm::value_ptr(matrix)
	}

	void Shader::SetUniformVec3fv(const std::string& name, const glm::vec3 vector)
	{
		glUniform3fv(GetUniformLocation(name), 1, &vector[0]);
	}
	void Shader::SetUniformVec4fv(const std::string& name, const glm::vec4 vector)
	{
		glUniform4fv(GetUniformLocation(name), 1, &vector[0]);
	}
}

