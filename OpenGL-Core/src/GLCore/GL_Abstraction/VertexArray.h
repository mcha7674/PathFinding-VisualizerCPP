#pragma once
#include "VertexBuffer.h"

/*
We can forward declare vertexBufferLayout class here since the AddBuffer() function below
only takes a reference. This allows us to remove VertexBufferLayout.h header file which means that
the header file's Renderer.h include wont be included in this file either. It wont give us cyclical
include probs.

The cyclical prob occurred because the VertexBufferLayout header includes renderer header file.
Since we use This header file (VertexArray) inside Renderer.h, the Renderer.h was being included in the 
Renderer.h File. THis leads to duplicate declarations in the header file! It also creates a sort of
infinite loop of headers referencing each other. So we remove the layout header file from the this header and
include it in the VertexArray.cpp file instead (since only the header files are being included in the other file
and thus the source of the cyclicalness).

We must still include VertexBufferLayout header inside of the VertexArray.cpp file.Or else
cpp file does not know where the layout methods are.
*/
class VertexBufferLayout; // forward declaration of VertexBufferLayout class.

class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	VertexArray();
	~VertexArray();

	void AddBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
	

	void Bind() const;
	void UnBind() const;

};