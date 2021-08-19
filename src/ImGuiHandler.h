//
// Created by Javier on 7/31/2021.
//

#ifndef EPSILONEDITOR_IMGUIHANDLER_H
#define EPSILONEDITOR_IMGUIHANDLER_H

#include <string>
#include <TextEditor.h>
#include <functional>
namespace Epsilon
{
    enum WantFlagBit
    {
        wNone = 0x0,
        wSave = 0x2,
        wNewShader = 0x4
    };

    using WantFlags = unsigned char;

    class ImGuiHandler
    {
    public:
        ImGuiHandler();

        void Render();

        [[nodiscard]] std::string GetEditorString() const{return editor_.GetText();};

        [[nodiscard]] std::size_t GetEditorCount() const { return editor_.GetText().size();};

        bool IsStartStringInit() const {return startString_ != nullptr;}
        void SetStartString(const char* string) { startString_ = string;};
        void SetEditorText(const std::string &text) { editor_.SetText(text); needsUpdating_ = true; isDirty_ = false;}
        void SetErrorText(const std::string &text);
        void ClearErrorText();
        WantFlags GetWantFlags() const {return wantFlags_;}
        void SetConfirmationModal(const std::function<void()> &onConfirmation);
    private:
        void RenderBar();
        void FindErrorMarkers();
        void ClearErrorMarkers();
        void UpdateModals();
        void UpdateSideBar();


        ImVec2 barSize_;
        TextEditor editor_;
        const char * startString_ = nullptr;
        std::string location_;
        std::string errMsg_;
        bool showModifiedModal_ = false, isDirty_ = false, needsUpdating_ = false;
        std::function<void()> onModalConfirm_;
        WantFlags wantFlags_{};



    };
}

#endif //EPSILONEDITOR_IMGUIHANDLER_H
