//
// Created by louis on 28/11/16.
//

#ifndef RENDUOPENGL_MATRIX_H
#define RENDUOPENGL_MATRIX_H

template<typename T> class Vector {
public :
    Vector(int size);
    virtual ~Vector();

    Vector<T> operator+(const Vector<T> &rhs) const;
private :
    int _length;
    T* _data;
};

template<typename T> class Matrix {
public :
    Matrix(int rows, int cols);
    virtual ~Matrix();

private :
    int _rows;
    int _cols;
    Vector<T> *_data;
};


#endif //RENDUOPENGL_MATRIX_H
