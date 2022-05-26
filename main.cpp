#include <iostream>
#include <time.h>
#include "ldpc.h"
#include "inputvectorgenerator.h"

double testSimpleRichardson(int k, int n, std::vector<BinaryVector>& input);
double testSpecialRichardson(int k, int n, std::vector<BinaryVector>& input);
double testAdaptive(int k, int n, std::vector<BinaryVector>& input);
double testRA(int k, int n, std::vector<BinaryVector>& input);

int main() {
    int k = 16, n = 256;
    auto input = generateInputVectors(1'024'000, k);
    
    testSimpleRichardson(k, n, input);
    testSpecialRichardson(k, n, input);
    testAdaptive(k, n, input);
    testRA(k, n, input);

    return -1;
}

double testSimpleRichardson(int k, int n, std::vector<BinaryVector>& input)
{
    BaseGraph graph = {{19, 1, 12, 0, 0, -1, -1, -1},
                       {67, 4, 18, 12, 23, 0, -1, -1},
                       {13, 7, 27, 43, 14, 54, 0, -1},
                       {23, 5, 23, 52, 13, 12, 34, 0},
                       {23, 5, 23, 52, 13, 12, 54, -1}};

    RichardsonEncoder encoder;
    encoder.setBaseGraph(graph);
    encoder.setEncodingParams(k, n);

    clock_t start = clock();

    for(const auto& vector : input)
    {
        auto a = encoder.encode(vector);
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "Simple Richardson: " << seconds << " seconds" << std::endl;

    return seconds;
}

double testSpecialRichardson(int k, int n, std::vector<BinaryVector>& input)
{
    BaseGraph graph = {{19, 1, 12, 0, 0, 23, -1, -1},
                       {67, 4, 18, 12, 23, 34, -1, -1},
                       {13, 7, 27, 43, 14, 0, -1, -1},
                       {23, 5, 23, 52, 13, 53, 0, -1},
                       {23, 5, 23, 52, 13, 23, -1, 0}};

    RichardsonSpecialEncoder encoder;
    encoder.setBaseGraph(graph);
    encoder.setEncodingParams(k, n);

    clock_t start = clock();

    for(const auto& vector : input)
    {
        encoder.encode(vector);
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "Special Richardson: " << seconds << " seconds"  << std::endl;

    return seconds;
}

double testAdaptive(int k, int n, std::vector<BinaryVector>& input)
{
    BaseGraph graph = {{19, 1, 12, 54, 0, -1, -1, -1},
                       {67, 4, 18, 12, 23, 0, -1, -1},
                       {13, 7, 27, 43, 14, 54, 0, -1},
                       {23, 5, 23, 52, 13, 12, 34, 0}};

    AdaptiveEncoder encoder;
    encoder.setBaseGraph(graph);
    encoder.setEncodingParams(k, n);

    clock_t start = clock();

    for(const auto& vector : input)
    {
        auto a = encoder.encode(vector);
    }

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    std::cout << "Adaptive: " << seconds << " seconds"  << std::endl;

    return seconds;
}

double testRA(int k, int n, std::vector<BinaryVector>& input)
{
    if(n / k > 1) {
        RaEncoder encoder;

        clock_t start = clock();

        for (const auto &vector: input) {
            auto a = encoder.encode(vector, k, n);
        }

        clock_t end = clock();
        double seconds = (double) (end - start) / CLOCKS_PER_SEC;
        std::cout << "RA: " << seconds << " seconds" << std::endl;

        return seconds;
    }

    return 0;
}
