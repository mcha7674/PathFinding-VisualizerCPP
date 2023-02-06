#pragma once
#include "Renderer.h"


struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		// size of different types
		switch (type)
		{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			default: return 0;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:

	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;
	
public:

	VertexBufferLayout()
		: m_Stride(0) {}


	template<typename T> // general template specification
	void Push(unsigned int count)
	{	// assert here since we should only be using the specialized template types below
		// Performs Compile-time assertion checking
		//static_assert(false); // COMMMENT OUT, ALWAYS ASSERTS FOR SOME REASON?? 
	}

	template<> // template specialization - this is how u speciailize a datatype for a general Template
	void Push<float>(unsigned int count)
	{
		VertexBufferElement vbe =  { GL_FLOAT, count, GL_FALSE };
		m_Elements.push_back(vbe);
		// adding a float, must add to stride
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count; // 4 bytes * number points in vertex (The count)
	}

	template<> 
	void Push<unsigned int>(unsigned int count)
	{
		VertexBufferElement vbe = { GL_UNSIGNED_INT, count, GL_FALSE };
		m_Elements.push_back(vbe);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<> 
	void Push<unsigned char>(unsigned int count)
	{
		VertexBufferElement vbe = { GL_UNSIGNED_BYTE, count, GL_TRUE };
		m_Elements.push_back(vbe);
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	// inline kwd to inline the following functions!! why? - Time is saved in execution of function
	// inline only small functions
	// inline reduces some of the overhead cost of calling a function by having no function control transfer, code is just copied in line
	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }

};