//=================================================
// Matt Pietrek
// MSDN Magazine, 2002
// FILE: WheatyExceptionReport 클래스를 수정하였음.
// Edited by bugman
//=================================================
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <dbghelp.h>
#include "NDExceptionReport.h"

#pragma comment(linker, "/defaultlib:dbghelp.lib")

TCHAR NDExceptionReport::m_szLogFileName[MAX_PATH];
HANDLE NDExceptionReport::m_hReportFile;
HANDLE NDExceptionReport::m_hProcess;


NDExceptionReport::NDExceptionReport()
{
	SYSTEMTIME curTime ;
	GetLocalTime( &curTime ) ;
	
	char filename[60] = {0,} ;
	char name[15]= "ND_Exception";

	sprintf( filename, "logfile\\ExceptionReport\\%s%02u%02u_%02u%02u.EXC", name, curTime.wMonth, curTime.wDay, curTime.wHour, curTime.wMinute );
	SetLogFileName(filename);

    m_hProcess = GetCurrentProcess();
	m_hReportFile = CreateFile( m_szLogFileName, GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, FILE_FLAG_WRITE_THROUGH, 0 );
}

NDExceptionReport::~NDExceptionReport( )
{
	if ( m_hReportFile )
    {
        SetFilePointer( m_hReportFile, 0, 0, FILE_END );
		CloseHandle( m_hReportFile );
        m_hReportFile = 0;
    }
}

void NDExceptionReport::GenerateExceptionReport( PEXCEPTION_POINTERS pExceptionInfo )    
{
    _tprintf(_T("//=====================================================\r\n"));

    PEXCEPTION_RECORD pExceptionRecord = pExceptionInfo->ExceptionRecord;
    _tprintf(   _T("Exception code: %08X %s\r\n"),
                pExceptionRecord->ExceptionCode,
                GetExceptionString(pExceptionRecord->ExceptionCode) );

    TCHAR szFaultingModule[MAX_PATH];
    DWORD section, offset;
    GetLogicalAddress(  pExceptionRecord->ExceptionAddress,
                        szFaultingModule,
                        sizeof( szFaultingModule ),
                        section, offset );

    _tprintf( _T("Fault address:  %08X %02X:%08X %s\r\n"),
                pExceptionRecord->ExceptionAddress,
                section, offset, szFaultingModule );

    PCONTEXT pCtx = pExceptionInfo->ContextRecord;

    #ifdef _M_IX86  // X86 Only!
    _tprintf( _T("\r\nRegisters:\r\n") );

    _tprintf(_T("EAX:%08X\r\nEBX:%08X\r\nECX:%08X\r\nEDX:%08X\r\nESI:%08X\r\nEDI:%08X\r\n")
            ,pCtx->Eax, pCtx->Ebx, pCtx->Ecx, pCtx->Edx,
            pCtx->Esi, pCtx->Edi );

    _tprintf( _T("CS:EIP:%04X:%08X\r\n"), pCtx->SegCs, pCtx->Eip );
    _tprintf( _T("SS:ESP:%04X:%08X  EBP:%08X\r\n"),
                pCtx->SegSs, pCtx->Esp, pCtx->Ebp );
    _tprintf( _T("DS:%04X  ES:%04X  FS:%04X  GS:%04X\r\n"),
                pCtx->SegDs, pCtx->SegEs, pCtx->SegFs, pCtx->SegGs );
    _tprintf( _T("Flags:%08X\r\n"), pCtx->EFlags );

    #endif

    SymSetOptions( SYMOPT_DEFERRED_LOADS );

    if ( !SymInitialize( GetCurrentProcess(), 0, TRUE ) )
        return;

    CONTEXT trashableContext = *pCtx;

    WriteStackDetails( &trashableContext, false );

    #ifdef _M_IX86  // X86 Only!

    _tprintf( _T("========================\r\n") );
    _tprintf( _T("Local Variables And Parameters\r\n") );

    trashableContext = *pCtx;
    WriteStackDetails( &trashableContext, true );

    _tprintf( _T("========================\r\n") );
    _tprintf( _T("Global Variables\r\n") );

    SymEnumSymbols( GetCurrentProcess(),
                    (DWORD64)GetModuleHandle(szFaultingModule),
                    0, EnumerateSymbolsCallback, 0 );
    
    #endif      // X86 Only!

    SymCleanup( GetCurrentProcess() );

    _tprintf( _T("\r\n") );
}

LPTSTR NDExceptionReport::GetExceptionString( DWORD dwCode )
{
    #define EXCEPTION( x ) case EXCEPTION_##x: return _T(#x);

    switch ( dwCode )
    {
        EXCEPTION( ACCESS_VIOLATION )
        EXCEPTION( DATATYPE_MISALIGNMENT )
        EXCEPTION( BREAKPOINT )
        EXCEPTION( SINGLE_STEP )
        EXCEPTION( ARRAY_BOUNDS_EXCEEDED )
        EXCEPTION( FLT_DENORMAL_OPERAND )
        EXCEPTION( FLT_DIVIDE_BY_ZERO )
        EXCEPTION( FLT_INEXACT_RESULT )
        EXCEPTION( FLT_INVALID_OPERATION )
        EXCEPTION( FLT_OVERFLOW )
        EXCEPTION( FLT_STACK_CHECK )
        EXCEPTION( FLT_UNDERFLOW )
        EXCEPTION( INT_DIVIDE_BY_ZERO )
        EXCEPTION( INT_OVERFLOW )
        EXCEPTION( PRIV_INSTRUCTION )
        EXCEPTION( IN_PAGE_ERROR )
        EXCEPTION( ILLEGAL_INSTRUCTION )
        EXCEPTION( NONCONTINUABLE_EXCEPTION )
        EXCEPTION( STACK_OVERFLOW )
        EXCEPTION( INVALID_DISPOSITION )
        EXCEPTION( GUARD_PAGE )
        EXCEPTION( INVALID_HANDLE )
    }
    static TCHAR szBuffer[512] = { 0 };

    FormatMessage( FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
                   GetModuleHandle( _T("NTDLL.DLL") ),
                   dwCode, 0, szBuffer, sizeof( szBuffer ), 0 );

    return szBuffer;
}

BOOL NDExceptionReport::GetLogicalAddress( PVOID addr, PTSTR szModule, DWORD len, DWORD& section, DWORD& offset )        
{
    MEMORY_BASIC_INFORMATION mbi;

    if ( !VirtualQuery( addr, &mbi, sizeof(mbi) ) )
        return FALSE;

    DWORD hMod = (DWORD)mbi.AllocationBase;

    if ( !GetModuleFileName( (HMODULE)hMod, szModule, len ) )
        return FALSE;

    PIMAGE_DOS_HEADER pDosHdr = (PIMAGE_DOS_HEADER)hMod;

    PIMAGE_NT_HEADERS pNtHdr = (PIMAGE_NT_HEADERS)(hMod + pDosHdr->e_lfanew);

    PIMAGE_SECTION_HEADER pSection = IMAGE_FIRST_SECTION( pNtHdr );

    DWORD rva = (DWORD)addr - hMod; 

    for (   unsigned i = 0;
            i < pNtHdr->FileHeader.NumberOfSections;
            i++, pSection++ )
    {
        DWORD sectionStart = pSection->VirtualAddress;
        DWORD sectionEnd = sectionStart
                    + max(pSection->SizeOfRawData, pSection->Misc.VirtualSize);

        if ( (rva >= sectionStart) && (rva <= sectionEnd) )
        {
            section = i+1;
            offset = rva - sectionStart;
            return TRUE;
        }
    }

    return FALSE; 
}

void NDExceptionReport::WriteStackDetails( PCONTEXT pContext, bool bWriteVariables )                  
{
    _tprintf( _T("\r\nCall stack:\r\n") );

    _tprintf( _T("Address   Frame     Function            SourceFile\r\n") );

    DWORD dwMachineType = 0;
   
    STACKFRAME sf;
    memset( &sf, 0, sizeof(sf) );

    #ifdef _M_IX86
    sf.AddrPC.Offset       = pContext->Eip;
    sf.AddrPC.Mode         = AddrModeFlat;
    sf.AddrStack.Offset    = pContext->Esp;
    sf.AddrStack.Mode      = AddrModeFlat;
    sf.AddrFrame.Offset    = pContext->Ebp;
    sf.AddrFrame.Mode      = AddrModeFlat;

    dwMachineType = IMAGE_FILE_MACHINE_I386;
    #endif

    while ( 1 )
    {
        if ( ! StackWalk(  dwMachineType,m_hProcess,GetCurrentThread(),&sf,pContext, 0,SymFunctionTableAccess,SymGetModuleBase, 0 ) ) break;                                                                                                                                                                                      
        if ( 0 == sf.AddrFrame.Offset ) break;                     

        _tprintf( _T("%08X  %08X  "), sf.AddrPC.Offset, sf.AddrFrame.Offset );

        BYTE symbolBuffer[ sizeof(SYMBOL_INFO) + 1024 ];
        PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)symbolBuffer;
        pSymbol->SizeOfStruct = sizeof(symbolBuffer);
        pSymbol->MaxNameLen = 1024;                        
        DWORD64 symDisplacement = 0;    
                                       
        if ( SymFromAddr(m_hProcess,sf.AddrPC.Offset,&symDisplacement,pSymbol))
        { _tprintf( _T("%hs+%I64X"), pSymbol->Name, symDisplacement ); }
        else   
        {
            TCHAR szModule[MAX_PATH] = _T("");
            DWORD section = 0, offset = 0;

            GetLogicalAddress(  (PVOID)sf.AddrPC.Offset, szModule, sizeof(szModule), section, offset );
            _tprintf( _T("%04X:%08X %s"), section, offset, szModule );
        }

        IMAGEHLP_LINE lineInfo = { sizeof(IMAGEHLP_LINE) };
        DWORD dwLineDisplacement;
        if ( SymGetLineFromAddr( m_hProcess, sf.AddrPC.Offset, &dwLineDisplacement, &lineInfo ) )
        { _tprintf(_T("  %s line %u"),lineInfo.FileName,lineInfo.LineNumber); }

        _tprintf( _T("\r\n") );

        if ( bWriteVariables )
        {
            IMAGEHLP_STACK_FRAME imagehlpStackFrame;
            imagehlpStackFrame.InstructionOffset = sf.AddrPC.Offset;
            SymSetContext( m_hProcess, &imagehlpStackFrame, 0 );
            SymEnumSymbols( m_hProcess, 0, 0, EnumerateSymbolsCallback, &sf );
            _tprintf( _T("\r\n") );
        }
    }

}

BOOL CALLBACK NDExceptionReport::EnumerateSymbolsCallback( PSYMBOL_INFO pSymInfo, ULONG SymbolSize,PVOID UserContext )    
{
    char szBuffer[2048];
    __try
    {
        if ( FormatSymbolValue( pSymInfo, (STACKFRAME*)UserContext, szBuffer, sizeof(szBuffer) ) )  
            _tprintf( _T("\t%s\r\n"), szBuffer );
    }
    __except( 1 ){ _tprintf( _T("punting on symbol %s\r\n"), pSymInfo->Name ); }

    return TRUE;
}

bool NDExceptionReport::FormatSymbolValue( PSYMBOL_INFO pSym, STACKFRAME * sf, char * pszBuffer, unsigned cbBuffer )                                              
{
    char * pszCurrBuffer = pszBuffer;

    if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_PARAMETER ) pszCurrBuffer += sprintf( pszCurrBuffer, "Parameter " );       
    else if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_LOCAL )pszCurrBuffer += sprintf( pszCurrBuffer, "Local " );
        
    if ( pSym->Tag == 5 ) return false;
        
    pszCurrBuffer += sprintf( pszCurrBuffer, "\'%s\'", pSym->Name );
    DWORD_PTR pVariable = 0; 

    if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGRELATIVE )
    {
        // if ( pSym->Register == 8 )   
        {                              
            pVariable = sf->AddrFrame.Offset;
            pVariable += (DWORD_PTR)pSym->Address;
        }
        // else
        //  return false;
    }
    else if ( pSym->Flags & IMAGEHLP_SYMBOL_INFO_REGISTER ){ return false; } 
    else{ pVariable = (DWORD_PTR)pSym->Address; }

    bool bHandled;
    pszCurrBuffer = DumpTypeIndex(pszCurrBuffer,pSym->ModBase, pSym->TypeIndex, 0, pVariable, bHandled );

    if ( !bHandled )
    {
        BasicType basicType = GetBasicType( pSym->TypeIndex, pSym->ModBase );   
        pszCurrBuffer = FormatOutputValue(pszCurrBuffer, basicType, pSym->Size, (PVOID)pVariable ); 
    }
    return true;
}

char * NDExceptionReport::DumpTypeIndex( char * pszCurrBuffer, DWORD64 modBase, DWORD dwTypeIndex, unsigned nestingLevel, DWORD_PTR offset, bool & bHandled )                                            
{
    bHandled = false;

    WCHAR * pwszTypeName;
    if ( SymGetTypeInfo( m_hProcess, modBase, dwTypeIndex, TI_GET_SYMNAME, &pwszTypeName ) )
    {
        pszCurrBuffer += sprintf( pszCurrBuffer, " %ls", pwszTypeName );
        LocalFree( pwszTypeName );
    }

    DWORD dwChildrenCount = 0;
    SymGetTypeInfo( m_hProcess, modBase, dwTypeIndex, TI_GET_CHILDRENCOUNT, &dwChildrenCount );

    if ( !dwChildrenCount ) return pszCurrBuffer;

    struct FINDCHILDREN : TI_FINDCHILDREN_PARAMS
    {
        ULONG   MoreChildIds[1024];
        FINDCHILDREN(){Count = sizeof(MoreChildIds) / sizeof(MoreChildIds[0]);}
    } children;

    children.Count = dwChildrenCount;
    children.Start= 0;

    if ( !SymGetTypeInfo( m_hProcess, modBase, dwTypeIndex, TI_FINDCHILDREN, &children ) )
    { return pszCurrBuffer; }

    pszCurrBuffer += sprintf( pszCurrBuffer, "\r\n" );

    for ( unsigned i = 0; i < dwChildrenCount; i++ )
    {
        for ( unsigned j = 0; j <= nestingLevel+1; j++ )
            pszCurrBuffer += sprintf( pszCurrBuffer, "\t" );

        bool bHandled2;
        pszCurrBuffer = DumpTypeIndex( pszCurrBuffer, modBase, children.ChildId[i], nestingLevel+1, offset, bHandled2 );                                                                               
        
		if ( !bHandled2 )
        {
            DWORD dwMemberOffset;
            SymGetTypeInfo( m_hProcess, modBase, children.ChildId[i],
                            TI_GET_OFFSET, &dwMemberOffset );

            DWORD typeId;
            SymGetTypeInfo( m_hProcess, modBase, children.ChildId[i],
                            TI_GET_TYPEID, &typeId );

            ULONG64 length;
            SymGetTypeInfo(m_hProcess, modBase, typeId, TI_GET_LENGTH,&length);

            DWORD_PTR dwFinalOffset = offset + dwMemberOffset;

            BasicType basicType = GetBasicType(children.ChildId[i], modBase );

            pszCurrBuffer = FormatOutputValue( pszCurrBuffer, basicType,
                                                length, (PVOID)dwFinalOffset ); 

            pszCurrBuffer += sprintf( pszCurrBuffer, "\r\n" );
        }
    }

    bHandled = true;
    return pszCurrBuffer;
}

char * NDExceptionReport::FormatOutputValue( char * pszCurrBuffer,BasicType basicType,DWORD64 length, PVOID pAddress )                                                                                                                                                           
{
    if ( length == 1 ) pszCurrBuffer += sprintf( pszCurrBuffer, " = %X", *(PBYTE)pAddress );       
    else if ( length == 2 ) pszCurrBuffer += sprintf( pszCurrBuffer, " = %X", *(PWORD)pAddress );       
    else if ( length == 4 )
    {
        if ( basicType == btFloat ){ pszCurrBuffer += sprintf(pszCurrBuffer," = %f", *(PFLOAT)pAddress); }
        else if ( basicType == btChar )
        {
            if ( !IsBadStringPtr( *(PSTR*)pAddress, 32) )
            { pszCurrBuffer += sprintf( pszCurrBuffer, " = \"%.31s\"", *(PDWORD)pAddress ); }
            else pszCurrBuffer += sprintf( pszCurrBuffer, " = %X", *(PDWORD)pAddress );                                            
        }
        else pszCurrBuffer += sprintf(pszCurrBuffer," = %X", *(PDWORD)pAddress);
    }
    else if ( length == 8 )
    {
        if ( basicType == btFloat )
        { pszCurrBuffer += sprintf( pszCurrBuffer, " = %lf", *(double *)pAddress ); }
        else pszCurrBuffer += sprintf( pszCurrBuffer, " = %I64X",*(DWORD64*)pAddress );                                        
    }
    return pszCurrBuffer;
}

BasicType NDExceptionReport::GetBasicType( DWORD typeIndex, DWORD64 modBase )
{
    BasicType basicType;
    if ( SymGetTypeInfo( m_hProcess, modBase, typeIndex,TI_GET_BASETYPE, &basicType ) ) { return basicType; }            
    DWORD typeId;
    if (SymGetTypeInfo(m_hProcess,modBase, typeIndex, TI_GET_TYPEID, &typeId))
    {
        if ( SymGetTypeInfo( m_hProcess, modBase, typeId, TI_GET_BASETYPE, &basicType ) ){ return basicType; }
    }
    return btNoType;
}

int __cdecl NDExceptionReport::_tprintf(const TCHAR * format, ...)
{
    TCHAR szBuff[1024];
    int retValue;
    DWORD cbWritten;
    va_list argptr;
          
    va_start( argptr, format );
    retValue = vsprintf( szBuff, format, argptr );
    va_end( argptr );

    WriteFile(m_hReportFile, szBuff, retValue * sizeof(TCHAR), &cbWritten, 0 );

    return retValue;
}
