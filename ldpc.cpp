#include "ldpc.h"

BaseGraph::BaseGraph(std::initializer_list<std::vector<int>> rows) {
    _m = rows.size();
    _n = rows.begin()->size();
    _values.reserve(_m);

    for(auto& row : rows)
    {
        if(row.size() != _n)
        {
            throw std::out_of_range("Wrong row length");
        }
        _values.push_back(row);
    }
}

BinaryMatrix BaseGraph::getCheckMatrix(int m, int n) {
    int k = n - m;
    int mulFactor = std::max(m, k);
    int z = std::max(m / _m, mulFactor / (_n / 2)) + 1;
    BinaryMatrix subResult(_m * z, _n * z);

    for(int i = 0; i < _m; i++)
    {
        for(int j = 0; j < _n; j++)
        {
            int shift = _values[i][j];
            if(shift != -1) {
                for (int k = 0; k < z; k++) {
                    subResult.set(i * z + k, j * z + (k + shift) % z, true);
                }
            }
        }
    }

    return subResult.getSubmatrix(0, _m * z / 2 - k, m, n);
}

void LdpcEncoder::setBaseGraph(BaseGraph graph) {
    _graph = graph;
}

void LdpcEncoder::setEncodingParams(int k, int n) {
    _checkMatrix = _graph.getCheckMatrix(n - k, n);

    makeBuffers(n-k, n);
}

BinaryVector RichardsonEncoder::encode(const BinaryVector& input) {
    auto p1 = _p1Factor * input;
    auto p2 = _inverseT * (_A * input + _B * p1);

    return {input, p1, p2};
}

void RichardsonEncoder::makeBuffers(int m, int n)
{
    int g = 0;
    for(int j = n - 1; j > n - m; j--)
    {
        if( _checkMatrix.get(0, j) != 0)
        {
            g = j - (n - m);
            break;
        }
    }

    auto A = _checkMatrix.getSubmatrix(0, 0, m - g, n - m);
    auto B = _checkMatrix.getSubmatrix(0, n - m, m - g, g);
    auto T = BinaryMatrix::identity(m - g);
    auto C = _checkMatrix.getSubmatrix(m - g, 0, g, n - m);
    auto D = BinaryMatrix::identity(g);
    auto E = _checkMatrix.getSubmatrix(m - g, n - m + g, g, m - g);

    auto inverseT = T.inversed();
    auto phi = E * inverseT * B + D;
    auto inversePhi = BinaryMatrix::identity(g);;

    _A = SparseBinaryMatrix(A);
    _B = SparseBinaryMatrix(B);
    _inverseT = SparseBinaryMatrix(inverseT);
    _p1Factor = SparseBinaryMatrix(inversePhi * (E * inverseT * A + C));
}

BinaryVector RichardsonSpecialEncoder::encode(const BinaryVector &input) {
    auto p1 = _p1Factor * input;
    auto p2 = _A * input + _B * p1;

    return {input, p1, p2};
}

void RichardsonSpecialEncoder::makeBuffers(int m, int n) {
    int g = 0;
    for(int j = n - 1; j > n - m; j--)
    {
        if( _checkMatrix.get(0, j) != 0)
        {
            g = j - (n - m);
            break;
        }
    }

    auto C = _checkMatrix.getSubmatrix(0, 0, g, n - m);
    auto P = _checkMatrix.getSubmatrix(0, n - m, g, g);
    auto A = _checkMatrix.getSubmatrix(g, 0, m - g, n - m);
    auto B = _checkMatrix.getSubmatrix(g, n - m, m - g, g);

    auto inverseP = BinaryMatrix::identity(g);

    _A = SparseBinaryMatrix(A);
    _B = SparseBinaryMatrix(B);

    _p1Factor = SparseBinaryMatrix(inverseP * C);
}

BinaryVector AdaptiveEncoder::encode(const BinaryVector &input) {
    auto p = _pFactor * input;
    return {input, p};
}

void AdaptiveEncoder::makeBuffers(int m, int n) {
    int t = m;

    auto A = _checkMatrix.getSubmatrix(0, 0, t, n-t);
    auto inverseT = BinaryMatrix::identity(t);

    _pFactor = SparseBinaryMatrix(inverseT * A);
}

BinaryVector RaEncoder::encode(const BinaryVector &input, int k, int n) {
    int q = n / k;
    BinaryVector s(n);
    for(int i = 0; i < k; i++)
    {
        for(int j = 0; j < q; j++)
        {
            s.set(i + j, input.get(i));
        }
    }
    s.shuffle();
    BinaryVector result(n);
    result.set(0, s.get(0));
    for(int i = 1; i < n; i++)
    {
        result.set(i, result.get(i - 1) ^ s.get(i));
    }
    return result;
}
