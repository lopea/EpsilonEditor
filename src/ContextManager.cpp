//
// Created by javier.s on 9/9/2021.
//

#include "ContextManager.h"
#include <glad/gl.h>

namespace Epsilon
{
    bool ContextManager::Initialize()
    {
        return true;
    }

    Window *ContextManager::CreateWindow(int width, int height)
    {
        return nullptr;
    }

    const char *ContextManager::GetVendorName()
    {
        return glGetString(GL_VENDOR);
    }

    bool ContextManager::Shutdown()
    {
        glfw
    }
}