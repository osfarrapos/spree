#ifndef XFUNCTIONSTACKTRACE_H
#define XFUNCTIONSTACKTRACE_H

#include <malloc.h>
#define _XFUNCTION_STACK_TRACE(x) \
	char * function_stack_trace_buffer = (char *) _alloca(8); \
	CopyMemory(function_stack_trace_buffer, x, 8);

#endif //XFUNCTIONSTACKTRACE_H
