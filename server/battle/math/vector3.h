#ifndef VECTOR3_H
#define VECTOR3_H

#include <math.h>

class vector3_t {
public:
    vector3_t() {
        x = 0.0f;
        y = 0.0f;
        z = 0.0f;
    }

    vector3_t(float x_, float y_, float z_) {
        x = x_;
        y = y_;
        z = z_;
    }

    vector3_t(const vector3_t& v) {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    float& operator[] (int i) {
        return ((float *)this)[i];
    }

    operator float* () {
        return (float*)this;
    }

    vector3_t& operator= (const vector3_t& v) {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    bool operator== (const vector3_t& v) const {
        return (x == v.x && y == v.y && z == v.z);
    }

    bool operator!= (const vector3_t& v) const {
        return (x != v.x || y != v.y || z != v.z);
    }

    vector3_t operator+ (const vector3_t& v) const {
        return vector3_t(x + v.x, y + v.y, z + v.z);
    }

    vector3_t operator- (const vector3_t& v) const {
        return vector3_t(x - v.x, y - v.y, z - v.z);
    }

    vector3_t operator* (const vector3_t& v) const {
        return vector3_t(x * v.x, y * v.y, z * v.z);
    }

    vector3_t operator* (float f) const {
        return vector3_t(x * f, y * f, z * f);
    }

    vector3_t operator/ (float f) const {
        return vector3_t(x / f, y / f, z / f);
    }

    vector3_t operator- () const {
        return vector3_t(-x, -y, -z);
    }

    vector3_t& operator+= (const vector3_t& v) {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }

    vector3_t& operator-= (const vector3_t& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
        return *this;
    }

    vector3_t& operator*= (const vector3_t& v){
        x *= v.x;
        y *= v.y;
        z *= v.z;
        return *this;
    }

    vector3_t& operator*= (float f) {
        x *= f;
        y *= f;
        z *= f;
        return *this;
    }

    vector3_t& operator/= (float f) {
        x /= f;
        y /= f;
        z /= f;
        return *this;
    }

    float square_length() const {
        return x * x + y * y + z * z;
    }

    float length() const {
        return sqrtf(x * x + y * y + z * z);
    }

    vector3_t direction() const {
        float square_len = x * x + y * y + z * z;
        if (square_len != 1.0f) {
            return *this / sqrtf(square_len);
        } else {
            return *this;
        }
    }

    float dot(const vector3_t& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

public:
    float x;

    float y;

    float z;
};

// vector3_t operator*(int s, const vector3_t& v);

// vector3_t operator*(float s, const vector3_t& v);

// vector3_t operator*(double s, const vector3_t& v);

#endif