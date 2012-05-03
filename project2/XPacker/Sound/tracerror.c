/*==========================================================================
  [FILE] tracerror.c
  
 */


// Le Macros TRY_DD, TRY_DS, TRY_D3D, TRY_D3DRM sono definite nel file Tracerror.h e
// sono utili per checcare gli errori.

//#define INITGUID


// File Inclusi
#include "myenv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ddraw.h>
#include <d3d.h>
#include <d3drm.h>
#include <dsound.h>
#include <dinput.h>
#include <dplay.h>
#include "gset.h"
#include "tracerror.h"
//#include "resource.h"
//#include "func_mouse.h"
//#include "load_textures.h"
//#include "write_string.h"
#include "funcalloc.h"
//#include "menu_util.h"
#include "lingua.h"

extern char Path_Program[];

#define ABORTIGNORA 1
#define OK                      3
#define ABORTRETRYIGNORA 2
#define PATHERROR "\\data\\Tab_Err\\"
#define LINGUA ".ita"
#define LUNGERROR     250
#define LUNGMENU	  10
#define MAXNUMBEROBJ  1000
#define NUMLINGUE 6
#define STRINGA 256
#define TOTALSTRINGA 2000

#ifndef _MYDEBUG
#define _MYDEBUG
#endif

//extern void finiGraphicsObjects(void);
//------------------------------------------------------------------
// 
// Funzione     : RegError()
//
// SCOPO        : Registra un error
//
//------------------------------------------------------------------
int RegError(char *sErr,int flag)
{
	FILE *filer;
	int val;
	char            g_sError[TOTALSTRINGA];
	
    sprintf(g_sError, "%s\0", sErr);
	// _MYFILE per scrivere su un file error.txt l'errore occorso
	//	#ifdef MYFILE
	filer=fopen("errore.log","w");
	fwrite( g_sError, sizeof( char ), strlen(g_sError), filer );
	fclose(filer);
	//	#endif
#ifdef BOX
	//lpDDRoot->lpVtbl->SetCooperativeLevel(lpDDRoot, hwnd,DDSCL_NORMAL);
    //val=MyMessageBox(0,g_sError,"Sommario Errore",flag);
#endif
	//#ifdef _ERR_GRAPH
	//MyMessageBox_Graph(sErr);
	//finiGraphicsObjects();
	//CloseGameInput();
	OutputDebugString(g_sError);
	WinExec("puma_error.exe /error.log",SW_SHOWDEFAULT);
	exit(1);
	//#endif
    
	return(val);
	//    g_bErrorOccured = TRUE;
}


void LeggiFile(FILE *filerr,HRESULT hErr,char dderr[STRINGA])
{
	//char dderr[STRINGA];
	long dist=0;
	strcpy(dderr,"");
	dist=(long)hErr;
	fseek( filerr, (dist-1)*LUNGERROR, SEEK_SET);
	fread( dderr,sizeof( char ), LUNGERROR, filerr );
	if (feof(filerr))
	{
		strcpy(dderr,"");
	}
	fclose(filerr);
	//return dderr;
}


int TraceErrorNormal( char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag)
{       
	//	char *dderrtemp;
    char dderr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;
	//strcpy(errorfile,"d:");
	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	//errorfile[0]=drive_context;
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		if (hErr!=0)
			LeggiFile(filerr,hErr,dderr);
		else
			sprintf(dderr, "Unknown Error");
	}
	else
		sprintf(dderr, "Unknown Error");
	{
#ifdef _MYDEBUG
		sprintf(err, "Our Error \n%s\nin file %s\nat line %d", dderr, sFile, nLine);
#else
		sprintf(err,"Our Error\n%s",dderr);
#endif
    }
    return RegError(err,flag);
}


int TraceErrorDD( char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag)
{       
	//	char *dderrtemp;
    char dderr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;
	//strcpy(errorfile,"d:");
	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	//errorfile[0]=drive_context;
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		switch (hErr)
		{
		case DDERR_ALREADYINITIALIZED : reg=1;break;
		case DDERR_CANNOTATTACHSURFACE : reg=2;break;
		case DDERR_CANNOTDETACHSURFACE : reg=3;break;
		case DDERR_CURRENTLYNOTAVAIL : reg=4;break;
		case DDERR_EXCEPTION : reg=5;break;
		case DDERR_GENERIC : reg=6;break;
		case DDERR_HEIGHTALIGN : reg=7;break;
		case DDERR_INCOMPATIBLEPRIMARY : reg=8;break;
		case DDERR_INVALIDCAPS : reg=9;break;
		case DDERR_INVALIDCLIPLIST : reg=10;break;
		case DDERR_INVALIDMODE : reg=11;break;
		case DDERR_INVALIDOBJECT : reg=12;break;
		case DDERR_INVALIDPARAMS : reg=13;break;
		case DDERR_INVALIDPIXELFORMAT : reg=14;break;
		case DDERR_INVALIDRECT : reg=15;break;
		case DDERR_LOCKEDSURFACES : reg=16;break;
		case DDERR_NO3D : reg=17;break;
		case DDERR_NOALPHAHW : reg=18;break;
		case DDERR_NOCLIPLIST : reg=19;break;
		case DDERR_NOCOLORCONVHW : reg=20;break;
		case DDERR_NOCOOPERATIVELEVELSET : reg=21;break;
		case DDERR_NOCOLORKEY : reg=22;break;
		case DDERR_NOCOLORKEYHW : reg=23;break;
		case DDERR_NODIRECTDRAWSUPPORT : reg=24;break;
		case DDERR_NOEXCLUSIVEMODE : reg=25;break;
		case DDERR_NOFLIPHW : reg=26;break;
		case DDERR_NOGDI : reg=27;break;
		case DDERR_NOMIRRORHW : reg=28;break;
		case DDERR_NOTFOUND : reg=29;break;
		case DDERR_NOOVERLAYHW : reg=30;break;
		case DDERR_NORASTEROPHW : reg=31;break;
		case DDERR_NOROTATIONHW : reg=32;break;
		case DDERR_NOSTRETCHHW : reg=33;break;
		case DDERR_NOT4BITCOLOR : reg=34;break;
		case DDERR_NOT4BITCOLORINDEX : reg=35;break;
		case DDERR_NOT8BITCOLOR : reg=36;break;
		case DDERR_NOTEXTUREHW : reg=37;break;
		case DDERR_NOVSYNCHW : reg=38;break;
		case DDERR_NOZBUFFERHW : reg=39;break;
		case DDERR_NOZOVERLAYHW : reg=40;break;
		case DDERR_OUTOFCAPS : reg=41;break;
		case DDERR_OUTOFMEMORY : reg=42;break;
		case DDERR_OUTOFVIDEOMEMORY : reg=43;break;
		case DDERR_OVERLAYCANTCLIP : reg=44;break;
		case DDERR_OVERLAYCOLORKEYONLYONEACTIVE : reg=45;break;
		case DDERR_PALETTEBUSY : reg=46;break;
		case DDERR_COLORKEYNOTSET : reg=47;break;
		case DDERR_SURFACEALREADYATTACHED : reg=48;break;
		case DDERR_SURFACEALREADYDEPENDENT : reg=49;break;
		case DDERR_SURFACEBUSY : reg=50;break;
		case DDERR_CANTLOCKSURFACE : reg=51;break;
		case DDERR_SURFACEISOBSCURED : reg=52;break;
		case DDERR_SURFACELOST : reg=53;break;
		case DDERR_SURFACENOTATTACHED : reg=54;break;
		case DDERR_TOOBIGHEIGHT : reg=55;break;
		case DDERR_TOOBIGSIZE : reg=56;break;
		case DDERR_TOOBIGWIDTH : reg=57;break;
		case DDERR_UNSUPPORTED : reg=58;break;
		case DDERR_UNSUPPORTEDFORMAT : reg=59;break;
		case DDERR_UNSUPPORTEDMASK : reg=60;break;
		case DDERR_VERTICALBLANKINPROGRESS : reg=61;break;
		case DDERR_WASSTILLDRAWING : reg=62;break;
		case DDERR_XALIGN : reg=63;break;
		case DDERR_INVALIDDIRECTDRAWGUID : reg=64;break;
		case DDERR_DIRECTDRAWALREADYCREATED : reg=65;break;
		case DDERR_NODIRECTDRAWHW : reg=66;break;
		case DDERR_PRIMARYSURFACEALREADYEXISTS : reg=67;break;
		case DDERR_NOEMULATION : reg=68;break;
		case DDERR_REGIONTOOSMALL : reg=69;break;
		case DDERR_CLIPPERISUSINGHWND : reg=70;break;
		case DDERR_NOCLIPPERATTACHED : reg=71;break;
		case DDERR_NOHWND : reg=72;break;
		case DDERR_HWNDSUBCLASSED : reg=73;break;
		case DDERR_HWNDALREADYSET : reg=74;break;
		case DDERR_NOPALETTEATTACHED : reg=75;break;
		case DDERR_NOPALETTEHW : reg=76;break;
		case DDERR_BLTFASTCANTCLIP : reg=77;break;
		case DDERR_NOBLTHW : reg=78;break;
		case DDERR_NODDROPSHW : reg=79;break;
		case DDERR_OVERLAYNOTVISIBLE : reg=80;break;
		case DDERR_NOOVERLAYDEST : reg=81;break;
		case DDERR_INVALIDPOSITION : reg=82;break;
		case DDERR_NOTAOVERLAYSURFACE : reg=83;break;
		case DDERR_EXCLUSIVEMODEALREADYSET : reg=84;break;
		case DDERR_CANTDUPLICATE  : reg=85;break;
		case DDERR_NOTFLIPPABLE: reg=86;break;
		case DDERR_NOTLOCKED : reg=87;break;
		case DDERR_CANTCREATEDC : reg=88;break;
		case DDERR_NODC : reg=89;break;
		case DDERR_WRONGMODE : reg=90;break;
		case DDERR_IMPLICITLYCREATED : reg=91;break;
		case DDERR_NOTPALETTIZED : reg=92;break;
		case DDERR_UNSUPPORTEDMODE : reg=93;break;
		case DDERR_NOMIPMAPHW : reg=94;break;
		case DDERR_INVALIDSURFACETYPE : reg=95;break;
		case DDERR_DCALREADYCREATED : reg=96;break;
		case DDERR_CANTPAGELOCK : reg=97;break;
		case DDERR_CANTPAGEUNLOCK : reg=98;break;
		case DDERR_NOTPAGELOCKED : reg=99;break;
		case DDERR_NOTINITIALIZED : reg=100;break;
		default : sprintf(dderr, "Unknown Error"); break;
		};
	}
	if (reg!=0)
		LeggiFile(filerr,reg,dderr);
	else
		sprintf(dderr, "Unknown Error");
	{
		//#ifdef _MYDEBUG
		sprintf(err, "DirectDraw Error \n%s\nin file %s\nat line %d", dderr, sFile, nLine);
		// #else
		//  sprintf(err,"DirectDraw Error\n%s",dderr);
		// #endif
    }
    return RegError(err,flag);
}

//----------------------------------------------------------------------
// 
// Function     : TraceErrorD3D()
//
// Purpose      : Traces an error (Direct3D)
//
//----------------------------------------------------------------------

int TraceErrorD3D(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag)
{       
    char d3derr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;
	//strcpy(errorfile,"d:");
	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	//errorfile[0]=drive_context;
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		
		switch (hErr)
		{
        case D3DERR_BADMAJORVERSION : reg=1; break;
        case D3DERR_BADMINORVERSION : reg=2; break;
        case D3DERR_EXECUTE_CREATE_FAILED : reg=3; break;
        case D3DERR_EXECUTE_DESTROY_FAILED : reg=4; break;
        case D3DERR_EXECUTE_LOCK_FAILED : reg=5; break;
        case D3DERR_EXECUTE_UNLOCK_FAILED : reg=6; break;
        case D3DERR_EXECUTE_LOCKED : reg=7; break;
        case D3DERR_EXECUTE_NOT_LOCKED : reg=8; break;
        case D3DERR_EXECUTE_FAILED : reg=9; break;
        case D3DERR_EXECUTE_CLIPPED_FAILED : reg=10; break;
        case D3DERR_TEXTURE_NO_SUPPORT : reg=11; break;
        case D3DERR_TEXTURE_CREATE_FAILED : reg=12; break;
        case D3DERR_TEXTURE_DESTROY_FAILED : reg=13; break;
        case D3DERR_TEXTURE_LOCK_FAILED : reg=14; break;
        case D3DERR_TEXTURE_UNLOCK_FAILED : reg=15; break;
        case D3DERR_TEXTURE_LOAD_FAILED : reg=16; break;
        case D3DERR_TEXTURE_SWAP_FAILED : reg=17; break;
        case D3DERR_TEXTURE_LOCKED : reg=18; break;
        case D3DERR_TEXTURE_NOT_LOCKED : reg=19; break;
        case D3DERR_TEXTURE_GETSURF_FAILED : reg=20; break;
        case D3DERR_MATRIX_CREATE_FAILED : reg=21; break;
        case D3DERR_MATRIX_DESTROY_FAILED : reg=22; break;
        case D3DERR_MATRIX_SETDATA_FAILED : reg=23; break;
        case D3DERR_MATRIX_GETDATA_FAILED : reg=24; break;
        case D3DERR_SETVIEWPORTDATA_FAILED : reg=25; break;
        case D3DERR_MATERIAL_CREATE_FAILED : reg=26; break;
        case D3DERR_MATERIAL_DESTROY_FAILED : reg=27; break;
        case D3DERR_MATERIAL_SETDATA_FAILED : reg=28; break;
        case D3DERR_MATERIAL_GETDATA_FAILED : reg=29; break;
        case D3DERR_LIGHT_SET_FAILED : reg=30; break;
        case D3DERR_SCENE_IN_SCENE : reg=31; break;
        case D3DERR_SCENE_NOT_IN_SCENE : reg=32; break;
        case D3DERR_SCENE_BEGIN_FAILED : reg=33; break;
        case D3DERR_SCENE_END_FAILED : reg=34; break;
			
        default : sprintf(d3derr, "Unknown Error"); break;
		}
	}
	if (reg!=0)
		LeggiFile(filerr,reg,d3derr);
	else
		sprintf(d3derr, "Unknown Error");
	
	{
		// #ifdef _MYDEBUG
		sprintf(err, "Direct3D Error \n%s\nin file %s\n alla linea %d", d3derr, sFile, nLine);
		//  #else
		//   sprintf(err,"Direct3D Error \n%s",d3derr);
		//  #endif
    }
    return RegError(err,flag);
}

//----------------------------------------------------------------------
// 
// Funzione     : TraceErrorD3DRM()
//
// Scopo        : Traces an error (Direct3D retained mode)
//
//----------------------------------------------------------------------

int TraceErrorD3DRM(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag)
{       
    char d3drmerr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;
	//strcpy(errorfile,"d:");
	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	//errorfile[0]=drive_context;
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		
		switch (hErr)
		{
        case D3DRMERR_BADOBJECT : reg=1; break;
        case D3DRMERR_BADTYPE : reg=2; break;
        case D3DRMERR_BADALLOC : reg=3; break;
        case D3DRMERR_FACEUSED : reg=4; break;
        case D3DRMERR_NOTFOUND : reg=5; break;
        case D3DRMERR_NOTDONEYET : reg=6; break;
        case D3DRMERR_FILENOTFOUND : reg=7; break;
        case D3DRMERR_BADFILE : reg=8; break;
        case D3DRMERR_BADDEVICE : reg=9; break;
        case D3DRMERR_BADVALUE : reg=10; break;
        case D3DRMERR_BADMAJORVERSION : reg=11; break;
        case D3DRMERR_BADMINORVERSION : reg=12; break;
        case D3DRMERR_UNABLETOEXECUTE : reg=13; break;
			
        default : sprintf(d3drmerr, "Unknown Error"); break;
		}
	}
	if (reg!=0)
		LeggiFile(filerr,reg,d3drmerr);
	else
		sprintf(d3drmerr, "Unknown Error");
	
	{
		sprintf(err, "Direct3D-RM Error \n%s\nin file %s\n alla linea %d", d3drmerr, sFile, nLine);
    }
    return RegError(err,flag);
}


//----------------------------------------------------------------------
// 
// Funzione     : TraceErrorD3DTexture()
//
// Scopo        : Traces an error (Direct3D Texture)
//
//----------------------------------------------------------------------

int TraceErrorD3DTexture(char *nomefile,char *sFile, int nLine,HRESULT hErr,int flag)
{       
    char merr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;

	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		
		switch (hErr)
		{
        case 10 : reg=1; break;
		case 20 : reg=2;break;
		case 30 : reg=3;break;
		case 40 : reg=4;break;
		case 50 : reg=5;break;
		case 60 : reg=6;break;
		case 70 : reg=7;break;
		case 80 : reg=8;break;
		case 90 : reg=9;break;
		case 100: reg=10;break;
        default : sprintf(merr, "Unknown Error"); break;
		}
	}
	if (reg!=0)
		LeggiFile(filerr,reg,merr);
	else
		sprintf(merr, "Unknown Error");
	{
		sprintf(err, "Direct3D-Texture Error \n%s\nin file %s\n alla linea %d", merr, sFile, nLine);
    }
    return RegError(err,flag);
}


//----------------------------------------------------------------------
// 
// Function     : TraceErrorDS()
//
// Purpose      : Traces an error (DirectSound)
//
//----------------------------------------------------------------------

int TraceErrorDS(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag)
{       
    char dserr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;
	//strcpy(errorfile,"d:");
	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	//errorfile[0]=drive_context;
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		
		switch (hErr)
		{
		case DSERR_ALLOCATED : reg=1; break;
		case DSERR_ALREADYINITIALIZED : reg=2;break;
		case DSERR_BADFORMAT : reg=3;break;
		case DSERR_BUFFERLOST : reg=4;break;
		case DSERR_CONTROLUNAVAIL : reg=5;break;
		case DSERR_GENERIC : reg=6;break;
		case DSERR_INVALIDCALL : reg=7;break;
		case DSERR_INVALIDPARAM : reg=8;break;
		case DSERR_NOAGGREGATION : reg=9;break;
		case DSERR_NODRIVER : reg=10;break;
		case DSERR_NOINTERFACE : reg=11;break;
		case DSERR_OTHERAPPHASPRIO : reg=12;break;
		case DSERR_OUTOFMEMORY : reg=13;break;
		case DSERR_PRIOLEVELNEEDED : reg=14;break;
		case DSERR_UNINITIALIZED : reg=15;break;
		case DSERR_UNSUPPORTED : reg=16;break;
			
			///////////
			
		default : sprintf(dserr, "Unknown Error"); break;
		}
	}
	if (reg!=0)
		LeggiFile(filerr,reg,dserr);
	else
		sprintf(dserr, "Unknown Error");
	
	{
#ifdef _MYDEBUG
		sprintf(err, "DirectSound Error \n%s\nin file %s\n alla linea %d", dserr, sFile, nLine);
#else
		sprintf(err,"DirectSound Error \n%s",dserr);
#endif
    }
    return RegError(err,flag);
}

//----------------------------------------------------------------------
// 
// Function     : TraceErrorSmacker()
//
// Purpose      : Traces an error (Smacker)
//
//----------------------------------------------------------------------

int TraceErrorSmacker(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag)
{       
    char dserr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;
	//strcpy(errorfile,"d:");
	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	//errorfile[0]=drive_context;
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		
		switch (hErr)
		{
        case 10: reg=1; break;
		case 20: reg=2;break;
		case 30: reg=3;break;
		case 40: reg=4;break;
        default : sprintf(dserr, "Unknown Error"); break;
		}
		if (reg!=0)
			LeggiFile(filerr,reg,dserr);
		else
			sprintf(dserr, "Unknown Error");
	}
	{
#ifdef _MYDEBUG
		sprintf(err, "Smacker Error \n%s\nin file %s\n alla linea %d", dserr, sFile, nLine);
#else
		sprintf(err,"Smacker Error \n%s",dserr);
#endif
    }
    return RegError(err,flag);
}


//----------------------------------------------------------------------
// 
// Function     : TraceErrorDI()
//
// Purpose      : Traces an error (DirectInput)
//
//----------------------------------------------------------------------

int TraceErrorDI(char *nomefile,char *sFile, int nLine,HRESULT hErr, int flag)
{       
    char dierr[STRINGA];
    char err[TOTALSTRINGA];
	char errorfile[_MAX_PATH];
	HRESULT reg=0;
	FILE *filerr;
	//strcpy(errorfile,"d:");
	strcpy(errorfile,Path_Program);
	strcat(errorfile,PATHERROR);
	strcat(errorfile,nomefile);
	//errorfile[0]=drive_context;
	filerr=fopen(errorfile,"r");
	if (filerr!=NULL)
	{
		switch (hErr) {
		case DI_BUFFEROVERFLOW : reg=1;break;
		case DI_DOWNLOADSKIPPED : reg=2;break;
		case DI_EFFECTRESTARTED : reg=3;break;
		case DI_POLLEDDEVICE : reg=6;break;
		case DI_TRUNCATED : reg=8;break;
		case DI_TRUNCATEDANDRESTARTED : reg=9;break;
		case DIERR_ACQUIRED : reg=10;break;
		case DIERR_ALREADYINITIALIZED : reg=11;break;
		case DIERR_BADDRIVERVER : reg=12;break;
		case DIERR_BETADIRECTINPUTVERSION : reg=13;break;
		case DIERR_DEVICEFULL : reg=14;break;
		case DIERR_DEVICENOTREG : reg=15;break;
		case DIERR_EFFECTPLAYING : reg=16;break;
		case DIERR_HASEFFECTS : reg=17;break;
		case DIERR_GENERIC : reg=18;break;
		case DIERR_HANDLEEXISTS : reg=19;break;
		case DIERR_INCOMPLETEEFFECT : reg=20;break;
		case DIERR_INPUTLOST : reg=21;break;
		case DIERR_INVALIDPARAM : reg=22;break;
		case DIERR_MOREDATA  : reg=23;break;
		case DIERR_NOAGGREGATION : reg=24;break;
		case DIERR_NOINTERFACE : reg=25;break;
		case DIERR_NOTACQUIRED : reg=26;break;
		case DIERR_NOTBUFFERED : reg=27;break;
		case DIERR_NOTDOWNLOADED : reg=28;break;
		case DIERR_NOTEXCLUSIVEACQUIRED : reg=29;break;
		case DIERR_NOTFOUND : reg=30;break;
		case DIERR_NOTINITIALIZED : reg=31;break;
		case DIERR_OLDDIRECTINPUTVERSION : reg=33;break;
		case DIERR_OUTOFMEMORY : reg=35;break;
		case DIERR_UNSUPPORTED : reg=37;break;
		case E_PENDING : reg=38;break;
        default : sprintf(dierr, "Unknown Error"); break;
		}
		if (reg!=0)
			LeggiFile(filerr,reg,dierr);
		else
			sprintf(dierr, "Unknown Error");
	}
	{
#ifdef _MYDEBUG
		sprintf(err, "DirectInput Error \n%s\nin file %s\n alla linea %d", dierr, sFile, nLine);
#else
		sprintf(err,"DirectInput Error \n%s",dierr);
#endif
    }
    return RegError(err,flag);
}

int MYERROR(int tabella,HRESULT indice,char *sFile,int nLine,int flag)
{
	char nometabella[5];
	char nomefile[13];
	char estensione[5];
	int value=0;
	int base=10;
	
	strcpy(estensione,".ita");
	
	_itoa( tabella, nometabella, base );
	
	if ((indice!=DD_OK)|(indice!=D3D_OK)|(indice!=D3DRM_OK)
		|(indice!=DS_OK)|(indice!=DP_OK)|(indice!=DI_OK)
		|(indice!=DEFAULT_OK))	{
		strcpy(nomefile,"err");
		strcat(nomefile,nometabella);
		strcat(nomefile,lingua[Lingua_Scelta]);
		
		//  lpDDRoot->lpVtbl->SetCooperativeLevel(lpDDRoot, hwnd,DDSCL_NORMAL);
		switch (tabella)		{
		case 5000:{ value=TraceErrorDD(nomefile,sFile,nLine,indice,flag); };break;
			
		case 6000:{ value=TraceErrorD3D(nomefile,sFile,nLine,indice,flag); };break;
			
		case 7000:{ value=TraceErrorD3DRM(nomefile,sFile,nLine,indice,flag); };break;
			
		case 8000:{  value=TraceErrorDS(nomefile,sFile,nLine,indice,flag); };break;
			
		case 9000:{  value=TraceErrorD3DTexture(nomefile,sFile,nLine,indice,flag); };break;
			
		case 10000: {  value=TraceErrorDI(nomefile,sFile,nLine,indice,flag); };break;
			
			//case 11000: {  value=TraceErrorDP(nomefile,sFile,nLine,indice,flag); };break;
			
		case 24000: {  value=TraceErrorSmacker(nomefile,sFile,nLine,indice,flag); };break;
			
		default :{ value=TraceErrorNormal(nomefile,sFile,nLine,indice,flag); };break;
		};
		
	}
	if (value==IDABORT) 
		exit(0);
	if (value==IDOK)
	{
		//lpDDRoot->lpVtbl->SetCooperativeLevel(lpDDRoot, hwnd,DDSCL_EXCLUSIVE|DDSCL_FULLSCREEN);//NORMAL);
		exit(0);
	}
	//        if (value==IDOK) exit(0);
	return value;
}

