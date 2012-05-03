#ifndef MYASSERT_H
#define MYASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

void __cdecl _myassert(void *, void *, unsigned,...);

#ifdef __cplusplus
}
#endif

#define myassert(exp,p,r) (void)( (exp) || (_myassert(#exp, __FILE__, __LINE__,p,r), 0) )

#define ASSERT(exp, arg, msg)

#endif