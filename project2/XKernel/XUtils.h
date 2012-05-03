#ifndef __UTILS_H__
#define __UTILS_H__
#ifdef __cplusplus

#include "xbasedefs.h"

// Helper function to stuff a FLOAT into a DWORD argument
extern DWORD FtoDW( FLOAT f );
extern void _XPlaySoundEffect( int index, D3DXVECTOR3 position, bool checkplay = false, bool loop = false );
extern void _XPlayEnvSoundEffect( int index, D3DXVECTOR3 position, bool checkplay = false, bool loop = false );
extern void _XPlayInterfaceSound( int index, bool checkplay = false );

#endif // __cplusplus
#endif // __UTILS_H__
