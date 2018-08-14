#ifndef VECTOR2_H
#define VECTOR2_H

#include <math.h>

class vector2_t {
public:
    vector2_t() {
        x = 0.0f;
        y = 0.0f;
    }

    vector2_t(float x_, float y_) {
        x = x_;
        y = y_;
    }

    vector2_t(const vector2_t& v) {
        x = v.x;
        y = v.y;
    }

    float& operator[] (int i) {
        return ((float *)this)[i];
    }

    operator float* () {
        return (float*)this;
    }

    vector2_t& operator= (const vector2_t& v) {
        x = v.x;
        y = v.y;
        return *this;
    }

    bool operator== (const vector2_t& v) const {
        return (x == v.x && y == v.y);
    }

    bool operator!= (const vector2_t& v) const {
        return (x != v.x || y != v.y);
    }

    vector2_t operator+ (const vector2_t& v) const {
        return vector2_t(x + v.x, y + v.y);
    }

    vector2_t operator- (const vector2_t& v) const {
        return vector2_t(x - v.x, y - v.y);
    }

    vector2_t operator* (const vector2_t& v) const {
        return vector2_t(x * v.x, y * v.y);
    }

    vector2_t operator* (float f) const {
        return vector2_t(x * f, y * f);
    }

    vector2_t operator/ (float f) const {
        return vector2_t(x / f, y / f);
    }

    vector2_t operator- () const {
        return vector2_t(-x, -y);
    }

    vector2_t& operator+= (const vector2_t& v) {
        x += v.x;
        y += v.y;
        return *this;
    }

    vector2_t& operator-= (const vector2_t& v) {
        x -= v.x;
        y -= v.y;
        return *this;
    }

    vector2_t& operator*= (const vector2_t& v){
        x *= v.x;
        y *= v.y;
        return *this;
    }

    vector2_t& operator*= (float f) {
        x *= f;
        y *= f;
        return *this;
    }

    vector2_t& operator/= (float f) {
        x /= f;
        y /= f;
        return *this;
    }

    float square_length() const {
        return x * x + y * y;
    }

    float length() const {
        return sqrtf(x * x + y * y);
    }

    vector2_t direction() const {
        float square_len = x * x + y * y;
        if (square_len != 1.0f) {
            return *this / sqrtf(square_len);
        } else {
            return *this;
        }
    }

    float dot(const vector2_t& v) const {
        return x * v.x + y * v.y;
    }

public:
    float x;

    float y;
};

// vector2_t operator*(int s, const vector2_t& v);

// vector2_t operator*(float s, const vector2_t& v);

// vector2_t operator*(double s, const vector2_t& v);

#endif