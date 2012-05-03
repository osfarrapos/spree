#ifndef __XDIALOGS_H__
#define __XDIALOGS_H__
#ifdef __cplusplus

#include "xbasedefs.h"

void SetDialogsInitialDir(TCHAR* p_strInitialDir);
bool OpenFileDialog(XString& p_strFilePath,
				  TCHAR* p_strTitle = "Open",
				  TCHAR* p_strFilters = "All Files\0*.*\0",
				  TCHAR* p_strDefExt = "",
				  bool p_bChangeDir = false);
bool SaveFileDialog(XString& p_strFilePath,
				  TCHAR* p_strTitle = "Save",
				  TCHAR* p_strFilters = "All Files\0*.*\0",
				  TCHAR* p_strDefExt="",
				  bool p_bChangeDir = false);
bool ChooseColorDialog(_XRGB *out);

#endif // __cplusplus
#endif // __XDIALOGS_H__
