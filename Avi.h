// references: 
//		https://docs.microsoft.com/en-us/windows/win32/api/vfw/
//		https://gk.palem.in/createmovie.html
//		https://www.codeproject.com/Articles/8739/Extracting-AVI-Frames

#pragma once

#include<Windows.h>
#include <Vfw.h>
#include <string>

class Avi
{
public:
	Avi(){ AVIFileInit(); }
	~Avi(){ AVIFileExit(); }
	bool readStart(std::string file, int& width, int& height);
	bool readEnd();
	unsigned char* getNextFrame();
	bool writeStart(std::string file);
	bool writeNextFrame(unsigned char* bmp);
	bool writeEnd();
	float getPeriod() { return m_period; }

private:
	int iNumFrames;
	int iFirstFrame;
	int m_width;
	int m_height;
	float m_period;
	PAVIFILE readFile;
	PAVIFILE writeFile;
	PAVISTREAM readStream;
	PAVISTREAM writeStream;
	PAVISTREAM	compressedStream;
	PGETFRAME pFrame;
	AVIFILEINFO file_info;
	AVISTREAMINFO stream_info;
	int frameIndex{ 0 };
};