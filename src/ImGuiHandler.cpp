//
// Created by Javier on 7/31/2021.
//

#include <iostream>
#include "ImGuiHandler.h"

namespace Epsilon
{
    ImGuiHandler::ImGuiHandler()
    {
      //get the proper palette for the editor
      editor_.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
      TextEditor::Palette palette = editor_.GetPalette();
      palette[(int)TextEditor::PaletteIndex::Background] = 0x00000000;
      palette[(int)TextEditor::PaletteIndex::ErrorMarker] = 0xFF0000AA;
      editor_.SetPalette(palette);

    }

    void ImGuiHandler::Render()
    {
      RenderBar();
      ImGui::GetStyle().ChildRounding = 0;
      ImGui::GetStyle().ChildBorderSize = 0;
      ImVec2 screenPos = ImGui::GetIO().DisplaySize;
      ImGui::SetNextWindowBgAlpha(0);
      ImGui::SetNextWindowSize({screenPos.x, screenPos.y - barSize_.y});
      ImGui::SetNextWindowPos({0,barSize_.y});
      ImGui::Begin("Shader Editor", nullptr, ImGuiWindowFlags_NoMove |ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize);
      editor_.Render("render");

      if(!errMsg_.empty())
      {
        screenPos.x = 0;
        screenPos.y *= 0.15f;
        ImGui::SetNextWindowBgAlpha(.90f);
        ImGui::SetNextWindowPos({0, ImGui::GetIO().DisplaySize.y *0.85f});
        ImGui::BeginChild("Error Message",screenPos, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
        ImGui::Text("%s", errMsg_.c_str());
        ImGui::EndChild();
      }

      ImGui::End();
    }

    void ImGuiHandler::RenderBar()
    {
      wantFlags_ = 0;
      //start menu bar process
      if(ImGui::BeginMainMenuBar())
      {
        //get the size of the window ( for other windows to not overlap the bar )
        barSize_ = ImGui::GetWindowSize();

        if(ImGui::BeginMenu("File"))
        {
          if(ImGui::MenuItem("New Shader...", "Ctrl + N"))
          {
            wantFlags_ |= wNewShader;
          }
          if(ImGui::MenuItem("Save", "Ctrl + S"))
          {
            wantFlags_ |= wSave;
          }
          ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
      }
    }

    void ImGuiHandler::SetErrorText(const std::string &text)
    {
      errMsg_ = text;
      FindErrorMarkers();
    }

    void ImGuiHandler::ClearErrorText()
    {
      errMsg_.clear();
      ClearErrorMarkers();
    }

    void ImGuiHandler::FindErrorMarkers()
    {
      std::size_t pos = errMsg_.find('\n');
      std::string line = errMsg_.substr(0, pos);
      TextEditor::ErrorMarkers marker;

      while (pos != std::string::npos)
      {
        //store the beginning of the line
        std::size_t prev = pos;

        //check if the current line contains an error in the shader
        std::size_t errorNum = line.find("0:");

        //if the current line does contain a line in the shader,
        if(errorNum != std::string::npos)
        {
          //move to line number
          errorNum += 2;
          std::size_t lineNumSize = 0;
          //get the line number
          int lineNum = std::stoi(line.substr(errorNum), &lineNumSize);

          //add the line number and the error message to the error marker database
          marker.emplace(lineNum, line.substr(errorNum + lineNumSize + 2));
        }

        //find the next line in the error message
        pos = errMsg_.find('\n', pos + 1);
        line = errMsg_.substr(prev + 1, pos - prev);
      }

      //send the error marker to the editor
      editor_.SetErrorMarkers(marker);

    }

    void ImGuiHandler::ClearErrorMarkers()
    {
      //remove all errors that are marked in the editor
      editor_.SetErrorMarkers(TextEditor::ErrorMarkers());
    }


}