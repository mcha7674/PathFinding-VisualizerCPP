#include "glpch.h"
#include "Texture.h"
#include <stb_image.h>

Texture::Texture(const std::string& filePath)
	: m_FilePath(filePath), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	// Flip our texture vertically (texture is always flipped so this corrects that).
	stbi_set_flip_vertically_on_load(1);
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4);
	// generate texture id 
	GLCall(glGenTextures(1, &m_RendererID));
	// BIND TEXTURE TO ID WITH TEXTURE TYPE
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

/* PARAMETER FILTER TYPES
GL_NEAREST
Returns the value of the texture element that is nearest (in Manhattan distance) to the center 
of the pixel being textured. 

GL_LINEAR
    Returns the weighted average of the four texture elements that are closest to the center 
	of the pixel being textured. 

GL_NEAREST_MIPMAP_NEAREST
    Chooses the mipmap that most closely matches the size of the pixel being textured and uses 
	the GL_NEAREST criterion (the texture element nearest to the center of the pixel) to 
	produce a texture value. 

GL_LINEAR_MIPMAP_NEAREST
    Chooses the mipmap that most closely matches the size of the pixel being textured 
	and uses the GL_LINEAR criterion (a weighted average of the four texture elements
	that are closest to the center of the pixel) to produce a texture value. 

GL_NEAREST_MIPMAP_LINEAR
    Chooses the two mipmaps that most closely match the size of the pixel being textured 
	and uses the GL_NEAREST criterion (the texture element nearest to the center of the pixel) 
	to produce a texture value from each mipmap. The final texture value is a weighted average 
	of those two values. 

GL_LINEAR_MIPMAP_LINEAR

    Chooses the two mipmaps that most closely match the size of the pixel being textured 
	and uses the GL_LINEAR criterion (a weighted average of the four texture elements that 
	are closest to the center of the pixel) to produce a texture value from each mipmap. 
	The final texture value is a weighted average of those two values. 
*/

/* WRAPPING PARAMETERS
	
    GL_REPEAT: The default behavior for textures. Repeats the texture image.

    GL_MIRRORED_REPEAT: Same as GL_REPEAT but mirrors the image with each repeat.

    GL_CLAMP_TO_EDGE: Clamps the coordinates between 0 and 1. 
	The result is that higher coordinates become clamped to the edge, 
	resulting in a stretched edge pattern.

    GL_CLAMP_TO_BORDER: Coordinates outside the range are now given a user-specified border color.

*/
	// The following 4 parameter functions MUST be specified or we get a black screen

	// FILTERING parameters - For both magnified and minified
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)); // for minified texture, use mipmap!
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	// WRAP Parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)); // S axis = x-axis
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)); // T axis = y-axis
	
	if (m_LocalBuffer) // if loaded data is not a null pointer...
	{	
		// arg 2 - set to 0 because it is not a multilevel texture
		// arg 3 - How we want openGL to deal with our data, RGBA is what we will use
		// arg 6 - border size in pixels
		// arg 7 - second format type: format of the data we are supplying (still RGBA)
		// arg 8 - type of data: set to unsigned byte since our m_locaBuufer is unsigned char type (a char = 1byte)
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
	}
	// We are done Generating, loading and configuring our texture- we can now unBind it
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	// Free The data from memory
	stbi_image_free(m_LocalBuffer);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID)); // delete texture from the GPU
}

void Texture::Bind(unsigned int slot) const
{
	/* 
	Switch between which texture is active by passing in
	the slot unsigned int ENUM. EX:
	slot = GL_TEXTURE0 // texture 1 active
	slot = GL_TEXTURE1 // texture 2 active

	We have a limited number of texture slots. That number depends
	on what platform you are in.
	*/
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::UnBind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
