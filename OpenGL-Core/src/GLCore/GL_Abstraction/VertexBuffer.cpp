#include "glpch.h"
#include "VertexBuffer.h"
#include "Renderer.h"

						// void pointer is a form of auto data type for a var
VertexBuffer::VertexBuffer(const void* data, unsigned int size, bool isDynamicDraw)
{
	// Generate, Bind, and implement into buffer Memory for a vertex buffer
	GLCall(glGenBuffers(1, &m_RendererID)); // generate id for our vertex buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	if (!isDynamicDraw){
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); // copies the prev defined vertex data into buffer's mem
	} else {
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
	}
}

void VertexBuffer::UpdateBuffer(const void* data, unsigned int size, bool isDynamicDraw)
{
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
	// TO unbind simply pass in 0 instead of ID
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
