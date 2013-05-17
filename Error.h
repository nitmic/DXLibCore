#pragma once
#include <tchar.h>
#include <Windows.h>
class Error{
public:
	static void printf(TCHAR *msg, long line, char *file){
		TCHAR tFile[256];
		MultiByteToWideChar(CP_ACP, 0, file, -1, tFile, 256);
		TCHAR tOutput[256];
		_swprintf(tOutput, __T("file = %s line = %d msg = %s"), tFile, line, msg);
		MessageBox(NULL, tOutput, __T("Error"), MB_OK);
	}
	Error(){};
	~Error(){};
};

#define ERROR_MSG(msg)	Error::printf(msg, __LINE__, __FILE__)