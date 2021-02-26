#include "Avi.h"


bool Avi::readStart(std::string file, int& width, int& height)
{

    // PAVIFILE avi;
    int res = AVIFileOpen(&readFile, file.c_str(), OF_READ, NULL);

    if (res != AVIERR_OK)
    {
        //an error occures
        if (readFile != NULL)
            AVIFileRelease(readFile);

        return FALSE;
    }

    // AVIFILEINFO avi_info;
    AVIFileInfo(readFile, &file_info, sizeof(AVIFILEINFO));

    height = static_cast<int>(file_info.dwHeight);
    width = static_cast<int>(file_info.dwWidth);
    m_period = static_cast<float>(file_info.dwScale) / static_cast<float>(file_info.dwRate);

    m_width = width;
    m_height = height;
    // PAVISTREAM pStream;
    res = AVIFileGetStream(readFile, &readStream, streamtypeVIDEO /*video stream*/,
        0 /*first stream*/);

    if (res != AVIERR_OK)
    {
        if (readStream != NULL)
            AVIStreamRelease(readStream);

        AVIFileExit();
        return FALSE;
    }

    iFirstFrame = AVIStreamStart(readStream);
    if (iFirstFrame == -1)
    {
        //Error getteing the frame inside the stream

        if (readStream != NULL)
            AVIStreamRelease(readStream);

        AVIFileExit();
        return FALSE;
    }

    iNumFrames = AVIStreamLength(readStream);
    if (iNumFrames == -1)
    {
        //Error getteing the number of frames inside the stream

        if (readStream != NULL)
            AVIStreamRelease(readStream);

        AVIFileExit();
        return FALSE;
    }
    // add stream info
    AVIStreamInfo(readStream, &stream_info, sizeof(AVISTREAMINFO));

    pFrame = AVIStreamGetFrameOpen(readStream,
        NULL/*(BITMAPINFOHEADER*) AVIGETFRAMEF_BESTDISPLAYFMT*/ /*&bih*/);
    return true;
}

unsigned char* Avi::getNextFrame()
{
    if (frameIndex >= iNumFrames - iFirstFrame)
        return nullptr;

    BYTE* pDIB = (BYTE*)AVIStreamGetFrame(pFrame, frameIndex); // return type DIB
    // ASSERT(pDIB != NULL);

    //Creates a full-color (no palette) DIB from a pointer to a
    //full-color memory DIB

    //get the BitmapInfoHeader
    BITMAPINFOHEADER bih;
    RtlMoveMemory(&bih.biSize, pDIB, sizeof(BITMAPINFOHEADER));

    //now get the bitmap bits
    if (bih.biSizeImage < 1)
    {
        return nullptr;
    }

    BYTE* Bits = new BYTE[bih.biSizeImage];

    RtlMoveMemory(Bits, pDIB + sizeof(BITMAPINFOHEADER), bih.biSizeImage);

    //and BitmapInfo variable-length UDT
    BYTE memBitmapInfo[40];
    RtlMoveMemory(memBitmapInfo, &bih, sizeof(bih));

    ++frameIndex;
    BYTE* p = Bits;
    for (int i = 0; i < m_width * m_height; i++)
    {
        BYTE temp = p[0];
        p[0] = p[2];
        p[2] = temp;
        p += 3;
    }
    return Bits;  // doc should delete this
}
bool Avi::readEnd()
{
    AVIStreamGetFrameClose(pFrame);

    //close the stream after finishing the task
    if (readStream != NULL)
        AVIStreamRelease(readStream);

    

    return TRUE;
}

bool Avi::writeStart(std::string file)
{
    HRESULT error = AVIFileOpen(&writeFile, file.c_str(), OF_CREATE | OF_WRITE, NULL);
    if ( error != AVIERR_OK)
    {
        return false;
    }

    stream_info.dwQuality = 10000;
    stream_info.fccHandler = mmioFOURCC('c', 'v', 'i', 'd');
    stream_info.dwLength = 0;

    if (AVIFileCreateStream(writeFile, &writeStream, &stream_info) != AVIERR_OK)
    {
        return false;
    }

    AVICOMPRESSOPTIONS	m_AviCompressOptions;
    ZeroMemory(&m_AviCompressOptions, sizeof(AVICOMPRESSOPTIONS));
    m_AviCompressOptions.fccType = streamtypeVIDEO;
    m_AviCompressOptions.fccHandler = stream_info.fccHandler;
    m_AviCompressOptions.dwFlags = AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
    m_AviCompressOptions.dwKeyFrameEvery = 1;

    if (AVIMakeCompressedStream(&compressedStream, writeStream, &m_AviCompressOptions, NULL) != AVIERR_OK)
    {
        // One reason this error might occur is if you are using a Codec that is not 
        // available on your system. Check the mmioFOURCC() code you are using and make
        // sure you have that codec installed properly on your machine.
        return 0;
    }

    BITMAPINFO bmpInfo;
    ZeroMemory(&bmpInfo, sizeof(BITMAPINFO));
    bmpInfo.bmiHeader.biPlanes = 1;
    bmpInfo.bmiHeader.biWidth = m_width;
    bmpInfo.bmiHeader.biHeight = m_height;
    bmpInfo.bmiHeader.biCompression = BI_RGB;
    bmpInfo.bmiHeader.biBitCount = 24;
    bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmpInfo.bmiHeader.biSizeImage = bmpInfo.bmiHeader.biWidth * bmpInfo.bmiHeader.biHeight * bmpInfo.bmiHeader.biBitCount / 8;

    if (AVIStreamSetFormat(compressedStream, 0, (LPVOID)&bmpInfo, bmpInfo.bmiHeader.biSize)!= AVIERR_OK)
    {
        // One reason this error might occur is if your bitmap does not meet the Codec requirements.
        // For example, 
        //   your bitmap is 32bpp while the Codec supports only 16 or 24 bpp; Or
        //   your bitmap is 274 * 258 size, while the Codec supports only sizes that are powers of 2; etc...
        // Possible solution to avoid this is: make your bitmap suit the codec requirements,
        // or Choose a codec that is suitable for your bitmap.
        return 0;
    }

    return 1;
    //ZeroMemory(&m_AviCompressOptions, sizeof(AVICOMPRESSOPTIONS));
    //m_AviCompressOptions.fccType = streamtypeVIDEO;
    //m_AviCompressOptions.fccHandler = m_AviStreamInfo.fccHandler;
    //m_AviCompressOptions.dwFlags = AVICOMPRESSF_KEYFRAMES | AVICOMPRESSF_VALID;//|AVICOMPRESSF_DATARATE;
    //m_AviCompressOptions.dwKeyFrameEvery = 1;
}
bool Avi::writeNextFrame(unsigned char* data)
{
    // BITMAPINFO	bmpInfo;

    // bmpInfo.bmiHeader.biBitCount = 0;
    // bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    // 
    // GetDIBits(m_hAviDC, hBitmap, 0, 0, NULL, &bmpInfo, DIB_RGB_COLORS);
    // 
    // bmpInfo.bmiHeader.biCompression = BI_RGB;

    // convert bmp to dib ?

    HRESULT error = AVIStreamWrite(compressedStream, frameIndex + iFirstFrame, 1,
        data, m_width * m_height * 3, 0, NULL, NULL);
    if ( error != AVIERR_OK)
    {

        return false;
    }

    return true;
}
bool Avi::writeEnd()
{
    //close the stream after finishing the task
    if (readStream != NULL)
        AVIStreamRelease(writeStream);

    return TRUE;
}