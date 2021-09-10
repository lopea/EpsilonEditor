//
// Created by javier.s on 9/9/2021.
//

#ifndef EPSILONEDITOR_CONTEXTMANAGER_H
#define EPSILONEDITOR_CONTEXTMANAGER_H


namespace Epsilon
{
    class Window;
    class ContextManager
    {
    public:
        static bool Initialize();

        static Window *CreateWindow(int width, int height);

        static const char* GetVendorName();

        static bool Shutdown();

        static void* GetProcAddress(const char* name);


    private:
        inline static std::vector<Window *> windows;
    };
}

#endif //EPSILONEDITOR_CONTEXTMANAGER_H
