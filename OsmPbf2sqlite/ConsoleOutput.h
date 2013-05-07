#pragma once

class CConsoleOutput
{
public:
	CConsoleOutput(void);
	~CConsoleOutput(void);


	// prints a formatted message to stderr, color coded to red
static 	void Err(const char* format, ...) ;

	// prints a formatted message to stderr, color coded to yellow
static 		void Warn(const char* format, ...) ;

	// prints a formatted message to stderr, color coded to green
static 		void Info(const char* format, ...);


	// prints a formatted message to stderr, color coded to white
static 		void Debug(const char* format, ...);

private:
	static void msg(const char* format,  va_list args);


};



#define err CConsoleOutput::Err 
#define warn CConsoleOutput::Warn 
#define info CConsoleOutput::Info 
#define debug CConsoleOutput::Debug
