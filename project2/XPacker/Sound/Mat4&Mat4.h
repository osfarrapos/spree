
/*************************
 * Function declaration: *
 *************************/

__inline void MatPerMat_Asm(void);
/*   
  Example: 
    ..........
           __asm lea edi, dword ptr["destination matrix"]
           MatPerMat_Asm(lpSrc1, lpSrc2);
    ..........
    ecx = lpScr1
    edx = lpScr2
    edi = destination matrix
    NB: you might use as destination Matrix only the first ONE
        otherwise (using the second) you'll have a wrong result
*/



__inline void Vect4PerMat_Asm(void);
/*
  ESI: (destination vertex)
  ECX: lpVertex
  EDX: lpMatrix
*/













/************************
 *    implementation:   *
 ************************/


 
 
//__declspec ( naked ) 
void MatPerMat_Asm(void)//LPD3DMATRIX lpSrc1, LPD3DMATRIX lpSrc2)
{                                   
    __asm
    {   
        fld  dword ptr [ecx+0*16+0*4]
        fmul dword ptr [edx+0*16+0*4]

        fld  dword ptr [ecx+0*16+1*4]
        fmul dword ptr [edx+1*16+0*4]

        fld  dword ptr [ecx+0*16+2*4]
        fmul dword ptr [edx+2*16+0*4]

        fld  dword ptr [ecx+0*16+3*4]
        fmul dword ptr [edx+3*16+0*4]
//
        fld  dword ptr [ecx+0*16+0*4]
        fmul dword ptr [edx+0*16+1*4]
  
        fld  dword ptr [ecx+0*16+1*4]
        fmul dword ptr [edx+1*16+1*4]
        
        fld  dword ptr [ecx+0*16+2*4]
        fmul dword ptr [edx+2*16+1*4]
        
        fld  dword ptr [ecx+0*16+3*4]
        fmul dword ptr [edx+3*16+1*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+0*16+0*4]
        fstp dword ptr [edi+0*16+1*4]

///
        fld  dword ptr [ecx+0*16+0*4]
        fmul dword ptr [edx+0*16+2*4]

        fld  dword ptr [ecx+0*16+1*4]
        fmul dword ptr [edx+1*16+2*4]

        fld  dword ptr [ecx+0*16+2*4]
        fmul dword ptr [edx+2*16+2*4]

        fld  dword ptr [ecx+0*16+3*4]
        fmul dword ptr [edx+3*16+2*4]
//
        fld  dword ptr [ecx+0*16+0*4]
        fmul dword ptr [edx+0*16+3*4]
  
        fld  dword ptr [ecx+0*16+1*4]
        fmul dword ptr [edx+1*16+3*4]
        
        fld  dword ptr [ecx+0*16+2*4]
        fmul dword ptr [edx+2*16+3*4]
        
        fld  dword ptr [ecx+0*16+3*4]
        fmul dword ptr [edx+3*16+3*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+0*16+2*4]
        fstp dword ptr [edi+0*16+3*4]

 /// seconda riga        
        fld  dword ptr [ecx+1*16+0*4]
        fmul dword ptr [edx+0*16+0*4]

        fld  dword ptr [ecx+1*16+1*4]
        fmul dword ptr [edx+1*16+0*4]

        fld  dword ptr [ecx+1*16+2*4]
        fmul dword ptr [edx+2*16+0*4]

        fld  dword ptr [ecx+1*16+3*4]
        fmul dword ptr [edx+3*16+0*4]
//
        fld  dword ptr [ecx+1*16+0*4]
        fmul dword ptr [edx+0*16+1*4]
  
        fld  dword ptr [ecx+1*16+1*4]
        fmul dword ptr [edx+1*16+1*4]
        
        fld  dword ptr [ecx+1*16+2*4]
        fmul dword ptr [edx+2*16+1*4]
        
        fld  dword ptr [ecx+1*16+3*4]
        fmul dword ptr [edx+3*16+1*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+1*16+0*4]
        fstp dword ptr [edi+1*16+1*4]

///
        fld  dword ptr [ecx+1*16+0*4]
        fmul dword ptr [edx+0*16+2*4]

        fld  dword ptr [ecx+1*16+1*4]
        fmul dword ptr [edx+1*16+2*4]

        fld  dword ptr [ecx+1*16+2*4]
        fmul dword ptr [edx+2*16+2*4]

        fld  dword ptr [ecx+1*16+3*4]
        fmul dword ptr [edx+3*16+2*4]
//
        fld  dword ptr [ecx+1*16+0*4]
        fmul dword ptr [edx+0*16+3*4]
  
        fld  dword ptr [ecx+1*16+1*4]
        fmul dword ptr [edx+1*16+3*4]
        
        fld  dword ptr [ecx+1*16+2*4]
        fmul dword ptr [edx+2*16+3*4]
        
        fld  dword ptr [ecx+1*16+3*4]
        fmul dword ptr [edx+3*16+3*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+1*16+2*4]
        fstp dword ptr [edi+1*16+3*4]
    
/// terza riga        
        fld  dword ptr [ecx+2*16+0*4]
        fmul dword ptr [edx+0*16+0*4]

        fld  dword ptr [ecx+2*16+1*4]
        fmul dword ptr [edx+1*16+0*4]

        fld  dword ptr [ecx+2*16+2*4]
        fmul dword ptr [edx+2*16+0*4]

        fld  dword ptr [ecx+2*16+3*4]
        fmul dword ptr [edx+3*16+0*4]
//
        fld  dword ptr [ecx+2*16+0*4]
        fmul dword ptr [edx+0*16+1*4]
  
        fld  dword ptr [ecx+2*16+1*4]
        fmul dword ptr [edx+1*16+1*4]
        
        fld  dword ptr [ecx+2*16+2*4]
        fmul dword ptr [edx+2*16+1*4]
        
        fld  dword ptr [ecx+2*16+3*4]
        fmul dword ptr [edx+3*16+1*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+2*16+0*4]
        fstp dword ptr [edi+2*16+1*4]

///
        fld  dword ptr [ecx+2*16+0*4]
        fmul dword ptr [edx+0*16+2*4]

        fld  dword ptr [ecx+2*16+1*4]
        fmul dword ptr [edx+1*16+2*4]

        fld  dword ptr [ecx+2*16+2*4]
        fmul dword ptr [edx+2*16+2*4]

        fld  dword ptr [ecx+2*16+3*4]
        fmul dword ptr [edx+3*16+2*4]
//
        fld  dword ptr [ecx+2*16+0*4]
        fmul dword ptr [edx+0*16+3*4]
  
        fld  dword ptr [ecx+2*16+1*4]
        fmul dword ptr [edx+1*16+3*4]
        
        fld  dword ptr [ecx+2*16+2*4]
        fmul dword ptr [edx+2*16+3*4]
        
        fld  dword ptr [ecx+2*16+3*4]
        fmul dword ptr [edx+3*16+3*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+2*16+2*4]
        fstp dword ptr [edi+2*16+3*4]

/// quarta riga        
        fld  dword ptr [ecx+3*16+0*4]
        fmul dword ptr [edx+0*16+0*4]

        fld  dword ptr [ecx+3*16+1*4]
        fmul dword ptr [edx+1*16+0*4]

        fld  dword ptr [ecx+3*16+2*4]
        fmul dword ptr [edx+2*16+0*4]

        fld  dword ptr [ecx+3*16+3*4]
        fmul dword ptr [edx+3*16+0*4]
//
        fld  dword ptr [ecx+3*16+0*4]
        fmul dword ptr [edx+0*16+1*4]
  
        fld  dword ptr [ecx+3*16+1*4]
        fmul dword ptr [edx+1*16+1*4]
        
        fld  dword ptr [ecx+3*16+2*4]
        fmul dword ptr [edx+2*16+1*4]
        
        fld  dword ptr [ecx+3*16+3*4]
        fmul dword ptr [edx+3*16+1*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+3*16+0*4]
        fstp dword ptr [edi+3*16+1*4]

///
        fld  dword ptr [ecx+3*16+0*4]
        fmul dword ptr [edx+0*16+2*4]

        fld  dword ptr [ecx+3*16+1*4]
        fmul dword ptr [edx+1*16+2*4]

        fld  dword ptr [ecx+3*16+2*4]
        fmul dword ptr [edx+2*16+2*4]

        fld  dword ptr [ecx+3*16+3*4]
        fmul dword ptr [edx+3*16+2*4]
//
        fld  dword ptr [ecx+3*16+0*4]
        fmul dword ptr [edx+0*16+3*4]
  
        fld  dword ptr [ecx+3*16+1*4]
        fmul dword ptr [edx+1*16+3*4]
        
        fld  dword ptr [ecx+3*16+2*4]
        fmul dword ptr [edx+2*16+3*4]
        
        fld  dword ptr [ecx+3*16+3*4]
        fmul dword ptr [edx+3*16+3*4]
//
        fxch st(7)
        faddp st(6),st
        faddp st(6),st
        fxch st(3)
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        faddp st(2),st
        //1+2+3+4, 5+6+7+8
        
        fstp dword ptr [edi+3*16+2*4]
        fstp dword ptr [edi+3*16+3*4]

  //      ret
    }
}



__inline void Vect4PerMatNorm_Asm_0(void)
/*
  ESI: (destination vertex)
  BX : numVertex
  ECX: lpVertex
  EDX: lpMatrix 
*/
{
__asm{
      //row 2   ...[3*4,2*16]=> [(colonna-1)*4,(riga-1)*16]
      fld  dword ptr [edx+2*4+2*16]
      fmul dword ptr [ecx+2*4]
      fld  dword ptr [edx+1*4+2*16]          
      fmul dword ptr [ecx+2*4]
      fld  dword ptr [edx+0*4+2*16]
      fmul dword ptr [ecx+2*4]      

      fld  dword ptr [edx+2*4+1*16]
      fmul dword ptr [ecx+1*4]
      fld  dword ptr [edx+1*4+1*16]          
      fmul dword ptr [ecx+1*4]
      fld  dword ptr [edx+0*4+1*16]
      fmul dword ptr [ecx+1*4]
      //OK!!! 012|012
      fxch st(2)
		
		faddp st(5),st
		faddp st(3),st
		faddp st(1),st		
// // ///012
  //OK!!!
      //row 0

      fld  dword ptr [edx+2*4+0*16]
      fmul dword ptr [ecx+0*4]
      fld  dword ptr [edx+1*4+0*16]          
      fmul dword ptr [ecx+0*4]
      fld  dword ptr [edx+0*4+0*16]
      fmul dword ptr [ecx+0*4]

// // /

		fxch st(2)
		faddp st(5),st		
		faddp st(3),st
		faddp st(1),st
		fldz

		fstp dword ptr [esi+3*4]
		fstp dword ptr [esi+0*4]
		fstp dword ptr [esi+1*4]
      	fstp dword ptr [esi+2*4]
// // ///
     }
}

__inline void Vect4PerMatNorm_Asm_1(void)
/*
  ESI: (destination vertex)
  BX : numVertex
  ECX: lpVertex
  EDX: lpMatrix 
*/
{
__asm{
      //row 2   ...[3*4,2*16]=> [(colonna-1)*4,(riga-1)*16]
      fld  dword ptr [edx+2*4+2*16]
      fmul dword ptr [ecx+2*4]
      fld  dword ptr [edx+1*4+2*16]          
      fmul dword ptr [ecx+2*4]
      fld  dword ptr [edx+0*4+2*16]
      fmul dword ptr [ecx+2*4]      

      fld  dword ptr [edx+2*4+1*16]
      fmul dword ptr [ecx+1*4]
      fld  dword ptr [edx+1*4+1*16]          
      fmul dword ptr [ecx+1*4]
      fld  dword ptr [edx+0*4+1*16]
      fmul dword ptr [ecx+1*4]
      //OK!!!
      fxch st(2)
      //row 3      
      fadd dword ptr [edx+2*4+3*16]
      fxch st(1)
      fadd dword ptr [edx+1*4+3*16]
      fxch st(2)
      fadd dword ptr [edx+0*4+3*16]
      //OK!!!  021

// // /	  
		fxch st(1)
		faddp st(5),st
		faddp st(2),st
		faddp st(2),st		
// // ///
  //OK!!!
      //row 0

      fld  dword ptr [edx+2*4+0*16]
      fmul dword ptr [ecx+0*4]
      fld  dword ptr [edx+1*4+0*16]          
      fmul dword ptr [ecx+0*4]
      fld  dword ptr [edx+0*4+0*16]
      fmul dword ptr [ecx+0*4]

// // /

		fxch st(2)
		faddp st(5),st		
		faddp st(3),st
		faddp st(1),st
		fld1

		fstp dword ptr [esi+3*4]
		fstp dword ptr [esi+0*4]
		fstp dword ptr [esi+1*4]
      	fstp dword ptr [esi+2*4]
// // ///
     }
}

