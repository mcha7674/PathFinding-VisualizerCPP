#pragma once
#define ib_GLtype GL_UNSIGNED_INT

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count; // How many indices in index buffer

public:
	// create vertex buffer constructor
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void UnBind() const;
	// getter function for count
	// inline specifier kwd avoids overhead created by the function call
	inline unsigned int GetCount() const { return m_Count; }
};