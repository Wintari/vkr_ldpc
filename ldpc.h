#ifndef VKR_LDPC_H
#define VKR_LDPC_H

#include "binarymatrix.h"

class BaseGraph {
    std::vector<std::vector<int>> _values;
    int _m = 0;
    int _n = 0;
public:
    BaseGraph(std::initializer_list<std::vector<int>>rows);
    BaseGraph() = default;
    BinaryMatrix getCheckMatrix(int m, int n);
};

class LdpcEncoder
{
private:
    BaseGraph _graph;
protected:
    BinaryMatrix _checkMatrix;
public:
    virtual BinaryVector encode(const BinaryVector& input) = 0;
    virtual void makeBuffers(int m, int n) = 0;
    void setBaseGraph(BaseGraph graph);
    void setEncodingParams(int k, int n);
    virtual ~LdpcEncoder() = default;

    LdpcEncoder() = default;
};

class RichardsonEncoder: public LdpcEncoder
{
private:
    SparseBinaryMatrix _p1Factor;
    SparseBinaryMatrix _A;
    SparseBinaryMatrix _B;
    SparseBinaryMatrix _inverseT;
public:
    BinaryVector encode(const BinaryVector& input) override;
    void makeBuffers(int m, int n) override;
};

class RichardsonSpecialEncoder: public LdpcEncoder
{
private:
    SparseBinaryMatrix _p1Factor;
    SparseBinaryMatrix _A;
    SparseBinaryMatrix _B;
public:
    BinaryVector encode(const BinaryVector& input) override;
    void makeBuffers(int m, int n) override;
};

class AdaptiveEncoder: public LdpcEncoder
{
private:
    SparseBinaryMatrix _pFactor;
public:
    BinaryVector encode(const BinaryVector& input) override;
    void makeBuffers(int m, int n) override;
};


class RaEncoder
{
public:
    BinaryVector encode(const BinaryVector& input, int k, int n);
};
#endif //VKR_LDPC_H
