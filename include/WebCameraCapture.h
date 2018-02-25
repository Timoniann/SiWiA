#ifndef WEBCAMERACAPTURE_H
#define WEBCAMERACAPTURE_H

#include <Element.h>
#include <vfw.h>

class WebCameraCapture : public Element
{
    public:
        WebCameraCapture(int x, int y, int width, int height);
        virtual ~WebCameraCapture();

        void Init(HWND parent = HWND_DESKTOP) override;
        void Show() override;

        void openDialogToSelectDriver();
        void Start();
        void Disconnect();
        void GrabToClipboard();
        HWND GetHandle();
        void SaveToFile(LPTSTR pszFile);
        void StartRecordingVideo();
        void SaveVideoToFile(char * fileName);
        void Abort();
        void ShowDialogDecompressing();
        void ShowDialogVideoFormat();
        void ShowDialogVideoSource();
        CAPDRIVERCAPS GetCaps();
        char * GetDeviceName();
        char * GetDeviceVersion();
        void AllocateFile(int size);
        char * GetCaptureFile();
        void SaveVideoToDIBFile(char * fileName);
        void SetCaptureFile(char * fileName);
        void SetVideoStreamCallback(LRESULT CALLBACK proc(HWND hWnd, LPVIDEOHDR lpVHdr));
        void SetAudioStreamCallback(LRESULT CALLBACK proc(HWND hWnd, LPWAVEHDR lpWHdr));
        HWAVEOUT OpenAudio();
        void StartStreamWithoutSaving();
        WAVEFORMATEX GetAudioFormat();
        void SetAudioFormat();
        void SetupSequence();
        void Stop();
        void SetCaptureControlCallback();
        void SetOverlay(bool set);


        static HBITMAP ConvertYUY2ToBITMAP(BITMAPINFO info, BYTE * YUY2Data);
        static void DrawYUY2(BITMAPINFO info, BYTE * YUY2Data, HDC device);

    protected:
    private:
};

#endif // WEBCAMERACAPTURE_H
