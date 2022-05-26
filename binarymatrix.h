#ifndef VKR_BINARYMATRIX_H
#define VKR_BINARYMATRIX_H

#include <vector>
#include <initializer_list>
#include <stdexcept>
#include <random>

class SparseBinaryMatrix;

class BinaryVector
{
private:
    std::vector<bool> _values;
public:
    BinaryVector(int length);
    BinaryVector(std::initializer_list<bool> values);
    BinaryVector(std::initializer_list<BinaryVector> subVectors);
    void set(int i, bool value);
    bool get(int i) const;
    int length() const;
    void shuffle();

    friend BinaryVector operator+(const BinaryVector& left, const BinaryVector& right);
};

class BinaryMatrix
{
private:
    std::vector<std::vector<bool>> _values;
    int _m = 0;
    int _n = 0;
public:
    BinaryMatrix() = default;
    BinaryMatrix(int m, int n);
    BinaryMatrix(std::initializer_list<BinaryVector> rows);
    static BinaryMatrix identity(int n);
    void set(int i, int j, bool value);
    bool get(int i, int j) const;
    BinaryMatrix getSubmatrix(int i, int j, int m, int n) const;
    BinaryMatrix inversed();
    BinaryMatrix transposed();
    friend BinaryMatrix operator*(const BinaryMatrix& left, const BinaryMatrix& right);
    friend BinaryMatrix operator+(const BinaryMatrix& left, const BinaryMatrix& right);
    friend SparseBinaryMatrix;
};

class SparseBinaryMatrix
{
private:
    std::vector<int> _cols;
    std::vector<int> _pointer;
    int _m = 0;
    int _n = 0;
public:
    SparseBinaryMatrix(const BinaryMatrix& matrix);
    SparseBinaryMatrix() = default;

    friend BinaryVector operator*(const BinaryVector& left, const SparseBinaryMatrix& right);
    friend BinaryVector operator*(const SparseBinaryMatrix& left, const BinaryVector& right);
};

#endif //VKR_BINARYMATRIX_H
