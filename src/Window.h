//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_WINDOW_H
#define EPSILONEDITOR_WINDOW_H

typedef struct HWND__* HWND;
typedef struct HDC__* HDC;
typedef struct HGLRC__* HGLRC;
#include <chrono>
namespace Epsilon
{
    class Window
    {

    public:

        static void* DumbProcAddress(const char* location);
        Window(int width, int height);
        void ClearFrame();
        void SetClearColor(float r,float g, float b, float a);
        ~Window();
        [[nodiscard]] const HWND GetHandle() const {return handle_;}
        bool WillClose() const {return close_; };
        void SwapBuffers();
        double GetCurrentTime() const {return currentTime;}
    private:
        HWND handle_;
        HDC hDC_;
        HGLRC hGLRC_;
        int dimensions_[2]{0};
        bool close_;
        double currentTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> last;

        //give access for the winproc to change values directly
        friend __int64 __stdcall WinProc(HWND hwnd, unsigned int umsg, unsigned __int64 wparam, __int64 lparam);
    };
}

#endif //EPSILONEDITOR_WINDOW_H
