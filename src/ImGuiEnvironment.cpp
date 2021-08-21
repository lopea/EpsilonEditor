//
// Created by Javier on 7/24/2021.
//

#include "ImGuiEnvironment.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_opengl3.h"
#include "Window.h"
namespace Epsilon
{
    void ImGuiEnvironment::Initialize()
    {
      assert(!initialized_);


      //do some checks
      IMGUI_CHECKVERSION();

      //initialize imgui
      ImGui::CreateContext();
      ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
      //we're rollin
      initialized_ = true;
    }

    void ImGuiEnvironment::LinkWindow(Window *window)
    {
      //initialize connection with window
      ImGui_ImplWin32_Init(window->GetHandle());

      //Initialize connection with Opengl
      ImGui_ImplOpenGL3_Init("#version 420");

    }

    void ImGuiEnvironment::NewFrame()
    {
      assert(initialized_);

      //set up the new frame for opengl
      ImGui_ImplOpenGL3_NewFrame();

      //set up the new frame for windows
      ImGui_ImplWin32_NewFrame();

      //set up the new frame for imgui
      ImGui::NewFrame();
    }

    void ImGuiEnvironment::Render()
    {
      assert(initialized_);

      //render all imgui data
      ImGui::Render();

      //get the render data for imgui
      ImDrawData* data = ImGui::GetDrawData();

      //send draw data for opengl
      ImGui_ImplOpenGL3_RenderDrawData(data);
    }

    void ImGuiEnvironment::Shutdown()
    {
      //shutdown opengl
      ImGui_ImplOpenGL3_Shutdown();

      //shutdown windows context
      ImGui_ImplWin32_Shutdown();


      //shutdown context
      ImGui::DestroyContext();
    }

}