
Static linking QHTM
-------------------
QHTM builds all it's libraries and DLLs into the /projects/bin/ folder.



Link to the following libraries:
* QHTMs.lib
* ZLib.lib
* lpng103.lib
* jpeglib.lib
* ImgLib.lib
* DebugHlp.lib
* DataSource.lib
* guitools.lib

Most of the above libraries have a debug version. This is shown by a "D" in their names.
In Debug builds you should link to these:
* QHTMsD.lib
* ZLibD.lib
* lpng103D.lib
* jpeglibD.lib
* ImgLibD.lib
* DebugHlp.lib
* DataSourceD.lib
* guitoolsD.lib

DebugHlp builds and runsin both debug and release with both debug and release. I use the release
version even in debug builds...except when I am debugging DebugHlp of course.


Unicode
-------
I haven't supplied a unicode build for static libraries but it should just be a matter
of adding the preprocessor directives for unicode.