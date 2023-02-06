#pragma once
#include "glpch.h"
#include "Renderer.h" //gives us va, shader, ib, and openGL funcs through glad.


class Texture
{
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer; // Local storage for the texture, stores texture data (stbi_laod())
	int m_Width, m_Height, m_BPP; // BPP = Bits per pixel
public:
	Texture(const std::string &filePath);
	~Texture();

	// Slot arg is for binding multiple textures at once! - set to default of 0
	void Bind(unsigned int slot = 0) const;
	void UnBind() const;

	// Helper functions
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};