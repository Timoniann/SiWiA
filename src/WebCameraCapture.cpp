#include "WebCameraCapture.h"


void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi, HBITMAP hBMP, HDC hDC)
{
    HANDLE hf;                  // file handle
    BITMAPFILEHEADER hdr;       // bitmap file-header
    PBITMAPINFOHEADER pbih;     // bitmap info-header
    LPBYTE lpBits;              // memory pointer
    DWORD dwTotal;              // total count of bytes
    DWORD cb;                   // incremental count of bytes
    BYTE *hp;                   // byte pointer
    DWORD dwTmp;

    pbih = (PBITMAPINFOHEADER) pbi;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    if (!lpBits)
    {
        MessageBox(hwnd,"GlobalAlloc","Error", MB_OK );
    }
// Retrieve the color table (RGBQUAD array) and the bits
// (array of palette indices) from the DIB.
    if (!GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbi,DIB_RGB_COLORS))
    {
        MessageBox(hwnd,"GetDIBits","Error",MB_OK );
    }
// Create the .BMP file.
    hf = CreateFile(pszFile,GENERIC_READ | GENERIC_WRITE,(DWORD) 0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,(HANDLE) NULL);
    if (hf == INVALID_HANDLE_VALUE)
    {
        MessageBox( hwnd,"CreateFile","Error", MB_OK);
    }

    hdr.bfType = 0x4d42;  // File type designator "BM" 0x42 = "B" 0x4d = "M"
// Compute the size of the entire file.
    hdr.bfSize = (DWORD) (sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
    hdr.bfReserved1 = 0;
    hdr.bfReserved2 = 0;
// Compute the offset to the array of color indices.
    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof (RGBQUAD);
// Copy the BITMAPFILEHEADER into the .BMP file.
    if (!WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL) )
    {
        MessageBox(hwnd,"WriteFileHeader","Error",MB_OK );
    }
// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
    if (!WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD), (LPDWORD) &dwTmp, NULL))
    {
        MessageBox(hwnd,"WriteInfoHeader","Error",MB_OK );
    }
// Copy the array of color indices into the .BMP file.
    dwTotal = cb = pbih->biSizeImage;
    hp = lpBits;
    if (!WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL))
    {
        MessageBox(hwnd,"WriteFile","Error",MB_OK );
    }
// Close the .BMP file.
    if (!CloseHandle(hf))
    {
        MessageBox(hwnd,"CloseHandle","Error",MB_OK );
    }

// Free memory.
    GlobalFree((HGLOBAL)lpBits);
}

PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd, HBITMAP hBmp)
{
    BITMAP bmp;
    PBITMAPINFO pbmi;
    WORD cClrBits;
// Retrieve the bitmap color format, width, and height.
    if (!GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp))
    {
        MessageBox(hwnd,"GetObject","Error",MB_OK );
    }
// Convert the color format to a count of bits.
    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
    if (cClrBits == 1)
        cClrBits = 1;
    else if (cClrBits <= 4)
        cClrBits = 4;
    else if (cClrBits <= 8)
        cClrBits = 8;
    else if (cClrBits <= 16)
        cClrBits = 16;
    else if (cClrBits <= 24)
        cClrBits = 24;
    else cClrBits = 32;

// Allocate memory for the BITMAPINFO structure. (This structure
// contains a BITMAPINFOHEADER structure and an array of RGBQUAD
// data structures.)

    if (cClrBits != 24)
    {
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR,sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits));
    }
// There is no RGBQUAD array for the 24-bit-per-pixel format.
    else
        pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

// Initialize the fields in the BITMAPINFO structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biWidth = bmp.bmWidth;
    pbmi->bmiHeader.biHeight = bmp.bmHeight;
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
    if (cClrBits < 24)
    {
        pbmi->bmiHeader.biClrUsed = (1<<cClrBits);
    }
// If the bitmap is not compressed, set the BI_RGB flag.
    pbmi->bmiHeader.biCompression = BI_RGB;

// Compute the number of bytes in the array of color
// indices and store the result in biSizeImage.
// For Windows NT, the width must be DWORD aligned unless
// the bitmap is RLE compressed. This example shows this.
// For Windows 95/98/Me, the width must be WORD aligned unless the
// bitmap is RLE compressed.
    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8 * pbmi->bmiHeader.biHeight;
// Set biClrImportant to 0, indicating that all of the
// device colors are important.
    pbmi->bmiHeader.biClrImportant = 0;

    return pbmi; //return BITMAPINFO
}

HWND GetMainHandle()
{
    return GetTopWindow(GetDesktopWindow());
}


WebCameraCapture::WebCameraCapture(int x, int y, int width, int height) :
    Element("", x, y, width, height)
{

}

void WebCameraCapture::Init(HWND parent)
{
    handle = capCreateCaptureWindow(initializer.title, WS_CHILD, initializer.x, initializer.y, initializer.width, initializer.height, parent, (int)hMenu);
    this->parent = parent;
}

WebCameraCapture::~WebCameraCapture()
{
    //dtor
}

void WebCameraCapture::openDialogToSelectDriver()
{
    SendMessage(handle,WM_CAP_DLG_VIDEOSOURCE,0,0);
}

void WebCameraCapture::Show()
{
    Start();
}

void WebCameraCapture::Start()
{
    ShowWindow(handle, SW_SHOW);
    SendMessage(handle, WM_CAP_DRIVER_CONNECT, 0, 0);
    SendMessage(handle, WM_CAP_SET_SCALE, true , 0);
    SendMessage(handle, WM_CAP_SET_PREVIEWRATE, 33, 0);
    SendMessage(handle, WM_CAP_SET_PREVIEW, true , 0);
}

void WebCameraCapture::Disconnect()
{
    ShowWindow(handle, SW_HIDE);
    SendMessage(handle, WM_CAP_DRIVER_DISCONNECT, 0, 0);
}

void WebCameraCapture::GrabToClipboard()
{
    SendMessage(handle, WM_CAP_GRAB_FRAME, 0, 0);
    SendMessage(handle, WM_CAP_EDIT_COPY, 0, 0);
}


HWND WebCameraCapture::GetHandle()
{
    return handle;
}

void WebCameraCapture::SaveToFile(LPTSTR filePath)
{
    PAINTSTRUCT ps;
    RECT rc;
    HBITMAP hbm;


    GrabToClipboard();

    HDC hdc = CreateCompatibleDC(BeginPaint(handle, &ps));
    if (hdc != NULL)
    {
        if (OpenClipboard(handle))
        {
            hbm = (HBITMAP) GetClipboardData(CF_BITMAP);
            SelectObject(hdc, hbm);
            GetClientRect(handle, &rc);
            CloseClipboard();
        }
    }

    PBITMAPINFO pbi = CreateBitmapInfoStruct(parent, hbm);
    CreateBMPFile(parent, filePath, pbi, hbm, hdc);

    Start();
}

void WebCameraCapture::StartRecordingVideo()
{
    SendMessage(handle, WM_CAP_SEQUENCE, 0, 0);
}

void WebCameraCapture::SaveVideoToFile(char * fileName)
{
    SendMessage(handle, WM_CAP_FILE_SAVEAS, 0, (LPARAM)fileName);
}

bool aborted = false;

void WebCameraCapture::Abort()
{
    aborted = true;
    SendMessage(handle, WM_CAP_ABORT, 0, 0);//WM_CAP_ABORT
}

void WebCameraCapture::Stop()
{
    aborted = true;
    SendMessage(handle, WM_CAP_STOP, 0, 0);//WM_CAP_ABORT
}

void WebCameraCapture::ShowDialogDecompressing()
{
    SendMessage(handle, WM_CAP_DLG_VIDEOCOMPRESSION, 0, 0);
}

void WebCameraCapture::ShowDialogVideoFormat()
{
    SendMessage(handle, WM_CAP_DLG_VIDEOFORMAT, 0, 0);
}

void WebCameraCapture::ShowDialogVideoSource()
{
    SendMessage(handle, WM_CAP_DLG_VIDEOSOURCE, 0, 0);
}

CAPDRIVERCAPS WebCameraCapture::GetCaps()
{
    CAPDRIVERCAPS CapDrvCaps;
    SendMessage (handle, WM_CAP_DRIVER_GET_CAPS, sizeof (CAPDRIVERCAPS), (LONG) (LPVOID) &CapDrvCaps);
    return CapDrvCaps;
}

char * WebCameraCapture::GetDeviceName()
{
    char * name;
    SendMessage(handle, WM_CAP_DRIVER_GET_NAME, 80, (LPARAM) (LPVOID) (LPSTR) name);
    return name;
}


char * WebCameraCapture::GetDeviceVersion()
{
    char * ver;
    SendMessage(handle, WM_CAP_DRIVER_GET_VERSION, 80, (LPARAM) (LPVOID) (LPSTR) ver);
    return ver;
}

void WebCameraCapture::AllocateFile(int size)
{
    SendMessage(handle, WM_CAP_FILE_ALLOCATE, 0, size);
}

char * WebCameraCapture::GetCaptureFile()
{
    char * file;
    SendMessage(handle, WM_CAP_FILE_GET_CAPTURE_FILE, /*Buffer size*/ 256, (LPARAM) (LPVOID) (LPSTR) file);
    return file;
}

void WebCameraCapture::SaveVideoToDIBFile(char * fileName)
{
    SendMessage(handle, WM_CAP_FILE_SAVEDIB, 0, (LPARAM)fileName);
}

void WebCameraCapture::SetCaptureFile(char * fileName)
{
    SendMessage(handle, WM_CAP_FILE_SET_CAPTURE_FILE, 0, (LPARAM) fileName);
}

void WebCameraCapture::StartStreamWithoutSaving()
{
    SetupSequence();
    SendMessage(handle, WM_CAP_SEQUENCE_NOFILE, 0, 0);
}

void WebCameraCapture::SetVideoStreamCallback(LRESULT CALLBACK proc(HWND hWnd, LPVIDEOHDR lpVHdr))
{
    SendMessage(handle, WM_CAP_SET_CALLBACK_VIDEOSTREAM, 0, (LPARAM)proc);
}

void WebCameraCapture::SetAudioStreamCallback(LRESULT CALLBACK proc(HWND hWnd, LPWAVEHDR lpWHdr))
{
    SendMessage(handle, WM_CAP_SET_CALLBACK_WAVESTREAM, 0, (LPARAM)proc);
}

LRESULT CALLBACK capControlCallback(HWND hWnd, int  nState)
{
    std::cout << nState << " ";
    if(aborted) {
        cout << "ASDSDA";
        return (LRESULT) FALSE;
    }
    return (LRESULT) TRUE;
}

LRESULT CALLBACK capErrorCallback(HWND hWnd,int nID,LPCTSTR lpsz)
{
    cout << "\nError: [nId: " << nID << ", msg: " << lpsz << "]\n";
}

LRESULT CALLBACK capStatusCallback(HWND hWnd, int nID, LPCTSTR lpsz)
{
    cout << "\nMessage: [nId: " << nID << ", msg: " << lpsz << "]\n";
}

void WebCameraCapture::SetCaptureControlCallback()
{
    SendMessage(handle, WM_CAP_SET_CALLBACK_CAPCONTROL, 0, (LPARAM)capControlCallback);
    SendMessage(handle, WM_CAP_SET_CALLBACK_ERROR, 0, (LPARAM)capErrorCallback);
    SendMessage(handle, WM_CAP_SET_CALLBACK_STATUS, 0, (LPARAM)capStatusCallback);
}

int clip(int n) {
    return n < 255 ? n : 255;
}

void WebCameraCapture::SetupSequence()
{
    CAPTUREPARMS cp;
    SendMessage(handle, WM_CAP_GET_SEQUENCE_SETUP, sizeof(CAPTUREPARMS), (LPARAM)&cp);
    cp.fYield = TRUE;
    cp.fAbortLeftMouse = FALSE;
    cp.fAbortRightMouse = FALSE;
    cp.fLimitEnabled = FALSE;
    cp.vKeyAbort = VK_SPACE;
    //cp.fCaptureAudio = FALSE;
    MessageBox(handle, to_string((int)cp.wNumVideoRequested).c_str(), "VideoRequests", 0);
    SendMessage(handle, WM_CAP_SET_SEQUENCE_SETUP, sizeof(CAPTUREPARMS), (LPARAM)&cp);
}


BOOL CanRead(LPVOID p)
{
    MEMORY_BASIC_INFORMATION mbi;
    mbi.Protect = 0;
    ::VirtualQuery(((LPCSTR)p), &mbi, sizeof(mbi));
    return ((mbi.Protect & 0xE6) != 0 && (mbi.Protect & PAGE_GUARD) == 0);
}

HBITMAP WebCameraCapture::ConvertYUY2ToBITMAP(BITMAPINFO info, BYTE * YUY2Data)
{
    if(aborted) {
        MessageBox(NULL, "Aborted", "Error at converting YUY2 to Bitmap", 0);
        ExitThread(0);
        return NULL;
    }
    COLORREF * arr = (COLORREF*) calloc(info.bmiHeader.biWidth * info.bmiHeader.biHeight, sizeof(COLORREF));

    for (int i = 0;  i < info.bmiHeader.biWidth * info.bmiHeader.biHeight / 2;  ++i)
    {
        int y0 = YUY2Data[0];
        int u0 = YUY2Data[1];
        int y1 = YUY2Data[2];
        int v0 = YUY2Data[3];

        YUY2Data += 4;

        int c = y0 - 16;
        int d = u0 - 128;
        int e = v0 - 128;

        unsigned char b = clip(( 298 * c + 516 * d + 128) >> 8); // blue
        unsigned char g = clip(( 298 * c - 100 * d - 208 * e + 128) >> 8); // green
        unsigned char r = clip(( 298 * c + 409 * e + 128) >> 8); // red

        arr[i * 2] = RGB(b, g, r);

        c = y1 - 16;
        b = clip(( 298 * c + 516 * d + 128) >> 8); // blue
        g = clip(( 298 * c - 100 * d - 208 * e + 128) >> 8); // green
        r = clip(( 298 * c + 409 * e + 128) >> 8); // red

        arr[i * 2 + 1] = RGB(b, g, r);
    }
    HBITMAP map = CreateBitmap(info.bmiHeader.biWidth, info.bmiHeader.biHeight, 1, 8*4, (PCVOID) arr);

    if(map == NULL) MessageBox(NULL, "HBITMAP is null", "Info", 0);
    free(arr);
    arr = nullptr;
    return map;
}

void WebCameraCapture::DrawYUY2(BITMAPINFO info, BYTE * YUY2Data, HDC device)
{
    BITMAPINFOHEADER sbit = info.bmiHeader;

    POINT devicePosition = {0, 0};

    HBITMAP hBM = ConvertYUY2ToBITMAP(info, YUY2Data);
    //HBITMAP hBM = CreateDIBitmap(device, &sbit, 100, YUY2Data, &info, DIB_RGB_COLORS);
    if(hBM == NULL) return;
    HDC src = CreateCompatibleDC(device);

    SelectObject(src, hBM);

    bool res = BitBlt(device, // Destination
           devicePosition.x,  // x and
           devicePosition.y,  // y - upper-left corner of place, where we'd like to copy
           info.bmiHeader.biWidth, // width of the region
           info.bmiHeader.biHeight, // height
           src, // source
           0,   // x and
           0,   // y of upper left corner  of part of the source, from where we'd like to copy
           SRCCOPY);

    DeleteObject(hBM);
    DeleteDC(src);
}

HWAVEOUT WebCameraCapture::OpenAudio()
{
    HWAVEOUT hWaveOut;
    WAVEFORMATEX wfx;

    SendMessage(handle, WM_CAP_GET_AUDIOFORMAT, sizeof(WAVEFORMATEX), (LPARAM) (LPVOID) (LPWAVEFORMATEX)&wfx);
    //MessageBox(NULL, "H", "HWND", 0);
    /*MessageBox(NULL, ((string)"Channels: " +
                                to_string(wfx.nChannels) +
                               ", Bytes per second: " +
                               to_string(wfx.nAvgBytesPerSec) +
                               ", Format tag: " +
                                to_string((int)wfx.wFormatTag)).c_str()
                               , "WaveFormatEx", 0);*/

    if(waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL) != MMSYSERR_NOERROR)
        MessageBox(NULL, "ERROR: Unable to open WAVE_MAPPER device\n", "Error", 0);

    return hWaveOut;
}


void WebCameraCapture::SetAudioFormat()
{
    WAVEFORMATEX wfx;
    SendMessage(handle, WM_CAP_SET_AUDIOFORMAT, sizeof(WAVEFORMATEX), (LPARAM)&wfx);
}

WAVEFORMATEX WebCameraCapture::GetAudioFormat()
{
    WAVEFORMATEX wfx;
    SendMessage(handle, WM_CAP_GET_AUDIOFORMAT, sizeof(WAVEFORMATEX), (LPARAM)&wfx);
    return wfx;
}

void WebCameraCapture::SetOverlay(bool set)
{
    SendMessage(handle, WM_CAP_SET_OVERLAY, (WPARAM)set, 0);
}

