#include "glpch.h"
#include "IndexBuffer.h"
#include "Renderer.h"


IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count) // init count
{
	// Generate, Bind, and implement into buffer Memory for a vertex buffer
	GLCall(glGenBuffers(1, &m_RendererID)); // generate id for our vertex buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW)); // copies the prev defined vertex data into buffer's mem
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
	// TO unbind simply pass in 0 instead of ID
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
