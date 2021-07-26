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
#include <gl/wglext.h>

#include <iostream>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

extern IMGUI_IMPL_API void ImGui_ImplWin32_EnableDpiAwareness();

int attribs[] = {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
    WGL_CONTEXT_MINOR_VERSION_ARB, 6,
    WGL_CONTEXT_PROFILE_MASK_ARB,
    WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
    0, 0
};

PIXELFORMATDESCRIPTOR pfd

    {
        sizeof(PIXELFORMATDESCRIPTOR), 1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0,
        0, 0, 0, 0, 0, 8, 0, 0,
        0, 0, 0, 0, 24, 8, 0,
        PFD_MAIN_PLANE, 0, 0, 0, 0
    };
namespace Epsilon
{
    void *Window::DumbProcAddress(const char *location)
    {
      void *res = wglGetProcAddress(location);
      if (!res || res == (void *) 0x1 || res == (void *) 0x2 || res == (void *) 0x3 || res == (void *) -1)
      {
        static HMODULE module = LoadLibraryA("opengl32.dll");
        res = (void *) GetProcAddress(module, location);
      }
      return res;
    }

    LRESULT CALLBACK WinProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
    {
      //get reference to the window based on the handle
      Window *windowHandle = (Window *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

      //check if it exists,
      if (windowHandle)
      {
        //give imgui any input that is necessary
        if (ImGuiEnvironment::IsInitialized() && ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam))
          return true;

        switch (umsg)
        {
            //check if the window will close (either through the exit button or the task mgr)
          case WM_CLOSE:
          case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
            //check if the window size has changed
          case WM_SIZE:
          {
            //store the new window size
            RECT rec{};
            //get the new dimensions of the window
            GetWindowRect(hwnd, &rec);
            windowHandle->dimensions_[0] = rec.right - rec.left;
            windowHandle->dimensions_[1] = rec.bottom - rec.top;
            return true;
          }
            //get left mouse click
          case WM_LBUTTONDOWN:
            windowHandle->mouse_[2] = true;
            break;
            //get left mouse release
          case WM_LBUTTONUP:
            windowHandle->mouse_[2] = false;
            //get right mouse click
          case WM_RBUTTONDOWN:
            windowHandle->mouse_[3] = true;
            break;
            //get left mouse release
          case WM_RBUTTONUP:
            windowHandle->mouse_[3] = false;
            //get window position on change
          case WM_WINDOWPOSCHANGED:
          {
            //get window values
            WINDOWPOS* pos = (WINDOWPOS*)lparam;

            //set position
            windowHandle->pos_[0] = pos->x;
            windowHandle->pos_[1] = pos->y;

          }

        }
      }
      //if nothing else return the normal messages
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
      RECT rec{0, 0, width, height};

      //set the look of the window (non fullscreen)
      DWORD dwStyle = WS_VISIBLE | WS_OVERLAPPEDWINDOW;

      //apply the resolution for the next window
      AdjustWindowRect(&rec, dwStyle, 0);

      //register the window class and check if it worked
      if (!RegisterClass(&windowClass))
        exit(0xF00BA12);


      //create window
      handle_ = CreateWindowEx
          (
              0,
              windowClass.lpszClassName,
              "Epsilon Shader Editor",
              dwStyle,
              (GetSystemMetrics(SM_CXSCREEN) - rec.right + rec.left) / 2,
              (GetSystemMetrics(SM_CYSCREEN) - rec.bottom + rec.top) / 2,
              rec.right - rec.left, rec.bottom - rec.top, 0, 0, 0, 0
          );

      if (!handle_)
        exit(0xF00BA12);

      //set the format for opengl
      hDC_ = GetDC(handle_);
      if (!hDC_)
        exit(0xF00BA12);

      if (!SetPixelFormat(hDC_, ChoosePixelFormat(hDC_, &pfd), &pfd))
        exit(0xF00BA12);

      auto temp = wglCreateContext(hDC_);
      wglMakeCurrent(hDC_, temp);
      //create as core context
      PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB;

      wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC) wglGetProcAddress("wglCreateContextAttribsARB");;


      //create an opengl context
      hGLRC_ = wglCreateContextAttribsARB(hDC_, 0, attribs);
      if (!hGLRC_)
        exit(0xF00BA12);

      //make the context the current context
      if (!wglMakeCurrent(hDC_, hGLRC_))
        exit(0xF00BA12);

      //initalize glad
      if (!gladLoadGL(reinterpret_cast<GLADloadfunc>(DumbProcAddress)))
        exit(0xF00BA12);



      //store reference to this class instance in the handle
      SetWindowLongPtr(handle_, GWLP_USERDATA, (LONG_PTR) this);

      //initialize variables
      close_ = false;
      dimensions_[0] = width;
      dimensions_[1] = height;
      currentTime = 0;
      last = std::chrono::high_resolution_clock::now();

      //get position of the window
      pos_[0] = (GetSystemMetrics(SM_CXSCREEN) - rec.right + rec.left) / 2;
      pos_[0] = (GetSystemMetrics(SM_CYSCREEN) - rec.bottom + rec.top) / 2;

      //set the dimensions for openGL
      glViewport(0, 0, dimensions_[0], dimensions_[1]);

      const unsigned char *glversion;
      glversion = glGetString(GL_VERSION);

      std::cout << glversion << std::endl;


    }


    void Window::SwapBuffers()
    {
      MSG msg;
      while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
      {
        if (msg.message == WM_QUIT) close_ = true;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }

      //update time
      currentTime = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - last).count();

      //get the current mouse data
      POINT mouse;
      GetCursorPos(&mouse);

      //update mouse data for the next frame
      memcpy(mouse_, &mouse, sizeof(float) * 2);

      //get window position


      //swap the buffers
      ::SwapBuffers(hDC_);
    }

    Window::~Window()
    {
      wglMakeCurrent(0, 0);
      wglDeleteContext(hGLRC_);
      DeleteDC(hDC_);
      DestroyWindow(handle_);
      UnregisterClass("--lopea--", 0);
    }

    void Window::ClearFrame()
    {
      glViewport(0, 0, dimensions_[0], dimensions_[1]);
      glClear(GL_COLOR_BUFFER_BIT);
    }

    void Window::SetClearColor(float r, float g, float b, float a)
    {
      glClearColor(r, g, b, a);

    }

}