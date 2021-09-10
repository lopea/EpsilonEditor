//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_WINDOW_H
#define EPSILONEDITOR_WINDOW_H


#ifdef _WIN32
#include <Windows.h>
#endif
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
        [[nodiscard]] bool WillClose() const {return close_; };
        void SwapBuffers();
        [[nodiscard]] double GetDeltaTime() const {return deltaTime_;}
        [[nodiscard]] const int* GetResolution() const { return dimensions_;}
        [[nodiscard]] const int* GetMouse() const {return mouse_;}
        [[nodiscard]] const int* GetPosition() const { return mouse_;}
    private:
        HWND handle_;
        HDC hDC_;
        HGLRC hGLRC_;
        int dimensions_[2]{0};
        int pos_[2]{0};
        int mouse_[4]{0};
        bool close_;
        double deltaTime_;


        std::chrono::time_point<std::chrono::high_resolution_clock> last;

#ifdef _WIN32
        //keep a reference to the opengl library for getting functions
        static inline HMODULE glModule_ = nullptr;
        //give access for the winproc to change values directly
        friend LRESULT CALLBACK WinProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
#endif
    };
}

#endif //EPSILONEDITOR_WINDOW_H
