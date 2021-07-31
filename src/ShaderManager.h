//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_SHADERMANAGER_H
#define EPSILONEDITOR_SHADERMANAGER_H

#include <string>
#include <TextEditor.h>
#include "ImGuiHandler.h"

namespace Epsilon
{
    class ShaderManager
    {
    public:
        struct UniformData
        {
            float mouse[4];
            float resolution[2];
            float time;
        };
        ShaderManager();
        void Render(const UniformData &time);

        bool CanRender() const { return program != 0;}
        void Update(const UniformData &data);

        ~ShaderManager();

        void SetData(const std::string& data);
        [[nodiscard]] std::string GetData() const {return imguiHandle_.GetEditorString();}
        void ForceRefresh() { needsUpdate_ = true; }


    private:
        void InitializeShader();
        void InitializeMesh();
        void InitializeUniformBuffer();
        void ApplyUniforms(const UniformData &data);
        unsigned program;
        bool needsUpdate_;
        std::string data_;
        unsigned fragmentHandle, vertexHandle;
        unsigned vbo_, vao_, ebo_, ubo_;
        ImGuiHandler imguiHandle_;
        float last;
    };
}

#endif //EPSILONEDITOR_SHADERMANAGER_H
