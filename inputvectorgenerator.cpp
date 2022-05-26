#include "inputvectorgenerator.h"

std::vector<BinaryVector> generateInputVectors(size_t inputLength, size_t blockSize)
{
    std::srand(std::time(nullptr));
    size_t blockCount = inputLength / blockSize;

    std::vector<BinaryVector> result;

    for(int i = 0; i < blockCount; i++)
    {
        BinaryVector block(blockSize);
        for(int j = 0; j < blockSize; j++)
        {
            block.set(j, std::rand() % 2);
        }
        result.push_back(block);
    }

    return result;
}

