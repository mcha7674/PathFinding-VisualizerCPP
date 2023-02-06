#pragma once


class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	// create vertex buffer constructor
	VertexBuffer(const void* data, unsigned int size, bool isDynamicDraw = false);
	~VertexBuffer();

	void UpdateBuffer(const void* data, unsigned int size, bool isDynamicDraw = false);

	void Bind() const;
	void UnBind() const;

};