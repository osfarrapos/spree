#ifndef _FUNCALLOC_H
#define _FUNCALLOC_H
/* [FILE] : FuncAlloc.h
   SCOPO  : Contiene le macro di allocazione e deallocazione della memoria
   */
/* MACRO :       MyMalloc(p,size)
   SCOPO :       Alloca una quantità di memoria pari a size*sizeof(p)
   				  utilizza la funzione WIN32 API GlobalAlloc
   	Parametri
		p 			ptr a tipo
		size 		quantità di byte desiderata   (size*(sizeof(p))
	flags
		GMEM_FIXED 	    sposta il blocco nella memoria fisica
		GMEM_ZEROINIT   azzera il contenuto della variabile

   ESEMPIO: int *s;
   			s=MyMalloc(typeof s,1024);

   Alloca una quantità di memoria per contenere 1024 interi
*/

extern void *gmalloc(int len);
extern void gfree(void *ptr);

#define GPTR (GMEM_FIXED | GMEM_ZEROINIT)
#define MyMalloc(p,size) (p *)gmalloc(size*sizeof(p))

/* MACRO :       MyFree(p)
   SCOPO :       Dealloca la memoria allocata per il ptr p
   				  utilizza la funzione WIN32 API GlobalAlloc
   	Parametri
		p 			ptr a tipo
		size 		quantità di byte desiderata   (size*(sizeof(p))
	flags
		GMEM_FIXED 	    sposta il blocco nella memoria fisica
		GMEM_ZEROINIT   azzera il contenuto della variabile

   ESEMPIO: int *s;
   			s=MyMalloc(typeof s,1024);

			MyFree(s);

   DeAlloca la memoria allocata precedentemente per contenere 1024 interi

*/

// aegis
#define MyFree(p) gfree(p)

#endif



