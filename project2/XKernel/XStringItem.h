#ifndef _X_STRINGITEM
#define _X_STRINGITEM

#include <windows.h>
#include "XItem.h"

class _XStringItem : public _XItem 
{
      private:
             TCHAR *sString;
             int   iAttribute1;
			 DWORD iAttribute2;
			 DWORD iAttribute3;
             int   iString_Size;			 
      protected:

      public:
		 _XStringItem();
	     _XStringItem( TCHAR *sDString,int iAttr1, int iAttr2 = 0 );
	     virtual ~_XStringItem();

         int     Get_Strlength(void){ return strlen(sString); }
	     TCHAR*  Get_String(void){ return sString; }
		 TCHAR*  getFilename(void);
	     void    Set_String(TCHAR *sDString);
		 int     Get_Attribute(int index = 0)
		 { 
				 if( index == 0 ) return iAttribute1; 
				 else if( index == 1 ) return iAttribute2;
				 else return iAttribute3;
		 }
         void    Set_Attribute( int iAttr1, int iAttr2 = 0  )
		 { 
				 iAttribute1 = iAttr1; 
				 iAttribute2 = iAttr2;
		 }
		 
		 void    Set_Attribute2( DWORD iAttr2 )
		 { 
				 iAttribute2 = iAttr2;
		 }

		 void    Set_Attribute3( DWORD iAttr3 )
		 { 
				 iAttribute3 = iAttr3;
		 }

		 void    Add( _XStringItem *DStr );
		 void    Add( TCHAR *DStr );
		 void    Add( BYTE Charactor );
		 void    Add( int Char );

		 void    LShift( void );
		 void    deleteExtention( void );
};

#endif


