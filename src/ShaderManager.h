//
// Created by Javier on 7/24/2021.
//

#ifndef EPSILONEDITOR_SHADERMANAGER_H
#define EPSILONEDITOR_SHADERMANAGER_H

#include <string>
#include <TextEditor.h>

namespace Epsilon
{
    class ShaderManager
    {
    public:

        ShaderManager();
        void Render(double time);

        bool CanRender() const { return program != 0;}
        void Update(double time);

        ~ShaderManager();

        void ForceRefresh() { needsUpdate_ = true; }


    private:
        void CreateImguiWindow();
        void InitializeShader();
        void InitializeMesh();
        unsigned program;
        bool needsUpdate_;
        std::string data_;

        std::string errMsg_;
        unsigned fragmentHandle, vertexHandle;
        unsigned vbo_, vao_, ebo_;
        TextEditor editor;
        float last;
    };
}

#endif //EPSILONEDITOR_SHADERMANAGER_H
