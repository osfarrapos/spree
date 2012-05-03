

#include "MemBlock.h"

using namespace Audio;

MemBlock::MemBlock()
{
	Clear();
}


MemBlock::~MemBlock()
{
}


void MemBlock::Clear()
{
	m_pData = 0;
	m_nSize = 0;
}



MemBlockManager::MemBlockManager()
{
	Clear();
}


MemBlockManager::~MemBlockManager()
{
	Term();
}


void MemBlockManager::Clear()
{
	m_Free.clear();
	m_Used.clear();

	m_nTotalMemory = 0;
	m_nBufferMax = 0;
}



bool MemBlockManager::Init(uint32 nBufferMax)
{
	m_nBufferMax = nBufferMax;

	return true;
}


bool MemBlockManager::Term()
{
	return true;
}



bool MemBlockManager::GetMemBlock(uint32 nSize, MemBlock*& pMemBlock)
{
	return true;
}


void MemBlockManager::ReleaseMemBlock(MemBlock* pMemBlock)
{
}
