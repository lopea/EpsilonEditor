//
// Created by Javier on 7/31/2021.
//

#include <iostream>
#include "ImGuiHandler.h"
#include "FileHelper.h"
#include "ImGuiEnvironment.h"
#include "ShaderManager.h"
#include "UniformData.h"

namespace Epsilon
{
    ImGuiHandler::ImGuiHandler()
    {
      //get the proper palette for the editor
      editor_.SetLanguageDefinition(TextEditor::LanguageDefinition::GLSL());
      TextEditor::Palette palette = editor_.GetPalette();
      palette[(int) TextEditor::PaletteIndex::Background] = 0x00000000;
      palette[(int) TextEditor::PaletteIndex::ErrorMarker] = 0xFF0000AA;
      editor_.SetPalette(palette);

      editor_.SetImGuiChildIgnored(true);
      editor_.SetText(ShaderManager::GetStartShader());
      startString_ = ShaderManager::GetStartShader();
    }

    void ImGuiHandler::Render(UniformData &data)
    {
      ImGuiEnvironment::NewFrame();

      //Render Menu Bar
      RenderBar();

      if (editor_.IsTextChanged() && !needsUpdating_)
        isDirty_ = true;

      if (needsUpdating_)
        needsUpdating_ = false;

      ImVec2 screenPos = ImGui::GetIO().DisplaySize;
      ImGuiConfigFlags flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar
          | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoDocking;
      ImGui::SetNextWindowSize({screenPos.x, (errMsg_.empty() ? screenPos.y : screenPos.y * .85f) - barSize_.y});
      ImGui::SetNextWindowPos({0, barSize_.y});
      ImGui::Begin("Shader Dock", nullptr,
                   flags);

      //create dockspace

      ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg,0x00000000);
      ImGui::DockSpace(ImGui::GetID("Shader Editor DockSpace"),{0,0}, 0);
      ImGui::PopStyleColor();
      ImGui::End();

      ImGui::SetNextWindowBgAlpha(0);

      //mGui::SetNextWindowDockID(ImGui::GetID("Shader Editor DockSpace"));
      ImGui::Begin("Shader Editor", nullptr, 0);
      editor_.Render("render");
      ImGui::End();

      UpdateModals();

      if (!errMsg_.empty())
      {
        screenPos.y *= 0.15f;
        ImGui::SetNextWindowBgAlpha(.90f);
        ImGui::SetNextWindowPos({0, ImGui::GetIO().DisplaySize.y * 0.85f});
        ImGui::SetNextWindowSize(screenPos);
        ImGui::Begin("Error Message", nullptr,
                     ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoFocusOnAppearing);
        ImGui::Text("%s", errMsg_.c_str());
        ImGui::End();


      }

      //show the side bar
      UpdateSideBar(data);
      ImGuiEnvironment::Render();
    }

    void ImGuiHandler::RenderBar()
    {

      //start menu bar process
      if (ImGui::BeginMainMenuBar())
      {
        //get the size of the window ( for other windows to not overlap the bar )
        barSize_ = ImGui::GetWindowSize();

        if (ImGui::BeginMenu("File"))
        {
          if (ImGui::MenuItem("Open", "Ctrl + O"))
          {
            if (isDirty_)
            {
              SetConfirmationModal(
                  [this]()
                  {
                      std::string result = FileHelper::ShowOpenDialog();
                      if (!result.empty())
                      {
                        location_ = result;
                        std::string content;
                        FileHelper::Load(result, content);
                        SetEditorText(content);
                        isDirty_ = false;
                      }

                  });
            } else
            {
              std::string result = FileHelper::ShowOpenDialog();
              if (!result.empty())
              {
                location_ = result;
                std::string content;
                FileHelper::Load(result, content);
                SetEditorText(content);
                isDirty_ = false;
              }
            }

          }
          if (ImGui::MenuItem("New Shader...", "Ctrl + N"))
          {
            //give confirmation if the person has modified or not initialized a file
            if (isDirty_)
              SetConfirmationModal([this]()
                                   {
                                       SetEditorText(startString_);
                                       isDirty_ = false;
                                   });
            else
              SetEditorText(startString_);

          }
          if (ImGui::MenuItem("Save", "Ctrl + S"))
          {


            if (location_.empty())
            {
              //store the new location
              std::string newLocation = FileHelper::ShowSaveDialogAndSave(GetEditorString());

              //check if a valid location was selected and set it as the new location if it is valid.
              if (!newLocation.empty())
                location_ = newLocation;
            } else
            {
              FileHelper::Save(location_, GetEditorString());
            }
          }
          if (ImGui::MenuItem("Save As", "Ctrl + S"))
          {
            //store the new location
            std::string newLocation = FileHelper::ShowSaveDialogAndSave(GetEditorString());

            //check if a valid location was selected and set it as the new location if it is valid.
            if (!newLocation.empty())
              location_ = newLocation;
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
        std::size_t errorNum = line.find("0(");

        //if the current line does contain a line in the shader,
        if (errorNum != std::string::npos)
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

    void ImGuiHandler::UpdateModals()
    {
      if (showModifiedModal_)
        ImGui::OpenPopup("Are You Sure? Popup");

      if (ImGui::BeginPopupModal("Are You Sure? Popup", nullptr,
                                 ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
      {
        ImGui::Text("Are you sure you want to continue? All data will be lost!");

        if (ImGui::Button("No"))
        {
          ImGui::CloseCurrentPopup();
          showModifiedModal_ = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Yes"))
        {
          onModalConfirm_();
          ImGui::CloseCurrentPopup();
          onModalConfirm_ = nullptr;
          showModifiedModal_ = false;
        }
        ImGui::EndPopup();
      }
    }

    void ImGuiHandler::UpdateSideBar(UniformData &data)
    {
      ImGui::Begin("Shader Tab");

      //Show and be able to edit uniforms
      if (ImGui::CollapsingHeader("Uniforms"))
      {
        ImGui::Text("Time: %.2f", data.time);

        //reset time
        if (ImGui::Button("Reset Time"))
        {
          data.time = 0;
        }

        ImGui::Separator();

        //show current resolution
        ImGui::Text("Resolution: ( %.0f, %.0f )", data.resolution[0], data.resolution[1]);

        ImGui::Separator();

        //show current mouse coords
        ImGui::Text("Mouse Coords ( %f, %f ) Left Click: %s, Right Click: %s", data.mouse[0], data.mouse[1],
                    data.mouse[2] ? "true" : "false", data.mouse[3] ? "true" : "false");
      }
      ImGui::End();

    }

    void ImGuiHandler::SetConfirmationModal(const std::function<void()> &onConfirmation)
    {
      showModifiedModal_ = true;
      onModalConfirm_ = onConfirmation;
    }


}