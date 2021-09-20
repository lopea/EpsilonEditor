//
// Created by Javier on 9/10/2021.
//

#ifndef EPSILONEDITOR_UNIFORMMANAGER_H
#define EPSILONEDITOR_UNIFORMMANAGER_H

#include "UniformDataBlock.h"
#include <vector>

namespace Epsilon
{
    class UniformManager
    {
    public:
        static void ApplyUniforms();

        static const UniformDataBlock* GetUniformBlock();

        unsigned int CreateBlock(std::size_t dataSize);

        void* GetBlockData(std::size_t& size);
    private:
        static inline std::vector<UniformDataBlock> blocks {};


    };
}

#endif //EPSILONEDITOR_UNIFORMMANAGER_H
