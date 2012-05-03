/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	config.h
Owner:	russf@gipsysoft.com
Purpose:	QHTM configuration

		Use the #defines below to add or remove QHTM functionality.

----------------------------------------------------------------------*/
#ifndef CONFIG_H
#define CONFIG_H


//
//	If you don't want cooltips support
#define QHTM_COOLTIPS

//
//	Comment out to totally remove imaging from QHTM
#define QHTM_BUILD_INTERNAL_IMAGING

//
//	Comment out to prevent QHTM from drawing focus for links
//	as well as keyboard navigation
#define QHTM_ALLOW_FOCUS


//
//	QHTM can use the <control ...> tag to create and display windows controls.
//	Uncomment it to try it. This is experimental and has some BUGS
#define	QHTM_WINDOWS_CONTROL


//
//	Printing 
#define QHTM_ALLOW_PRINT

//
//	Rendering HTML onto any device context
#define QHTM_ALLOW_RENDER

//
//	HTML on a button
#define QHTM_ALLOW_HTML_BUTTON

//
//	Image object callback
#define QHTM_ALLOW_IMAGECALLBACK

//
//	resource callbacks...
#define QHTM_ALLOW_RESOURCECALLBACK

//
//	Allow the QHTM_ADD_HTML functionality
#define QHTM_ALLOW_ADD_HTML

//
//	Define this to stop linking the multiple monitor support
//	This is useful if you are statically linking against MFC
//#define _NO_MULTIMON_SUPPORT


#endif //CONFIG_H