#include "pch.h"
#include "GlobalRingBuffer.h"
CGlobalRingBuffer::CGlobalRingBuffer(void):m_Buffer()
{
	m_Buffer.Create(0xd00);
}
CGlobalRingBuffer::~CGlobalRingBuffer(void)
{
	m_Buffer.Destroy();
}
void* CGlobalRingBuffer::Alloc(size_t tSize)
{
	return m_Buffer.Alloc(tSize);
}