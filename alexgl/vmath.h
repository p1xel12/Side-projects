#include <math.h>

#ifndef _AGL_VMATH_H
#define _AGL_VMATH_H

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    float x, y, z;
} vec3_t;

typedef struct {
    float x, y, z, w;
} vec4_t;

vec2_t add2(vec2_t v0, vec2_t v1) {
    v0.x += v1.x;
    v0.y += v1.y;
    return v0;
}

vec3_t add3(vec3_t v0, vec3_t v1) {
    v0.x += v1.x;
    v0.y += v1.y;
    v0.z += v1.z;
    return v0;
}

vec4_t add4(vec4_t v0, vec4_t v1) {
    v0.x += v1.x;
    v0.y += v1.y;
    v0.z += v1.z;
    v0.w += v1.w;
    return v0;
}

vec2_t sub2(vec2_t v0, vec2_t v1) {
    v0.x -= v1.x;
    v0.y -= v1.y;
    return v0;
}

vec3_t sub3(vec3_t v0, vec3_t v1) {
    v0.x -= v1.x;
    v0.y -= v1.y;
    v0.z -= v1.z;
    return v0;
}

vec4_t sub4(vec4_t v0, vec4_t v1) {
    v0.x -= v1.x;
    v0.y -= v1.y;
    v0.z -= v1.z;
    v0.w -= v1.w;
    return v0;
}

vec2_t mul2(vec2_t v0, vec2_t v1) {
    v0.x *= v1.x;
    v0.y *= v1.y;
    return v0;
}

vec3_t mul3(vec3_t v0, vec3_t v1) {
    v0.x *= v1.x;
    v0.y *= v1.y;
    v0.z *= v1.z;
    return v0;
}

vec4_t mul4(vec4_t v0, vec4_t v1) {
    v0.x *= v1.x;
    v0.y *= v1.y;
    v0.z *= v1.z;
    v0.w *= v1.w;
    return v0;
}

vec2_t div2(vec2_t v0, vec2_t v1) {
    v0.x /= v1.x;
    v0.y /= v1.y;
    return v0;
}

vec3_t div3(vec3_t v0, vec3_t v1) {
    v0.x /= v1.x;
    v0.y /= v1.y;
    v0.z /= v1.z;
    return v0;
}

vec4_t div4(vec4_t v0, vec4_t v1) {
    v0.x /= v1.x;
    v0.y /= v1.y;
    v0.z /= v1.z;
    v0.w /= v1.w;
    return v0;
}

#define PI 3.1416

float deg_to_rad(int deg) {
    return deg*PI / 180.0f;
}

vec2_t rot2(vec2_t v, float ang) {
    v.x = 5;
}

vec3_t rot3x(vec3_t v, float ang);

vec3_t rot3y(vec3_t v, float ang) {
    float x = v.x;
    float z = v.z;

    v.x = x*cos(ang) + z*sin(ang);
    v.z = -x*sin(ang) + z*cos(ang);

    return v;
}

float dot2(vec2_t v0, vec2_t v1) {
    return (v0.x*v1.x) + (v0.y*v1.y);
}

float dot3(vec3_t v0, vec3_t v1) {
    return (v0.x*v1.x) + (v0.y*v1.y) + (v0.z*v1.z);
}

float dot4(vec4_t v0, vec4_t v1) {
    return (v0.x*v1.x) + (v0.y*v1.y) + (v0.z*v1.z) + (v0.w*v1.w);
}

float cross2(vec2_t v0, vec2_t v1) {
    return (v0.x*v1.y) - (v0.y*v1.x);
}

#endif