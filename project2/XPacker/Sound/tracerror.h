// 
//  [FILE]: tracerror.h
//
//      Includere nel file dove si vuole utilizzare il debug con i messagebox
//  Le Funzioni sono contnute nel file traceerror.cpp
//                                                                         traceerror.c
//  Esempio MYERROR(5000,100,__FILE__,__LINE__,ABORTRETRYIGNORA);
//  Farà riferimento alla tabella codici errore 5000, codice errore 100 ,
//   nella messabox appariranno i pulsanti ABORT RETRY IGNORA
//  Possibili combinazioni
//      OK
//  ABORTIGNORA
//  ABORTRETRYIGNORA

// Tabella 5000	 : Errori prodotti da Direct 
// Tabella 6000  : Errori prodotti da Direct 3D
// Tabella 7000  : Errori prodotti da Direct 3D RM
// Tabella 8000  : Errori prodotti da Direct Sound
// Tabella 9000  : Errori prodotti da Texture Load
// Tabella 10000 : Errori prodotti da Direct Input
// Tabella 11000 : Errori prodotti da Direct Play

#ifndef __TRACERROR_H_
#define __TRACERROR_H_

#define ABORTIGNORA 1
#define OK          3
#define ABORTRETRYIGNORA 2
#define DDERROR 5000
#define D3DERROR 6000
#define D3DRMERROR 7000
#define DSERROR 8000
#define TEXTUREERROR 9000
#define DEFAULT_OK 0

#ifdef __cplusplus
  extern "C" int TraceErrorDD(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  extern "C" int TraceErrorD3D(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  extern "C" int TraceErrorD3DRM(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  extern "C" int TraceErrorDS(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
  extern "C" int TraceErrorD3DTexture(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  extern "C" int TraceErrorDI(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
  extern "C" int TraceErrorDP(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
  extern "C" int TraceErrorNormal( char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  extern "C" int TraceErrorSmacker(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
#else
  int TraceErrorDD(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  int TraceErrorD3D(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  int TraceErrorD3DRM(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  int TraceErrorDS(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
  int TraceErrorD3DTexture(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  int TraceErrorDI(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
  int TraceErrorDP(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
  int TraceErrorNormal( char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag);
  int TraceErrorSmacker(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag);
#endif

//------------------------------------------------------------------
// 
// Funzione     : MYERROR()
//
// SCOPO        : Identifica la tabella codici errore e chiama la funzione
//
//------------------------------------------------------------------
int MYERROR(int tabella, HRESULT indice, char *sFile, int nLine, int flag);

#endif
