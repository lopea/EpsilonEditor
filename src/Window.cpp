//
// Created by Javier on 7/24/2021.
//

#include "Window.h"


#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
#include <stdexcept>
#include "ImGuiEnvironment.h"
#include <glad/gl.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
extern IMGUI_IMPL_API void     ImGui_ImplWin32_EnableDpiAwareness();

PIXELFORMATDESCRIPTOR pfd

    {
        sizeof(PIXELFORMATDESCRIPTOR),1,
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,PFD_TYPE_RGBA,32,0,
        0,0,0,0,0,8,0,0,
        0,0,0,0,24,8,0,
        PFD_MAIN_PLANE,0, 0,0,0
    };
namespace Epsilon
{
    void* DumbProcAddress(const char* location)
    {
      void* res = wglGetProcAddress(location);
      if(!res || res == (void*)0x1 || res == (void*)0x2 || res == (void*)0x3 || res == (void*)-1)
      {
        static HMODULE module = LoadLibraryA("opengl32.dll");
        res = (void*) GetProcAddress(module,location);
      }
      return res;
    }

    LRESULT CALLBACK WinProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
    {
      if(ImGuiEnvironment::IsInitialized() && ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam))
        return true;
      if(umsg == WM_CLOSE || umsg == WM_DESTROY )
      {
        PostQuitMessage(0);
        return 0;
      }
      return DefWindowProc(hwnd, umsg, wparam, lparam);
    }

    Window::Window(int width, int height)
    {
      ImGui_ImplWin32_EnableDpiAwareness();
      //create window class
      WNDCLASS windowClass;

      //clear memory
      ZeroMemory(&windowClass, sizeof(WNDCLASS));

      //set basic values for the new window
      windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
      windowClass.hInstance = nullptr;
      windowClass.lpszClassName = "--lopea--";
      windowClass.lpfnWndProc = WinProc;

      //set the resolution of the window
      RECT rec {0,0,width,height};

      //set the look of the window (non fullscreen)
      DWORD dwStyle = WS_VISIBLE | WS_CAPTION | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_SYSMENU;

      //apply the resolution for the next window
      AdjustWindowRect(&rec, dwStyle, 0);

      //register the window class and check if it worked
      if(!RegisterClass(&windowClass))
        exit(0xF00BA12);


      //create window
      handle_ = CreateWindowEx
          (
            WS_EX_WINDOWEDGE | WS_EX_APPWINDOW,
            windowClass.lpszClassName,
            "Epsilon Shader Editor",
            dwStyle,
            (GetSystemMetrics(SM_CXSCREEN) - rec.right + rec.left)/2,
            (GetSystemMetrics(SM_CYSCREEN) - rec.bottom + rec.top)/2,
            rec.right-rec.left,rec.bottom-rec.top, 0,0,0, 0
          );

      if(!handle_)
        exit(0xF00BA12);

      //set the format for opengl
      hDC_ = GetDC(handle_);
      if(!hDC_)
        exit(0xF00BA12);

      if(!SetPixelFormat(hDC_, ChoosePixelFormat(hDC_, &pfd), &pfd))
        exit(0xF00BA12);


      //create an opengl context
      hGLRC_  = wglCreateContext(hDC_);
      if(!hGLRC_)
        exit(0xF00BA12);

      //make the context the current context
      if(!wglMakeCurrent(hDC_, hGLRC_))
        exit(0xF00BA12);

      //initalize glad
      if(!gladLoadGL(reinterpret_cast<GLADloadfunc>(DumbProcAddress)))
        exit(0xF00BA12);



      //initialize variables
      close_ = false;
      dimensions_[0] = width;
      dimensions_[1] = height;
      glViewport(0,0, dimensions_[0], dimensions_[1]);

    }


    void Window::SwapBuffers()
    {
        MSG msg;
        while(PeekMessage(&msg, 0,0,0, PM_REMOVE))
        {
          if(msg.message == WM_QUIT) close_ = true;
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      ::SwapBuffers(hDC_);
    }

    Window::~Window()
    {
      wglMakeCurrent(0,0);
      wglDeleteContext(hGLRC_);
      DeleteDC(hDC_);
      DestroyWindow(handle_);
    }

    void Window::ClearFrame()
    {
      glViewport(0,0, dimensions_[0], dimensions_[1]);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::SetClearColor(float r, float g, float b, float a)
    {
      glClearColor(r,g,b,a);

    }
}