/*----------------------------------------------------------------------
Copyright (c)  Gipsysoft. All Rights Reserved.
Please see the file "licence.txt" for licencing details.
File:	FocusControl.h
Owner:	russf@gipsysoft.com
Purpose:

	Abstract class that takes care of setting, getting and determining who has the focus.

----------------------------------------------------------------------*/
#ifndef FOCUSCONTROL_H
#define FOCUSCONTROL_H

class CFocusControlABC
{
public:
	//
	//	Set the focus to this item
	virtual void SetFocus( bool bHasFocus ) = 0;

	//
	//	Perform whatever activation is needed (e.g. launch a URL)
	virtual void Activate() = 0;

	//
	//	Update the display of the item
	virtual void Update() = 0;

	//
	//	Is this item focused
	virtual bool IsFocused() const = 0;

	//
	//	Get the focus items bounding rectangle
	virtual void GetObjectRect( WinHelper::CRect &rcBounds ) const = 0;
};


#endif //FOCUSCONTROL_H