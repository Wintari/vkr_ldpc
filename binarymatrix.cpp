#include <algorithm>
#include "binarymatrix.h"

BinaryVector::BinaryVector(int length) {
    _values.resize(length);
}

BinaryVector::BinaryVector(std::initializer_list<bool> values) {
    _values.reserve(values.size());

    for(auto value : values)
    {
        _values.push_back(value);
    }
}

void BinaryVector::set(int i, bool value) {
    _values[i] = value;
}

bool BinaryVector::get(int i) const {
    return _values[i];
}

int BinaryVector::length() const {
    return _values.size();
}

BinaryVector::BinaryVector(std::initializer_list<BinaryVector> subVectors) {
    for(auto& vector: subVectors)
    {
        _values.reserve(_values.size() + vector.length());
        for(auto value : vector._values)
        {
            _values.push_back(value);
        }
    }
}

BinaryVector operator+(const BinaryVector &left, const BinaryVector &right) {
    if(left.length() == right.length())
    {
        BinaryVector result(left.length());

        for(int i = 0; i < result.length(); i++)
        {
            result._values[i] = left._values[i] ^ right._values[i];
        }

        return result;
    }
    else
    {
        throw std::out_of_range("Wrong vectors size");
    }
}

void BinaryVector::shuffle() {
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(_values.begin(), _values.end(), g);
}

BinaryMatrix::BinaryMatrix(int m, int n) : _m(m), _n(n) {
    _m = m;
    _n = n;
    _values.resize(m);
    for(auto& row : _values)
    {
        row.resize(n);
    }
}

BinaryMatrix::BinaryMatrix(std::initializer_list<BinaryVector> rows) {
    auto begin = rows.begin();
    _m = rows.size();
    _n = begin->length();
    _values.resize(_m);

    int i = 0;
    for(auto& row : rows)
    {
        _values[i].reserve(_n);
        if(row.length() != _n)
        {
            throw std::out_of_range("Wrong row length");
        }
        for(int j = 0; j < _n; j++)
        {
            _values[i].push_back(row.get(j));
        }
        i += 1;
    }
}

void BinaryMatrix::set(int i, int j, bool value) {
    if(i < _m && j < _n)
    {
        _values[i][j] = value;
    }
    else
    {
        throw std::out_of_range("Indices out of range");
    }
}

bool BinaryMatrix::get(int i, int j) const {
    if(i < _m && j < _n)
    {
        return _values[i][j];
    }
    else
    {
        throw std::out_of_range("Indices out of range");
    }
}


BinaryMatrix BinaryMatrix::getSubmatrix(int i, int j, int m, int n) const {
    if(i + m  <= _m && j + n <= _n)
    {
        BinaryMatrix result(m, n);

        for(int _i = 0; _i < m; _i++)
        {
            for(int _j = 0; _j < n; _j++)
            {
                result.set(_i, _j, get(_i + i, _j + j));
            }
        }

        return result;
    }
    else
    {
        throw std::out_of_range("Wrong matrices size");
    }
}

BinaryMatrix BinaryMatrix::transposed() {
    BinaryMatrix result(_n, _m);

    for(int i = 0; i < _m; i++)
    {
        for(int j = 0; j < _n; j++)
        {
            result.set(j, i, get(i, j));
        }
    }

    return result;
}

BinaryMatrix operator+(const BinaryMatrix &left, const BinaryMatrix &right) {
    if(left._m == right._m && left._n == right._n)
    {
        BinaryMatrix result(left._m, left._n);

        for(int i = 0; i < result._m; i++)
        {
            for(int j = 0; j < result._n; j++)
            {
                result.set(i, j, left.get(i, j) ^ right.get(i, j));
            }
        }

        return result;
    }
    else
    {
        throw std::out_of_range("Wrong matrices size");
    }
}

BinaryMatrix operator*(const BinaryMatrix &left, const BinaryMatrix &right) {
    if(left._n == right._m)
    {
        BinaryMatrix result(left._m, right._n);

        for(int i = 0; i < result._m; i++)
        {
            for(int j = 0; j < result._n; j++)
            {
                bool n = false;
                for(int k = 0; k < left._n; k++)
                {
                    n ^= left.get(i, k) * right.get(k, j);
                }
                result.set(i, j, n);
            }
        }

        return result;
    }
    else
    {
        throw std::out_of_range("Wrong matrices size");
    }
}

BinaryMatrix BinaryMatrix::inversed() {
    if(_m == _n)
    {
        BinaryMatrix result(_m, _m);
        BinaryMatrix buffer(*this);

        for(int i = 0; i < _m; i++)
        {
            result._values[i][i] = true;
        }
        for (int i = _m - 1; i > 0; i--) {
            if (buffer._values[i - 1][0] < buffer._values[i][0]) {
                buffer._values[i - 1].swap(buffer._values[i]);
                result._values[i - 1].swap(result._values[i]);
            }
        }

        for (int i = 0; i < _m; i++) {
            for (int j = 0; j < _m; j++) {
                if (j != i) {
                    bool isAdditive = buffer._values[j][i] / buffer._values[i][i];
                    for (int k = 0; k < _m; k++) {
                        buffer._values[j][k] = buffer._values[j][k] ^ (buffer._values[i][k] * isAdditive);
                        result._values[j][k] = result._values[j][k] ^ (result._values[i][k] * isAdditive);
                    }
                }
            }
        }

        return result;
    }
    else
    {
        throw std::out_of_range("Wrong matrices size");
    }
}

BinaryMatrix BinaryMatrix::identity(int n) {
    BinaryMatrix result(n, n);
    for(int i = 0; i < n; i++)
    {
        result.set(i, i, true);
    }
    return result;
}

SparseBinaryMatrix::SparseBinaryMatrix(const BinaryMatrix& matrix) {
    _m = matrix._m;
    _n = matrix._n;
    _pointer.push_back(0);

    for(int i = 0; i < _m; i++)
    {
        for(int j = 0; j < _n; j++)
        {
            if(matrix.get(i, j))
            {
                _cols.push_back(j);
            }
        }

        _pointer.push_back(_cols.size());
    }
}

BinaryVector operator*(const BinaryVector &left, const SparseBinaryMatrix &right) {
        return right * left;
}

BinaryVector operator*(const SparseBinaryMatrix &left, const BinaryVector &right) {
    if(left._n == right.length())
    {
        BinaryVector result(left._m);
        for(int i = 0; i < left._pointer.size() - 1; i++)
        {
            bool value = false;
            for(int j = left._pointer[i]; j < left._pointer[i + 1]; j++)
            {
                value ^= right.get(left._cols[j]);
            }
            result.set(i, value);
        }
        return result;
    }
}