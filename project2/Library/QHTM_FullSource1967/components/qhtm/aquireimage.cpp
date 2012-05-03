/*----------------------------------------------------------------------
Copyright (c) 1998 Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.

File:	AquireImage.cpp
Owner:	russf@gipsysoft.com
Purpose:	Function for handling default image acquisition
----------------------------------------------------------------------*/
#include "stdafx.h"
#include "QHTM.h"
#include "QHTMImage.h"
#include "DebugHlp/DebugHlp.h"
#include "AquireImage.h"
#include "QHTM_Types.h"
#include "stdlib.h"
#include <datasource/ResourceDataSource.h>
#include <datasource/FileDataSource.h>

#ifdef QHTM_BUILD_INTERNAL_IMAGING
static bool LoadFromResource( CImage *pImage, HINSTANCE hInstance, LPCTSTR pcszResourceName )
{
	CResourceDataSource ds;
	//
	//	We try icon group first because it's one of the top level items in the resources
	if (ds.Open( hInstance, pcszResourceName, RT_GROUP_ICON ))
	{
		ds.Reset();
		return pImage->Load( ds );
	}
	else if( ds.Open( hInstance, pcszResourceName, RT_RCDATA ) )
	{
		return pImage->Load( ds );
	}
	else if (ds.Open( hInstance, pcszResourceName, RT_BITMAP ))
	{
		ds.Reset();
		return pImage->Load( ds );
	}
	else if (ds.Open( hInstance, pcszResourceName, RT_ICON ))
	{
		ds.Reset();
		return pImage->Load( ds );
	}
	return false;
}


CQHTMImageABC *AquireImage( HINSTANCE hInstance, LPCTSTR pcszFilePath, LPCTSTR pcszFilename, bool bIsTransparent, COLORREF crForceTransparent )
{
	CQHTMImage *pImage = NULL;

	pImage = new CQHTMImage;

	//
	//	Check for resource based images
	if( !_tcsnicmp( pcszFilename, _T("RES:"), 4 ) )
	{
		pcszFilename += 4;
		ASSERT( _tcslen( pcszFilename  ) );
		if( LoadFromResource( pImage, hInstance, pcszFilename ) )
		{
			if( bIsTransparent ) pImage->ForceTransparent( crForceTransparent );
			return pImage;
		}

		LPTSTR endptr;
		if( LoadFromResource( pImage, hInstance, MAKEINTRESOURCE( _tcstol( pcszFilename, &endptr, 10 ) ) ) )
		{
			if( bIsTransparent ) pImage->ForceTransparent( crForceTransparent );
			return pImage;
		}


		//
		//	Here we attempt to read the resource given the name RES:exeORdll:number
		//	However, despite out valiant efforts it has not worked, at least not for icons
		HMODULE hOther = NULL;
		LPTSTR pszNumber = _tcsrchr( pcszFilename, ':' );
		if( pszNumber )
		{
			*pszNumber = '\000';
			hOther = LoadLibraryEx( pcszFilename, NULL, LOAD_LIBRARY_AS_DATAFILE );
			pcszFilename = pszNumber + 1;
		}
		else
		{
			hOther = LoadLibraryEx( pcszFilename, NULL, LOAD_LIBRARY_AS_DATAFILE );
		}
		
		if( hOther )
		{

			if( LoadFromResource( pImage, hOther, pcszFilename ) )
			{
				if( bIsTransparent ) pImage->ForceTransparent( crForceTransparent );
				return pImage;
			}

			LPTSTR endptr;
			if( LoadFromResource( pImage, hOther, MAKEINTRESOURCE( _tcstol( pcszFilename, &endptr, 10 ) ) ) )
			{
				if( bIsTransparent ) pImage->ForceTransparent( crForceTransparent );
				return pImage;
			}
		}
		pImage->Destroy();
		return NULL;
	}

	//
	//	Check for file based images
	StringClass strImageName( pcszFilePath );
	strImageName += pcszFilename ;

	CFileDataSource file;
	if( file.Open( pcszFilename ) || file.Open( strImageName ) )
	{
		if( pImage->Load( file ) )
		{
			if( bIsTransparent ) pImage->ForceTransparent( crForceTransparent );
			return pImage;
		}
	}

	pImage->Destroy();
	return NULL;
}

#endif	//	QHTM_BUILD_INTERNAL_IMAGING