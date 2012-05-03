/////////////////////////////////////////////////////////////////////
/// @file   FileEx.h
///         wrapper class for low-level file operation which is based
///         on file handle.
///
/// @since  August 7, 2000
/// @author seojt@kogsoft.com
///
/// @date   August 22, 2002
///         - Doxygen JAVADOC comments added for public release^^
///

#if !defined(_CFileEx_Defined_)
#define _CFileEx_Defined_

#if _MSC_VER > 1000
#pragma once
#endif // #if _MSC_VER > 1000

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>


/////////////////////////////////////////////////////////////////////
/// @class  CFileEx
///         wrapper class for handle based file operation.
///
/// @see    CFileExBuffering
///
class CFileEx
{
public:
    /// @enum   OpenFlags
    /// flags which is needed for Open() function.
    /// @see    CFileEx::Open()
	enum OpenFlags
    {
		modeRead       = 0x0000, ///< open for reading only
		modeWrite      = 0x0001, ///< open for writing only
		modeReadWrite  = 0x0002, ///< open for reading and writing
        modeAppend     = 0x0008, ///< write done at EOF

        modeCreate     = 0x0100, ///< create and open file
        modeTrunc      = 0x0200, ///< open and truncate
        modeExcl       = 0x0400, ///< open only if file doesn't already exist
		//shareCompat    = 0x0000, // OBSOLETE!
		//shareExclusive = 0x0010,
		//shareDenyWrite = 0x0020,
		//shareDenyRead  = 0x0030,
		//shareDenyNone  = 0x0040,
        modeRandom     = 0x0010, ///< file access is primarily random
        modeSequential = 0x0020, ///< file access is primarily sequential
        modeTemporary  = 0x0040, ///< temporary file bit
		modeNoInherit  = 0x0080, ///< child process doesn't inherit file
		typeText       = 0x4000, 
		typeBinary     = (int)0x8000 
	};// enum OpenFlags

    /* enum Attribute {     // OBSOLETE!
		normal    = 0x00,
		readOnly  = 0x01,
		hidden    = 0x02,
		system    = 0x04,
		volume    = 0x08,
		directory = 0x10,
		archive   = 0x20
	}; // enum Attribute
    /**/

    /// @enum   SeekPosition
    /// @see    CFileEx::Seek()
	enum SeekPosition
    {
        begin   = 0, ///< SEEK_BEGIN
        current = 1, ///< SEEK_CUR
        end     = 2  ///< SEEK_END
    };// enum SeekPosition

    /// invalid handle value.
    /// @note   handle 0 is used by system
	enum { hFileNull = -1 };

    static int READ;        ///< combined general read flag.
    static int WRITE;       ///< combined general write flag.
    static int READWRITE;   ///< combined general read/write flag.

public:
    int             m_hFile;            ///< handle to this file
    
    /// @note  relative path filename will automatically converted to fullpath.
    char            m_szaFullPath[256]; ///< full path filename
      
    char            m_szaDrive[3];      ///< parsed drive name(include ":" like "C:")
    char            m_szaPath[256];     ///< parsed absolute path
    char            m_szaName[256];     ///< parsed filename
    char            m_szaExt[20];       ///< parsed extension

    int             m_nOpenFlags;       ///< opening flags
    int             m_nShareFlags;      ///< sharing flags
    struct _stat    m_stat;             ///< file status

private:
    /// init handle and some members.
    void _InitMembers();

public:
    /// default constructor.
    CFileEx();

    /// constructor.
    /// open file with mode
    /// @param  lpszFileName: [in] input filename
    /// @param  nOpenFlags: file I/O flags
    CFileEx(const char* lpszFileName, int nOpenFlags = CFileEx::READ);

    /// destructor.
    /// this class can be used base class so destructor is 'virtual'
    virtual ~CFileEx();

    ///////////////////////////////////////////////////////////////////////
    // I/O functions: open, close, read, and write

    /// open file.
    /// @param  lpszFileName: [in] input filename
    /// @param  nOpenFlag: file I/O flags
    /// @return opened file handle
    ///         -1 when file isn't opened.
    int Open(const char* lpszFileName, int nOpenFlags = CFileEx::READ);

    /// close file.
    virtual void Close();

    /// read data.
    /// @param  lpBuf: [out] buffer which contains read data
    /// @param  nCount: number of byte(s) to read
    /// @return number of byte(s) read
    inline int Read(void* lpBuf, int nCount = 1)
    {
        return _read(m_hFile, lpBuf, nCount);
    }//Read()

    /// write data.
    /// @param  lpBuf: [in] buffer which contains write data
    /// @param  nCount: number of byte(s) to write
    /// @return number of byte(s) written
    inline int Write(const void* lpBuf, int nCount = 1)
    {
        return _write(m_hFile, lpBuf, nCount);
    }//Write()

    ///////////////////////////////////////////////////////////////////////
    // seek functions

    /// move FP(file pointer).
    /// @param  lOff: relative offset
    /// @param  nFrom: seek flag
    /// @see    CFileEx::SeekPosition
    /// @return new FP(relative offset)
    inline long Seek(long lOffset, int nFrom = CFileEx::current)
    {
        return _lseek(m_hFile, lOffset, nFrom);
    }//Seek()

    /// move FP to begin.
    inline long SeekToBegin()
    {
        return Seek(0L, CFileEx::begin);
    }//SeekToBegin()

    /// move FP to end.
    inline long SeekToEnd()
    {
        return Seek(0L, CFileEx::end);
    }//SeekToEnd()

    /// get current FP.
    /// @return current FP
    inline long GetFilePointer()
    {
        return _tell(m_hFile);
    }//GetFilePointer()

    /// get current FP.
    inline long GetPosition()
    {
        return _tell(m_hFile);
    }//GetPosition()

    /// get file size in bytes.
    /// @return file size in bytes
    inline long GetLength() const
    {
        return _filelength(m_hFile);
    }//GetLength()

    /// set new file size.
    /// @param  lNewLen: new size in bytes.
    /// @return 1 when size changed
    int SetLength( long lNewLen )
    {
        return _chsize(m_hFile, lNewLen);
    }//SetLength()

    /// get file handle.
    inline int GetHandle() const { return m_hFile; }

    /// check EOF.
    /// @return 1 when EOF reached
    virtual int IsEOF()
    {
        return _eof(m_hFile);
    }//IsEOF()

    /// flush data.
    inline void Flush() { _commit(m_hFile); }

    /// get file status.
    /// status is set in 'm_stat'
    /// @return 1 when status set succeeded
    /// @note   access CFileEx::m_stat to get status info.
    inline int GetStatus()
    {
        return _fstat(m_hFile, &m_stat) == 0;
    }//GetStatus()

    /// return 1 when file handle isn't NULL.
    inline int IsOpened()
    {
        return m_hFile != -1;
    }//IsOpened()

    ///////////////////////////////////////////////////////////////////////
    // filename parsing functions

    /// get fullpath filename like 'c:\mydir\test.cpp'.
    inline char* GetFullName() const
    {
        return (char*)m_szaFullPath;
    }//GetFullName()
    
    /// get fullpath filename.
    inline operator char*() const
    {
        return (char*)m_szaFullPath;
    }//operator char*()
    
    /// get filename like 'c:'.
    inline char* GetFileDrive() const
    {
        return (char*)m_szaDrive;
    }//GetFileDrive()
    
    /// get file path like '\mydir\'.
    inline char* GetFilePath() const
    {
        return (char*)m_szaPath;
    }//GetFilePath()
    
    /// get file title like 'test'.
    inline char* GetFileName() const
    {
        return (char*)m_szaName;
    }//GetFileName()
    
    /// get file extension like '.cpp'.
    inline char* GetFileExt() const
    {
        return (char*)m_szaExt;
    }//GetFileExt()

};//class CFileEx

#endif // !defined(_CFileEx_Defined_)
