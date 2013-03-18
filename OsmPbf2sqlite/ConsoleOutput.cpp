#include "StdAfx.h"
#include "ConsoleOutput.h"

CConsoleOutput::CConsoleOutput(void)
{
}

CConsoleOutput::~CConsoleOutput(void)
{
}


// prints a formatted message to stdout, optionally color coded
//static
void CConsoleOutput::msg(const char* format,  va_list args) {

	vfprintf(stdout, format, args);
	fprintf(stdout, "\n");
}

// prints a formatted message to stderr, color coded to red
//static
void CConsoleOutput::Err(const char* format, ...) {
	fprintf(stdout, "ERROR:");
	va_list args;
	va_start(args, format);
	msg(format, args);
	va_end(args);
	ASSERT(FALSE);
	exit(1);
}

// prints a formatted message to stderr, color coded to yellow
//static
void CConsoleOutput::Warn(const char* format, ...) {
	fprintf(stdout, "Warning:");
	va_list args;
	va_start(args, format);
	msg(format, args);
	va_end(args);
}

// prints a formatted message to stderr, color coded to green
//static
void CConsoleOutput::Info(const char* format, ...) {
	va_list args;
	va_start(args, format);
	msg(format, args);
	va_end(args);
}

// prints a formatted message to stderr, color coded to white
//static
void CConsoleOutput::Debug(const char* format, ...) {
	fprintf(stdout, "+++:");
	va_list args;
	va_start(args, format);
	msg(format, args);
	va_end(args);
}
