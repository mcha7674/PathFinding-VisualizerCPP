#pragma once
#include "glpch.h"
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp> // maths libraries

namespace GLCore::Utils {

	class Shader
	{
	public:
		~Shader();

		GLuint GetRendererID() { return m_RendererID; }

		static Shader* FromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);

		void use() const;

		void unuse() const; // deactivate shader

		//ShaderUniforms(const GLuint &ID);
		// Utility uniform functions (to be used in Abstraction Child Class)
		// add const in front of args to make it a compiler error for this
		// - class function to change a member variable of the class. Only reading allowed
		void SetUniform1i(const std::string& name, bool value);
		void SetUniform1i(const std::string& name, int value);
		void SetUniform1f(const std::string& name, float value);
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v30); // 4f uniform
		// Matrix Uniform
		void SetUniformMatrix4fv(const std::string& name, const glm::mat4 matrix); //mat4 uniform
		// vector Uniform
		void SetUniformVec3fv(const std::string& name, const glm::vec3 vector);
		void SetUniformVec4fv(const std::string& name, const glm::vec4 vector);

	private:
		Shader() = default;
		void LoadFromGLSLTextFiles(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
		GLuint CompileShader(GLenum type, const std::string& source);
		int GetUniformLocation(const std::string& name);

		GLuint m_RendererID;
		std::unordered_map <std::string, int> m_UniformLocationCache;
	};

}