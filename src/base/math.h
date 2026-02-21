#pragma once

// Vectors
union Vec2
{
    struct
    {
        f32 x;
        f32 y;
    };
    struct
    {
        f32 width;
        f32 height;
    };
    struct
    {
        f32 min;
        f32 max;
    };
    f32 data[2];
};

union Vec3
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
    };
    struct
    {
        f32 r;
        f32 g;
        f32 b;
    };
    struct
    {
        Vec2 xy;
        f32 _z0;
    };
    struct
    {
        f32 _x0;
        Vec2 yz;
    };
    f32 data[3];
};

union Vec4
{
    struct
    {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
    struct
    {
        f32 r;
        f32 g;
        f32 b;
        f32 a;
    };
    struct
    {
        Vec2 xy;
        Vec2 zw;
    };
    struct
    {
        f32 _x0;
        Vec2 yz;
        f32 _w0;
    };
    struct
    {
        Vec3 xyz;
        f32 _w1;
    };
    struct
    {
        f32 _x1;
        Vec3 yzw;
    };
    f32 data[4];

};

// Rect position starts at top-left corner
union Rect
{
    struct
    {
        Vec2 pos;
        Vec2 dim;
    };
    struct
    {
        f32 x;
        f32 y;
        f32 width;
        f32 height;
    };
    f32 data[4];

};

// TODO: Matrices
struct Mat3
{
    f32 v[3][3];
};

struct Mat4
{
    f32 v[4][4];
};

// TODO: SIMD
static Vec2 operator+(Vec2 a, Vec2 b)
{
    return {a.x + b.x, a.y + b.y};
}

template<typename Type>
static Vec2 operator+(Vec2 a, Type b)
{
    return {a.x + b, a.y + b};
}

static Vec2 operator-(Vec2 a, Vec2 b)
{
    return {a.x - b.x, a.y - b.y};
}

template<typename Type>
static Vec2 operator-(Vec2 a, Type b)
{
    return {a.x - b, a.y - b};
}

static Vec2 operator*(Vec2 a, Vec2 b)
{
    return {a.x * b.x, a.y * b.y};
}

template<typename Type>
static Vec2 operator*(Vec2 a, Type b)
{
    return {a.x * b, a.y * b};
}

static Vec2 operator/(Vec2 a, Vec2 b)
{
    return {a.x / b.x, a.y / b.y};
}

template<typename Type>
static Vec2 operator/(Vec2 a, Type b)
{
    return {a.x / b, a.y / b};
}

static b8 operator==(Vec2 a, Vec2 b)
{
    return a.x == b.x && a.y == b.y;
}

static b8 operator!=(Vec2 a, Vec2 b)
{
    return a.x != b.x || a.y != b.y;
}

static Vec3 operator+(Vec3 a, Vec3 b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z};
}

template<typename Type>
static Vec3 operator+(Vec3 a, Type b)
{
    return {a.x + b, a.y + b, a.z + b};
}

static Vec3 operator-(Vec3 a, Vec3 b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

template<typename Type>
static Vec3 operator-(Vec3 a, Type b)
{
    return {a.x - b, a.y - b, a.z - b};
}

static Vec3 operator*(Vec3 a, Vec3 b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z};
}

template<typename Type>
static Vec3 operator*(Vec3 a, Type b)
{
    return {a.x * b, a.y * b, a.z * b};
}

static Vec3 operator/(Vec3 a, Vec3 b)
{
    return {a.x / b.x, a.y / b.y, a.z / b.z};
}

template<typename Type>
static Vec3 operator/(Vec3 a, Type b)
{
    return {a.x / b, a.y / b, a.z / b};
}

static b8 operator==(Vec3 a, Vec3 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z;
}

static b8 operator!=(Vec3 a, Vec3 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z;
}

static Vec4 operator+(Vec4 a, Vec4 b)
{
    return {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
}

template<typename Type>
static Vec4 operator+(Vec4 a, Type b)
{
    return {a.x + b, a.y + b, a.z + b, a.w + b};
}

static Vec4 operator-(Vec4 a, Vec4 b)
{
    return {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
}

template<typename Type>
static Vec4 operator-(Vec4 a, Type b)
{
    return {a.x - b, a.y - b, a.z - b, a.w - b};
}

static Vec4 operator*(Vec4 a, Vec4 b)
{
    return {a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w};
}

template<typename Type>
static Vec4 operator*(Vec4 a, Type b)
{
    return {a.x * b, a.y * b, a.z * b, a.w * b};
}

static Vec4 operator/(Vec4 a, Vec4 b)
{
    return {a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w};
}

template<typename Type>
static Vec4 operator/(Vec4 a, Type b)
{
    return {a.x / b, a.y / b, a.z / b, a.w / b};
}

static b8 operator==(Vec4 a, Vec4 b)
{
    return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

static b8 operator!=(Vec4 a, Vec4 b)
{
    return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

static f32 dot(Vec2 a, Vec2 b)
{
    return (a.x * b.x) + (a.y * b.y);
}

static f32 dot(Vec3 a, Vec3 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

static f32 dot(Vec4 a, Vec4 b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

static Vec3 cross(Vec3 a, Vec3 b)
{
    return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

// TODO: Matrix Math

