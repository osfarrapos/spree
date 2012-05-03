/*
 * 20000228
 *
 * jconfig.h par Pierre Renaux
 *
 * Fichier de configuration pour les plateforme que supporte le AGL
 *
 */

#define HAVE_PROTOTYPES
#define HAVE_UNSIGNED_CHAR
#define HAVE_UNSIGNED_SHORT
/* #define void char */
/* #define const */
#undef CHAR_IS_UNSIGNED
#define HAVE_STDDEF_H
#define HAVE_STDLIB_H
#undef NEED_BSD_STRINGS
#undef NEED_SYS_TYPES_H
#undef NEED_SHORT_EXTERNAL_NAMES
#undef INCOMPLETE_TYPES_BROKEN

#ifdef JPEG_INTERNALS

#undef RIGHT_SHIFT_IS_UNSIGNED

#endif /* JPEG_INTERNALS */

#ifdef JPEG_CJPEG_DJPEG

#undef BMP_SUPPORTED       /* BMP image file format */
#undef GIF_SUPPORTED       /* GIF image file format */
#undef PPM_SUPPORTED       /* PBMPLUS PPM/PGM image file format */
#undef RLE_SUPPORTED        /* Utah RLE image file format */
#undef TARGA_SUPPORTED     /* Targa image file format */

#undef TWO_FILE_COMMANDLINE /* optional */
#define USE_SETMODE     /* Needed to make one-file style work in DJGPP */
#undef NEED_SIGNAL_CATCHER  /* Define this if you use jmemname.c */
#undef DONT_USE_B_MODE
#undef PROGRESS_REPORT      /* optional */

#endif /* JPEG_CJPEG_DJPEG */
