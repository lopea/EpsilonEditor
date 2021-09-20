//
// Created by Javier on 9/10/2021.
//

#include "UniformManager.h"

namespace Epsilon
{
    void UniformManager::ApplyUniforms()
    {
        //iterate through each block
        for(UniformDataBlock& dataBlock : blocks)
        {
            //check if we need to bind the data block
            if(!dataBlock.isBound())
            {

            }
        }
    }

    const UniformDataBlock *UniformManager::GetUniformBlock()
    {
        return nullptr;
    }

    unsigned int UniformManager::CreateBlock(std::size_t dataSize)
    {
        return 0;
    }

    void *UniformManager::GetBlockData(size_t &size)
    {
        return nullptr;
    }
}