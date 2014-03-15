//-----------------------------------------------------------------------------
// File:			W_VertexBuffer.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_VertexBuffer.h"

namespace wolf
{
//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
VertexBuffer::VertexBuffer(unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
	glGenBuffers(1, &m_uiBuffer);
}

//----------------------------------------------------------
// Constructor
//----------------------------------------------------------
VertexBuffer::VertexBuffer(const void* p_pData, unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
	m_usage = usage_static;
	glGenBuffers(1, &m_uiBuffer);
	Write(p_pData);
}

VertexBuffer::VertexBuffer(BufferUsage usage,const void* p_pData, unsigned int p_uiLength) : m_uiLength(p_uiLength)
{
	m_usage = usage;
	glGenBuffers(1, &m_uiBuffer);
	Write(p_pData);
}

//----------------------------------------------------------
// Destructor
//----------------------------------------------------------
VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1,&m_uiBuffer);
}

//----------------------------------------------------------
// Fills this vertex buffer with the given data
//----------------------------------------------------------
void VertexBuffer::Write(const void* p_pData, int p_iLength)
{
	Bind();
	glBufferData(GL_ARRAY_BUFFER, p_iLength == -1 ? m_uiLength : p_iLength, p_pData, m_usage);
}

//----------------------------------------------------------
// Binds this buffer
//----------------------------------------------------------
void VertexBuffer::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiBuffer);
}

void* VertexBuffer::Lock()
{
	return glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
}

void* VertexBuffer::Unlock()
{
	glUnmapBuffer(GL_ARRAY_BUFFER);
	return nullptr;
}
}
