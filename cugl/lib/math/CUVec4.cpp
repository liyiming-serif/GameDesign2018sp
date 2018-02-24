//
//  CUVec4.cpp
//  Cornell University Game Library (CUGL)
//
//  This module provides support for a 4d vector.  It has support for basic
//  arithmetic, as well as conversions to color formats.  It also has
//  homogenous vector support for Vec3.
//
//  Because math objects are intended to be on the stack, we do not provide
//  any shared pointer support in this class.
//
//  This module is based on an original file from GamePlay3D: http://gameplay3d.org.
//  It has been modified to support the CUGL framework.
//
//  CUGL zlib License:
//      This software is provided 'as-is', without any express or implied
//      warranty.  In no event will the authors be held liable for any damages
//      arising from the use of this software.
//
//      Permission is granted to anyone to use this software for any purpose,
//      including commercial applications, and to alter it and redistribute it
//      freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not
//      be misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source distribution.
//
//  Author: Walker White
//  Version: 5/30/16
#include <SDL/SDL.h>
#include <algorithm>
#include <iostream>
#include <sstream>

#include <cugl/util/CUDebug.h>
#include <cugl/util/CUStrings.h>
#include <cugl/math/CUVec2.h>
#include <cugl/math/CUVec3.h>
#include <cugl/math/CUVec4.h>
#include <cugl/math/CUColor4.h>

using namespace cugl;

#pragma mark -
#pragma mark Static Arithmetic

/**
 * Clamps the specified vector within the given range and returns it in dst.
 *
 * @param v     The vector to clamp.
 * @param min   The minimum value.
 * @param max   The maximum value.
 * @param dst   A vector to store the result in.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::clamp(const Vec4& v, const Vec4& min, const Vec4& max, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
    dst->x = clampf(v.x,min.x,max.x);
    dst->y = clampf(v.y,min.y,max.y);
    dst->z = clampf(v.z,min.z,max.z);
    dst->w = clampf(v.w,min.w,max.w);
    return dst;
}

/**
 * Returns the angle (in radians) between the specified vectors.
 *
 * This computes the angle between the vectors in 4d space.  It does not
 * treat the vectors as homogenous coordinates.  To get the angle between
 * two homogenous vectors, cast them to Vec3 and use Vec3::angle.
 *
 * The angle returned is unsigned, as there is no clear sense of direction.
 * If either vector is zero, this method returns zero.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 *
 * @return The angle between the two vectors (in radians).
 */
float Vec4::angle(const Vec4& v1, const Vec4& v2) {
    if (v1.isZero() || v2.isZero()) { return 0; }
    return acosf(v1.dot(v2)/(v1.length()*v2.length()));
}

/**
 * Adds the specified vectors and stores the result in dst.
 *
 * @param v1    The first vector.
 * @param v2    The second vector.
 * @param dst   A vector to store the result in
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::add(const Vec4& v1, const Vec4& v2, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
#if defined CU_MATH_VECTOR_APPLE
    dst->v = v1.v+v2.v;
#else
    dst->x = v1.x+v2.x;
    dst->y = v1.y+v2.y;
    dst->z = v1.z+v2.z;
    dst->w = v1.w+v2.w;
#endif
    return dst;
}

/**
 * Subtracts the specified vectors and stores the result in dst.
 * The resulting vector is computed as (v1 - v2).
 *
 * @param v1    The first vector.
 * @param v2    The second vector.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::subtract(const Vec4& v1, const Vec4& v2, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
#if defined CU_MATH_VECTOR_APPLE
    dst->v = v1.v-v2.v;
#else
    dst->x = v1.x-v2.x;
    dst->y = v1.y-v2.y;
    dst->z = v1.z-v2.z;
    dst->w = v1.w-v2.w;
#endif
    return dst;
}

/**
 * Scales the specified vector and stores the result in dst.
 *
 * The scale is uniform by the constant s.
 *
 * @param v     The vector to scale.
 * @param s     The uniform scaling factor.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::scale(const Vec4& v, float s, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
#if defined CU_MATH_VECTOR_APPLE
    dst->v = v.v;
    vSscal(4, s, &(dst->v));
#else
    dst->x = v.x*s; dst->y = v.y*s; dst->z = v.z*s; dst->w = v.w*s;
#endif
    return dst;
}

/**
 * Scales the specified vector and stores the result in dst.
 *
 * The scale is nonuniform by the attributes in v2.
 *
 * @param v1    The vector to scale.
 * @param v2    The nonuniform scaling factor.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::scale(const Vec4& v1, const Vec4& v2, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
#if defined CU_MATH_VECTOR_APPLE
    dst->v = v1.v*v2.v;
#else
    dst->x = v1.x*v2.x; dst->y = v1.y*v2.y; dst->z = v1.z*v2.z; dst->w = v1.w*v2.w;
#endif
    return dst;
}

/**
 * Divides the specified vector and stores the result in dst.
 *
 * The division is uniform by the constant s.
 *
 * @param v     The vector to divide.
 * @param s     The uniform division factor.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::divide(const Vec4& v, float s, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
    CUAssertLog(s != 0,"Division by zero");
#if defined CU_MATH_VECTOR_APPLE
    dst->v = v.v;
    vSscal(4,1/s,&(dst->v));
#else
    dst->x = v.x/s; dst->y = v.y/s; dst->z = v.z/s; dst->w = v.w/s;
#endif
    return dst;
}

/**
 * Divides the specified vector and stores the result in dst.
 *
 * The division is nonuniform by the attributes in v2.
 *
 * @param v1    The vector to divide.
 * @param v2    The nonuniform division factor.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::divide(const Vec4& v1, const Vec4& v2, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
    CUAssertLog(v2.x != 0 && v2.y != 0 && v2.z != 0 && v2.w != 0,"Division by zero");
#if defined CU_MATH_VECTOR_APPLE
    dst->v = v1.v / v2.v;
#else
    dst->x = v1.x / v2.x; dst->y = v1.y / v2.y;
    dst->z = v1.z / v2.z; dst->w = v1.w / v2.w;
#endif
    return dst;
}

/**
 * Reciprocates the specified quaternion and stores the result in dst.
 *
 * The reciprocal is computed for each element invidiually.  This method
 * does not check that all elements are non-zero.  If any element is
 * zero, the result will be NaN.
 *
 * @param v     The vector to reciprocate.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::reciprocate(const Vec4& v, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
    CUAssertLog(v.x != 0 && v.y != 0 && v.z != 0 && v.w != 0,"Reciprocating zero value");
    dst->x = 1.0f/v.x; dst->y = 1.0f/v.y; dst->z = 1.0f/v.z; dst->w = 1.0f/v.w;
    return dst;
}

/**
 * Negates the specified vector and stores the result in dst.
 *
 * @param q1    The quaternion to negate.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::negate(const Vec4& v, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
    dst->x = -v.x; dst->y = -v.y; dst->z = -v.z; dst->w = -v.w;
    return dst;
}


#pragma mark -
#pragma mark Arithmetic
/**
 * Clamps this vector within the given range.
 *
 * @param min The minimum value.
 * @param max The maximum value.
 *
 * @return A reference to this (modified) Vec4 for chaining.
 */
Vec4& Vec4::clamp(const Vec4& min, const Vec4& max) {
    x = clampf(x, min.x, max.x);
    y = clampf(y, min.y, max.y);
    z = clampf(z, min.z, max.z);
    w = clampf(w, min.w, max.w);
    return *this;
}

/**
 * Divides this vector in place by the given factor.
 *
 * @param s The scalar to divide by
 *
 * @return A reference to this (modified) Vec4 for chaining.
 */
Vec4& Vec4::divide(float s) {
    CUAssertLog(s != 0,"Division by zero");
#if defined CU_MATH_VECTOR_APPLE
    vSscal(4,1/s,&v);
#else
    x /= s; y /= s; z /= s; w /= s;
#endif
    return *this;
}

/**
 * Divides this vector nonuniformly by the given factors.
 *
 * @param sx The scalar to divide the x-axis
 * @param sy The scalar to divide the y-axis
 * @param sz The scalar to divide the z-axis
 * @param sw The scalar to divide the w-axis
 *
 * @return A reference to this (modified) Vec4 for chaining.
 */
Vec4& Vec4::divide(float sx, float sy, float sz, float sw) {
    CUAssertLog(sx != 0 && sy != 0 && sz != 0 && sw != 0,"Division by zero");
    x /= sx; y /= sy; z /= sz; w /= sw;
    return *this;
}

/**
 * Divides this vector in place by the given vector.
 *
 * This method is provided to support non-uniform scaling.
 *
 * @param s The vector to divide by
 *
 * @return A reference to this (modified) Vec4 for chaining.
 */
Vec4& Vec4::divide(const Vec4& v) {
    CUAssertLog(v.x != 0 && v.y != 0 && v.z != 0 && v.w != 0,"Division by zero");
#if defined CU_MATH_VECTOR_APPLE
    this->v /= v.v;
#else
    x /= v.x; y /= v.y; z /= v.z; w /= v.w;
#endif
    return *this;
}


#pragma mark -
#pragma mark Linear Algebra
/**
 * Returns the angle between this vector and other.
 *
 * This computes the angle between the vectors in 4d space.  It does not
 * treat the vectors as homogenous coordinates.  To get the angle between
 * two homogenous vectors, cast them to Vec3 and use Vec3::angle.
 *
 * The angle returned is unsigned, as there is no clear sense of direction.
 * If either vector is zero, this method returns zero.
 *
 * @param other The vector to compare.
 *
 * @return the angle between this vector and other.
 */
float Vec4::getAngle(const Vec4& other) const {
    if (isZero() || other.isZero()) { return 0; }
    return acosf(dot(other)/(length()*other.length()));
}

/**
 * Normalizes this vector.
 *
 * This method normalizes Vec4 so that it is of unit length (i.e. the
 * length of the vector after calling this method will be 1.0f). If the
 * vector already has unit length or if the length of the vector is zero,
 * this method does nothing.
 *
 * @return This vector, after the normalization occurs.
 */
Vec4& Vec4::normalize() {
#if defined CU_MATH_VECTOR_APPLE
    float n = vSnorm2(4, &v);
    if (n >= CU_MATH_FLOAT_SMALL) {
        vSscal(4, 1/n, &v);
    }
#else
    float n = x * x + y * y + z * z + w * w;
    // Already normalized.
    if (n == 1.0f) {
        return *this;
    }
    
    n = sqrt(n);
    
    // Do nothing if too close to zero.
    if (n >= CU_MATH_FLOAT_SMALL) {
        n = 1.0f / n;
        x *= n;
        y *= n;
        z *= n;
        w *= n;
    }
#endif
    return *this;
}

/**
 * Updates this vector towards the given target using a smoothing function.
 *
 * The given response time determines the amount of smoothing (lag). A
 * longer response time yields a smoother result and more lag. To force
 * this vector to follow the target closely, provide a response time that
 * is very small relative to the given elapsed time.
 *
 * @param target    The target value.
 * @param elapsed   The elapsed time between calls.
 * @param response  The response time (in the same units as elapsed).
 */
void Vec4::smooth(const Vec4& target, float elapsed, float response) {
    if (elapsed > 0) {
        *this += (target - *this) * (elapsed / (elapsed + response));
    }
}


#pragma mark -
#pragma mark Static Linear Algebra
/**
 * Returns the dot product between the specified vectors.
 *
 * @param v1 The first vector.
 * @param v2 The second vector.
 *
 * @return The dot product between the vectors.
 */
float Vec4::dot(const Vec4& v1, const Vec4& v2) {
#if defined CU_MATH_VECTOR_APPLE
    return vSdot(4, &v1.v, &v2.v);
#else
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w);
#endif
}

/**
 * Normalizes the specified vector and stores the result in dst.
 *
 * If the vector already has unit length or if the length of the
 *  vector is zero, this method copies v into dst.
 *
 * @param v     The vector to normalize.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::normalize(const Vec4& v, Vec4* dst) {
#if defined CU_MATH_VECTOR_APPLE
    float n = vSnorm2(4, &(v.v));
    if (n >= CU_MATH_FLOAT_SMALL) {
        dst->v = v.v;
        vSscal(4, 1/n, &(dst->v));
    }
#else
    float n = v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
    // Already normalized.
    if (n == 1.0f) {
        *dst = v;
        return dst;
    }
    
    n = sqrt(n);
    
    // Do nothing if too close to zero.
    if (n >= CU_MATH_FLOAT_SMALL) {
        n = 1.0f / n;
        dst->x = v.x*n;
        dst->y = v.y*n;
        dst->z = v.z*n;
        dst->w = v.w*n;
    }
#endif
    return dst;
}

/**
 * Homogenizes the specified vector and stores the result in dst.
 *
 * If w is 0, it sets the value to 1.  Otherwise, it divides all
 * attributes by w.
 *
 * @param v     The vector to homogenize.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::homogenize(const Vec4& v, Vec4* dst) {
    float dw = (v.w == 0 ? 1 : 1/v.w);
    *dst = v*dw;
    dst->w = 1;
    return dst;
}


/**
 * Computes the midpoint between two points and stores it in dst.
 *
 * @param v1    The first point.
 * @param v2    The second point.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::midpoint(const Vec4& v1, const Vec4& v2, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
#if defined CU_MATH_VECTOR_APPLE
    dst->v = v1.v+v2.v;
    vSscal(4, 0.5f, &(dst->v));
#else
    dst->x = (v1.x + v2.x) / 2.0f;
    dst->y = (v1.y + v2.y) / 2.0f;
    dst->z = (v1.z + v2.z) / 2.0f;
    dst->w = (v1.w + v2.w) / 2.0f;
#endif
    return dst;
}

/**
 * Computes the projection of one vector on to another and stores it in dst.
 *
 * @param v1    The original vector.
 * @param v2    The vector to project on.
 * @param dst   The destination vector.
 *
 * @return A reference to dst for chaining
 */
Vec4* Vec4::project(const Vec4& v1, const Vec4& v2, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
    *dst = v2 * (v1.dot(v2)/v2.dot(v2));
    return dst;
}

/**
 * Computes the linear interpolation of two vectors and stores it in dst.
 *
 * If alpha is 0, the vector is a copy of v1.  If alpha is 1, the vector is
 * a copy of v2.  Otherwise it is a value on the line v1v2.  This method
 * supports alpha outside of the range 0..1.
 *
 * @param v1    The first point.
 * @param v2    The second point.
 * @param alpha The interpolation value.
 * @param dst   The destination vector.
 *
 * @return The linear interpolation of this vector with other.
 */
Vec4* Vec4::lerp(const Vec4& v1, const Vec4& v2, float alpha, Vec4* dst) {
    CUAssertLog(dst, "Assignment vector is null");
    *dst = v1 * (1.f - alpha) + v2 * alpha;
    return dst;
}


#pragma mark -
#pragma mark Conversion Methods
/**
 * Returns a string representation of this vector for debuggging purposes.
 *
 * If verbose is true, the string will include class information.  This
 * allows us to unambiguously identify the class.
 *
 * @param verbose Whether to include class information
 *
 * @return a string representation of this vector for debuggging purposes.
 */
std::string Vec4::toString(bool verbose) const {
    std::stringstream ss;
    ss << (verbose ? "cugl::Vec4(" : "(");
    ss <<  to_string(x);
    ss << ",";
    ss << to_string(y);
    ss << ",";
    ss << to_string(z);
    ss << ",";
    ss << to_string(w);
    ss << ")";
    return ss.str();
}

/** Cast from Vec4 to a Color4. */
Vec4::operator Color4() const {
    CUAssertLog(0 <= x && x <= 1, "Red value out of range: %.3f",   x);
    CUAssertLog(0 <= y && y <= 1, "Green value out of range: %.3f", y);
    CUAssertLog(0 <= z && z <= 1, "Blue value out of range: %.3f",  z);
    CUAssertLog(0 <= w && w <= 1, "Alpha value out of range: %.3f", w);
    return Color4(COLOR_FLOAT_TO_BYTE(x),COLOR_FLOAT_TO_BYTE(y),
                  COLOR_FLOAT_TO_BYTE(z),COLOR_FLOAT_TO_BYTE(w));
}

/**
 * Creates a vector from the given color.
 *
 * The attributes are read in the order r,g,b,a. Values are all divided
 * uniformly by 255.0.
 *
 * @param color The color to convert
 */
Vec4::Vec4(Color4 color) {
    x = COLOR_BYTE_TO_FLOAT(color.r);
    y = COLOR_BYTE_TO_FLOAT(color.g);
    z = COLOR_BYTE_TO_FLOAT(color.b);
    w = COLOR_BYTE_TO_FLOAT(color.a);
}

/**
 * Sets the coordinates of this vector to those of the given color.
 *
 * The attributes are read in the order r,g,b,a. Values are all divided
 * uniformly by 255.0.
 *
 * @param color The color to convert.
 *
 * @return A reference to this (modified) Vec4 for chaining.
 */
Vec4& Vec4::operator= (Color4 color) {
    x = COLOR_BYTE_TO_FLOAT(color.r);
    y = COLOR_BYTE_TO_FLOAT(color.g);
    z = COLOR_BYTE_TO_FLOAT(color.b);
    w = COLOR_BYTE_TO_FLOAT(color.a);
    return *this;
}

/** Cast from Vec4 to a Color4f. */
Vec4::operator Color4f() const {
    CUAssertLog(0 <= x && x <= 1, "Red value out of range: %.3f",   x);
    CUAssertLog(0 <= y && y <= 1, "Green value out of range: %.3f", y);
    CUAssertLog(0 <= z && z <= 1, "Blue value out of range: %.3f",  z);
    CUAssertLog(0 <= w && w <= 1, "Alpha value out of range: %.3f", w);
    return Color4f(x,y,z,w);
}

/**
 * Creates a vector from the given color.
 *
 * The attributes are read in the order r,g,b,a.
 *
 * @param color The color to convert
 */
Vec4::Vec4(const Color4f& color) {
    x = color.r; y = color.g; z = color.b; w = color.a;
}

/**
 * Sets the coordinates of this vector to those of the given color.
 *
 * The attributes are read in the order r,g,b,a.
 *
 * @param color The color to convert.
 *
 * @return A reference to this (modified) Vec4 for chaining.
 */
Vec4& Vec4::operator= (const Color4f& color) {
    x = color.r; y = color.g; z = color.b; w = color.a;
    return *this;
}

/**
 * Casts from a homogenous vector to Vec4.
 *
 * All coordinates are divided by the w-coordinate (assuming it is not
 * zero) before this conversion. Afterwards, both z and w are dropped.
 */
Vec4::operator Vec2() const {
    float d = (w != 0 ? 1/w : 1);
    return Vec2(x*d,y*d);
}

/**
 * Creates a homogenous vector from the given 2d one.
 *
 * The z-value is set to 0.  However, the w-value is set to 1, to support
 * homogenous coordinates.
 *
 * @param v The vector to convert
 */
Vec4::Vec4(const Vec2& v) {
    x = v.x; y = v.y; z = 0; w = 1;
}

/**
 * Sets the coordinates of this vector to those of the given 2d vector.
 *
 * The z-value is set to 0.  However, the w-value is set to 1, to support
 * homogenous coordinates.
 *
 * @param v The vector to convert
 *
 * @return A reference to this (modified) Vec4 for chaining.
 */
Vec4& Vec4::operator= (const Vec2& v) {
    x = v.x; y = v.y; z = 0; w = 1;
    return *this;
}

/**
 * Casts from Vec4 to Vec3.
 *
 * The w-value is set to 1.  That is because the standard usage of Vec4
 * objects is homogenous coords.
 */
Vec4::operator Vec3() const {
    float d = (w != 0 ? 1/w : 1);
    return Vec3(x*d,y*d,z*d);
}

/**
 * Creates a homogenous vector from the given 3d one.
 *
 * The w-value is set to 1.  That is because the standard usage of Vec4
 * objects is homogenous coords.
 *
 * @param v The vector to convert
 */
Vec4::Vec4(const Vec3& v) {
    x = v.x; y = v.y; z = v.z; w = 1;
}

/**
 * Creates a 4d vector from the given 3d one and optional w value.
 *
 * The w-value is set to the optional value.  Otherwise, it is set to 1.
 *
 * @param v The vector to convert
 * @param w The optional w-coordinate
 */
Vec4::Vec4(const Vec3& v, float w) {
    x = v.x; y = v.y; z = v.z; this->w = w;
}

/**
 * Sets the coordinates of this vector to those of the given 3d vector.
 *
 * The w-value is set to 1.  That is because the standard usage of Vec4
 * objects is homogenous coords.
 *
 * @param v The vector to convert
 *
 * @return A reference to this (modified) Vec3 for chaining.
 */
Vec4& Vec4::operator= (const Vec3& v) {
    x = v.x; y = v.y; z = v.z; w = 1;
    return *this;
}

/**
 * Sets the coordinates of this vector to those of the given 3d vector.
 *
 * The w-value is set to the optional value.  Otherwise, it is set to 1.
 *
 * @param v The vector to convert
 *
 * @return A reference to this (modified) Vec3 for chaining.
 */
Vec4& Vec4::set(const Vec3& v, float w) {
    x = v.x; y = v.y; z = v.z; this->w = w;
    return *this;
}


#pragma mark -
#pragma mark Constants

/** The zero vector Vec4(0,0,0,0) */
const Vec4 Vec4::ZERO(0,0,0,0);
/** The ones vector Vec4(1,1,1,1) */
const Vec4 Vec4::ONE(1,1,1,1);
/** The x-axis Vec4(1,0,0,0) */
const Vec4 Vec4::UNIT_X(1,0,0,0);
/** The y-axis Vec4(0,1,0,0) */
const Vec4 Vec4::UNIT_Y(0,1,0,0);
/** The z-axis Vec4(0,0,1,0) */
const Vec4 Vec4::UNIT_Z(0,0,1,0);
/** The w-axis Vec4(0,0,0,1) */
const Vec4 Vec4::UNIT_W(0,0,0,1);

// Homogenous constants
/** The homogenous origin Vec4(0,0,0,1) */
const Vec4 Vec4::HOMOG_ORIGIN(0,0,0,1);
/** The homogenous x-axis Vec4(1,0,0,1) */
const Vec4 Vec4::HOMOG_X(1,0,0,1);
/** The homogenous y-axis Vec4(0,1,0,1) */
const Vec4 Vec4::HOMOG_Y(0,1,0,1);
/** The homogenous z-axis Vec4(0,0,1,1) */
const Vec4 Vec4::HOMOG_Z(0,0,1,1);

