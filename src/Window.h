//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_WINDOW_H
#define EPSILONEDITOR_WINDOW_H

typedef struct HWND__* HWND;
typedef struct HDC__* HDC;
typedef struct HGLRC__* HGLRC;
namespace Epsilon
{
    class Window
    {

    public:
        Window(int width, int height);
        void ClearFrame();
        void SetClearColor(float r,float g, float b, float a);
        ~Window();
        [[nodiscard]] const HWND GetHandle() const {return handle_;}
        bool WillClose() const {return close_; };
        void SwapBuffers();
    private:
        HWND handle_;
        HDC hDC_;
        HGLRC hGLRC_;
        int dimensions_[2]{0};
        bool close_;
    };
}

#endif //EPSILONEDITOR_WINDOW_H
