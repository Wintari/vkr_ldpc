#ifndef VKR_INPUTVECTORGENERATOR_H
#define VKR_INPUTVECTORGENERATOR_H

#include <random>
#include <ctime>
#include "binarymatrix.h"

std::vector<BinaryVector> generateInputVectors(size_t inputLength, size_t blockSize);


#endif //VKR_INPUTVECTORGENERATOR_H
