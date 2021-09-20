//
// Created by Javier on 8/21/2021.
//

#ifndef EPSILONEDITOR_UNIFORMDATABLOCK_H
#define EPSILONEDITOR_UNIFORMDATABLOCK_H
#include <vector>
namespace Epsilon
{
    class UniformDataBlock
    {
    public:
        /*!
         * Constructor for the Uniform Data Block
         * @param blockSize Sets the initial block size for the GLSL Block
         * @param id the identifier for the graphics specification to identify it's corresponding GPU Block.
         */
        explicit UniformDataBlock(unsigned id, size_t blockSize)
        : uniformID(0), update_(true), isBound_(false), data_(blockSize) { }

        /*!
         * Get the pointer to the uniform block for any data changes. The Data wont be applied until render time
         * @return the reference to the data block
         */
        void* GetData() {update_ = true; return data_.data();}

        /*!
         * Set a new size to the uniform block. Note any data in the block may get removed so use at your own risk!
         * @param newsize the new size of the object
         */
        void Resize(std::size_t newsize) { data_.resize(newsize); }

        /*!
         * Check if the block data from the CPU needs to get uploaded to the GPU block
         * @return true if updating the GPU block is necessary, false otherwise
         */
        [[nodiscard]] bool NeedsUpdate() const { return update_;}

        /*!
         * @return Get the block identifier to reference the GPU block.
         */
        [[nodiscard]] unsigned GetID() const { return uniformID; }

        /*!
         * @return true if the graphics specification has a block of memory allocated for this block, false otherwise
         */
        [[nodiscard]] bool isBound() const { return isBound_;}
    private:

        unsigned uniformID, BoundID, ObjectID_;
        bool update_, isBound_;

        std::vector<std::byte> data_;
        friend class UniformManager;
    };
}
#endif //EPSILONEDITOR_UNIFORMDATABLOCK_H
