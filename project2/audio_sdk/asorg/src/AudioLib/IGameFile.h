#ifndef IGAMEFILE_H__
#define IGAMEFILE_H__

namespace Audio
{

// Wrapper class for ease of handling files.  
class IGameFile
{
public:
	IGameFile()				{}
	virtual ~IGameFile()	{}

	bool Open(std::string sFileName);
	void Close();

	virtual bool operator !() = 0;
	virtual uint32 GetSize() = 0;
	virtual uint8* GetData() = 0;

	virtual bool Read() = 0;
	virtual bool Read(uint8* pBuffer, uint32 nSize) = 0;

	virtual bool GetFileInfo(std::string& sFileName, uint32& nOffset) = 0;
};

}; // namespace Audio

#endif // IGAMEFILE_H__
