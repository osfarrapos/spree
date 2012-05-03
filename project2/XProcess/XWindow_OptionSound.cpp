// XWindow_OptionSound.cpp: implementation of the _XWindow_OptionSound class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XKernel.h"
#include "EmperorOfDragons.h"
#include "EODEXTDEF_GlobalObject.h"
#include "XProc_ProcessDefine.h"
#include "XWindow_OptionSound.h"
#include "XSR_STRINGHEADER.H"

extern void _XDrawEdgedSolidBar( FLOAT left, FLOAT top, FLOAT right, FLOAT bottom, D3DCOLOR color );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_XWindow_OptionSound::_XWindow_OptionSound()
{
	
}

_XWindow_OptionSound::~_XWindow_OptionSound()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Graphic option
BOOL _XWindow_OptionSound::Initialize( void )
{
	SetDrawBorder( FALSE );

	int characterwindowimageindex = g_MainInterfaceTextureArchive.FindResource( "MI_Characterwin.tga" );
	int optionmenuinterface = g_MainInterfaceTextureArchive.FindResource( "MI_System.tga" );

	_XSLIDERCONTROL_STRUCTURE	slidercreatestruct =
	{
		TRUE, { 156, 39 }, 
		{ 88, 12 }, NULL,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		optionmenuinterface,
		{ -5, -4 }, { 12, 12 },
		optionmenuinterface,
		&g_MainInterfaceTextureArchive,
		"HSlider"
	};
	
	SIZE spinbtnsize = { 12, 12 };
	RECT leftspinbtncliprect[3] =
	{
		{ 200, 17, 212, 29 },
		{ 200, 29, 212, 41 },
		{ 200, 41, 212, 53 },
	};
	
	RECT rightspinbtncliprect[3] =
	{
		{ 178,  0,  190, 12 },
		{ 178,  12, 190, 24 },
		{ 178,  24, 190, 36 },
	};

	
	_XCHECKBTN_STRUCTURE checkbtnstruct = { TRUE, { 243, 20 }, 
	{ 14, 14 }, 
	_XDEF_OPTION_SOUND_USEBGM,
	optionmenuinterface,optionmenuinterface,
	&g_MainInterfaceTextureArchive };
	
	m_pUseBGMButton = new _XCheckButton;
	m_pUseBGMButton->Create( checkbtnstruct );	
	m_pUseBGMButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pUseBGMButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pUseBGMButton->SetCheck( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMENABLE, TRUE, TRUE ) );
	m_pUseBGMButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pUseBGMButton );

	slidercreatestruct.commandid = _XDEF_OPTION_SOUND_BGMVOLUME;
	m_pBGMVolumeSlider = new _XHSlider;
	m_pBGMVolumeSlider->Create( slidercreatestruct );
	m_pBGMVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pBGMVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pBGMVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pBGMVolumeSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pBGMVolumeSlider->SetClipRect( 2,244, 90, 256 );
	m_pBGMVolumeSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_pBGMVolumeSlider->SetGabSize( 1 );
	m_pBGMVolumeSlider->ShowWindow( TRUE );
	m_pBGMVolumeSlider->EnableWindow( TRUE );
	FLOAT maxvolume = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_BGMVOLUME, 50, TRUE );
	g_FMODWrapper.SetBGMMaxVolume( (BYTE)(255 * (maxvolume/ 100.0f)) );
	m_pBGMVolumeSlider->SetGaugeValue( maxvolume );
	m_pBGMVolumeSlider->SetUseSpinButton( TRUE, 5 );
	m_pBGMVolumeSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );		
	InsertChildObject( m_pBGMVolumeSlider );

	g_pBGMSoundMaxVolume = (FLOAT)maxvolume / 100.0f;


	checkbtnstruct.position.y = 64;
	checkbtnstruct.commandid = _XDEF_OPTION_SOUND_USEEFFECTSOUND;
	m_pUseEffectSoundButton= new _XCheckButton;
	m_pUseEffectSoundButton->Create( checkbtnstruct );	
	m_pUseEffectSoundButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pUseEffectSoundButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pUseEffectSoundButton->SetCheck( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDEFFECTENABLE, FALSE, TRUE ) );
	m_pUseEffectSoundButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pUseEffectSoundButton );

	slidercreatestruct.position.y = 83;
	slidercreatestruct.commandid = _XDEF_OPTION_SOUND_EFFECTSOUNDVOLUME;
	m_pEffectSoundVolumeSlider = new _XHSlider;
	m_pEffectSoundVolumeSlider->Create( slidercreatestruct );
	m_pEffectSoundVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pEffectSoundVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pEffectSoundVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pEffectSoundVolumeSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pEffectSoundVolumeSlider->SetClipRect( 2,244, 90, 256 );
	m_pEffectSoundVolumeSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_pEffectSoundVolumeSlider->SetGabSize( 1 );
	m_pEffectSoundVolumeSlider->ShowWindow( TRUE );
	m_pEffectSoundVolumeSlider->EnableWindow( TRUE );
	FLOAT effectvolume = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_SOUNDEFFECTVOLUME, 75, TRUE );
	m_pEffectSoundVolumeSlider->SetGaugeValue( effectvolume );	
	g_FMODWrapper.SetVolume( (BYTE)(255 * (effectvolume/ 100.0f)) );

	m_pEffectSoundVolumeSlider->SetUseSpinButton( TRUE, 5 );
	m_pEffectSoundVolumeSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );	
	InsertChildObject( m_pEffectSoundVolumeSlider );

	checkbtnstruct.position.y = 107;
	checkbtnstruct.commandid = _XDEF_OPTION_SOUND_USEENVSOUND;
	m_pUseEnvSoundButton= new _XCheckButton;
	m_pUseEnvSoundButton->Create( checkbtnstruct );	
	m_pUseEnvSoundButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pUseEnvSoundButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	g_3DSoundEffectFlag = g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_3DSOUNDENABLE, TRUE, TRUE );
	m_pUseEnvSoundButton->SetCheck( g_3DSoundEffectFlag );
	m_pUseEnvSoundButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pUseEnvSoundButton );


	slidercreatestruct.position.y = 126;
	slidercreatestruct.commandid = _XDEF_OPTION_SOUND_ENVVOLUMESLIDER;
	m_pEnvSoundVolumeSlider = new _XHSlider;
	m_pEnvSoundVolumeSlider->Create( slidercreatestruct );
	m_pEnvSoundVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_NORMAL, 200, 2, 212, 14 );
	m_pEnvSoundVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_READY,  200, 2, 212, 14 );
	m_pEnvSoundVolumeSlider->m_TrackBarButton.SetButtonImageClipRect( _XBUTTONIMAGE_CLICK,  200, 2, 212, 14 );
	m_pEnvSoundVolumeSlider->m_IndicatorImage.SetClipRect( 161,244 , 249, 256 );
	m_pEnvSoundVolumeSlider->SetClipRect( 2,244, 90, 256 );
	m_pEnvSoundVolumeSlider->m_FColor = D3DCOLOR_ARGB( 255, 255, 255, 255 );
	m_pEnvSoundVolumeSlider->SetGabSize( 1 );
	m_pEnvSoundVolumeSlider->ShowWindow( TRUE );
	m_pEnvSoundVolumeSlider->EnableWindow( TRUE );
	FLOAT envsoundvolume = (FLOAT)g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_ENVSOUNDEFFECTVOLUME,  100, TRUE );
	g_FMODWrapper.FL_SetVolume( (BYTE)(255 * (envsoundvolume / 100.0f)) );
	m_pEnvSoundVolumeSlider->SetGaugeValue( envsoundvolume );
	m_pEnvSoundVolumeSlider->SetUseSpinButton( TRUE, 5 );
	m_pEnvSoundVolumeSlider->CreateSpinButton( spinbtnsize, &g_MainInterfaceTextureArchive, optionmenuinterface, characterwindowimageindex, leftspinbtncliprect, rightspinbtncliprect );	
	InsertChildObject( m_pEnvSoundVolumeSlider );
	

	checkbtnstruct.position.y = 164;
	checkbtnstruct.commandid = _XDEF_OPTION_SOUND_USEGUIDESOUND;
	m_pUseGuideSoundButton= new _XCheckButton;
	m_pUseGuideSoundButton->Create( checkbtnstruct );	
	m_pUseGuideSoundButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_NORMAL, 216,31, 230, 45 );
	m_pUseGuideSoundButton->SetButtonImageClipRect( _XCHECKBUTTONIMAGE_CLICK,  216, 0, 230, 14 );
	m_pUseGuideSoundButton->SetCheck( g_RegManager._XRegReadInt( ID_REGSTR_MASTERKEY, ID_REGSTR_INDY21KEY, ID_REGSTR_USEREALTIMEGENEOBJECTANI, TRUE, TRUE ) );
	m_pUseGuideSoundButton->SetButtonSwapMode( FALSE );
	InsertChildObject( m_pUseGuideSoundButton );

	return TRUE;
}


void _XWindow_OptionSound::DestroyWindow( void )
{
	_XWindow::DestroyWindow();
}


BOOL _XWindow_OptionSound::Process( _XGUIObject*& pfocusobject )
{
	if( !this->m_ShowWindow ) return FALSE;
	if( !this->m_Enable     ) return FALSE;
	
	if( !_XWindow::Process(pfocusobject) ) return FALSE;
	
	return TRUE;
}	

void _XWindow_OptionSound::Draw( _XGUIObject*& pfocusobject )
{
	if(this->m_ShowWindow)
	{
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 9,  m_WindowPosition.x + 269, m_WindowPosition.y + 149, 0x40000000 );

		_XDrawEdgedSolidBar( m_WindowPosition.x + 13, m_WindowPosition.y + 14,  m_WindowPosition.x + 261, m_WindowPosition.y + 55, 0x40000000 );
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 58,  m_WindowPosition.x + 269, m_WindowPosition.y + 99, 0x40000000 );
		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 102,  m_WindowPosition.x + 269, m_WindowPosition.y + 143, 0x40000000 );


		_XDrawEdgedSolidBar( m_WindowPosition.x + 6, m_WindowPosition.y + 153,  m_WindowPosition.x + 269, m_WindowPosition.y +  297, 0x80000000 );		
	}

	_XWindow::Draw(pfocusobject);
	
	if(!this->m_ShowWindow) return;
	if( m_WindowAnimationType != _XW_ANIMTYPE_NONE ) return;


	g_XBaseFont->SetColor( D3DCOLOR_ARGB( 255, 180, 180, 180 ) );
	
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 21, _XGETINTERFACETEXT(ID_STRING_OPTION_MUSIC) );	// _T("배경음악")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 39, _XGETINTERFACETEXT(ID_STRING_OPTION_MUSICVOLUME) );	// _T("배경음악 크기")
	
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 64, _XGETINTERFACETEXT(ID_STRING_OPTION_SOUND) );	// _T("기본 효과음")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 82, _XGETINTERFACETEXT(ID_STRING_OPTION_SOUNDVOLUME) );	// _T("기본 효과음 크기")
	
	
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 108, _XGETINTERFACETEXT(ID_STRING_OPTION_ENVEFFECT) );		// _T("환경 효과음")
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 126, _XGETINTERFACETEXT(ID_STRING_OPTION_ENVEFFECTVOLUME) );	// _T("환경 효과음 크기")
		
	g_XBaseFont->Puts( m_WindowPosition.x + 21, m_WindowPosition.y + 164, _XGETINTERFACETEXT(ID_STRING_OPTION_GUIDESOUND) );	// _T("가이드 효과음")
		
	g_XBaseFont->Flush();
}