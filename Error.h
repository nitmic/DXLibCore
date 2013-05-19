#pragma once
#include <tchar.h>
#include <Windows.h>
class Error{
public:
	enum{
		MAXSIZE = 256
	};
	static void printf(TCHAR *msg, long line, char *file){
		TCHAR tFile[MAXSIZE];
		MultiByteToWideChar(CP_ACP, 0, file, -1, tFile, MAXSIZE);
		TCHAR tOutput[MAXSIZE];
		swprintf_s(tOutput, __T("file = %s line = %d msg = %s"), tFile, line, msg);
		MessageBox(NULL, tOutput, __T("Error"), MB_OK);
	}
	Error(){};
	~Error(){};
};

#define ERROR_MSG(msg)	Error::printf(msg, __LINE__, __FILE__)