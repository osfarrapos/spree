#ifndef MEMBLOCK_H__
#define MEMBLOCK_H__

#include "Audio.h"

namespace Audio
{

class MemBlock;

typedef std::vector<MemBlock*> MemBlockVector;

class MemBlock
{
public:
	unsigned char* GetData()		{  return m_pData;  }
	unsigned int GetSize()  const	{  return m_nSize;  }

	friend class MemBlockManager;

private:
	MemBlock();
	~MemBlock();
	void Clear();

	uint8*	m_pData;
	uint32	m_nSize;

};


class MemBlockManager
{
public:
	MemBlockManager();
	~MemBlockManager();
	void Clear();

	bool Init(uint32 nBufferMax = 0xFFFFFFFF);
	bool Term();

	bool GetMemBlock(uint32 nSize, MemBlock*& pMemBlock);
	void ReleaseMemBlock(MemBlock* pMemBlock);

private:

	MemBlockVector	m_Free;
	MemBlockVector	m_Used;

	uint32			m_nTotalMemory;
	uint32			m_nBufferMax;
};


}; // namespace Audio


#endif // MEMBLOCK_H__

