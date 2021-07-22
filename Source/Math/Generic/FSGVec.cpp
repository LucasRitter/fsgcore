#include "Math/FSGMath.hpp"
#include "Core/FSGAssert.hpp"
#include "Math/FSGMatrix.hpp"
#include "Math/FSGVec.hpp"

Vec::Vec()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->w = 0.0;
}

Vec::Vec(const class Vec& vec)
{
    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
    this->w = vec.w;
}

f32 Vec::Length2() const
{
    return FSGSqrt(x * x + y * y);
}

f32 Vec::Length3() const
{
    return FSGSqrt(x * x + y * y + z * z);
}

f32 Vec::Length4() const
{
    return FSGSqrt(x * x + y * y + z * z + w * w);
}

bool Vec::operator!=(const class Vec& vec) const
{
    if(this->x != vec.x || this->y != vec.y || this->z != vec.z || this->w != vec.w)
    {
        return true;
    }

    return false;
}

Vec& Vec::operator/=(const f32& factor)
{
    FSG_ASSERT(factor != 0.0, "Cannot divide by zero.");

    this->x /= factor;
    this->y /= factor;
    this->z /= factor;
    this->w /= factor;

    return *this;
}

class Vec& Vec::operator/(const f32& factor) const
{
    FSG_ASSERT(factor != 0.0, "Cannot divide by zero.");

    auto vec = new Vec();

    vec->x = this->x / factor;
    vec->y = this->y / factor;
    vec->z = this->z / factor;
    vec->w = this->w / factor;

    return *vec;
}

Vec& Vec::operator*=(const f32& factor)
{
    FSG_ASSERT(factor != 0.0, "Cannot divide by zero.");

    this->x *= factor;
    this->y *= factor;
    this->z *= factor;
    this->w *= factor;

    return *this;
}

class Vec& Vec::operator*(const f32& factor) const
{
    FSG_ASSERT(factor != 0.0, "Cannot divide by zero.");

    auto vec = new Vec();

    vec->x = this->x * factor;
    vec->y = this->y * factor;
    vec->z = this->z * factor;
    vec->w = this->w * factor;

    return *vec;
}

class Vec& Vec::operator+=(const class Vec& vec)
{
    this->x += vec.x;
    this->y += vec.y;
    this->z += vec.z;
    this->w += vec.w;

    return *this;
}

class Vec& Vec::operator+(const class Vec& a) const
{
    auto vec = new Vec();

    vec->x = this->x + a.x;
    vec->y = this->y + a.y;
    vec->z = this->z + a.z;
    vec->w = this->w + a.w;

    return *vec;
}

class Vec& Vec::operator-=(const class Vec& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
    this->z -= vec.z;
    this->w -= vec.w;

    return *this;
}

class Vec& Vec::operator-(const class Vec& a) const
{
    auto vec = new Vec();

    vec->x = this->x - a.x;
    vec->y = this->y - a.y;
    vec->z = this->z - a.z;
    vec->w = this->w - a.w;

    return *vec;
}

class Vec Vec::operator*(const class Matrix& mat)
{
    auto vec = new Vec();

    vec->Mul4(mat);

    return *vec;
}

void Vec::Mul3(const class Matrix& matrix)
{
    this->x = ((matrix.m[0][0] * this->x) + ((matrix.m[1][0] * this->y) + (matrix.m[2][0] * this->z))) + matrix.m[3][0];
    this->y = ((matrix.m[0][1] * this->x) + ((matrix.m[1][1] * this->y) + (matrix.m[2][1] * this->z))) + matrix.m[3][1];
    this->z = ((matrix.m[0][2] * this->x) + ((matrix.m[1][2] * this->y) + (matrix.m[2][2] * this->z))) + matrix.m[3][2];
}

void Vec::Mul4(const class Matrix& matrix)
{
    this->x = ((matrix.m[0][0] * this->x) + ((matrix.m[1][0] * this->y) + (matrix.m[2][0] * this->z))) + matrix.m[3][0];
    this->y = ((matrix.m[0][1] * this->x) + ((matrix.m[1][1] * this->y) + (matrix.m[2][1] * this->z))) + matrix.m[3][1];
    this->z = ((matrix.m[0][2] * this->x) + ((matrix.m[1][2] * this->y) + (matrix.m[2][2] * this->z))) + matrix.m[3][2];
    this->w = ((matrix.m[0][3] * this->x) + ((matrix.m[1][3] * this->y) + (matrix.m[2][3] * this->z))) + matrix.m[3][3];
}

f32 Vec::Normalise2()
{
    auto length = this->Length2();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->x *= 1.0f / length;
    this->y *= 1.0f / length;

    return length;
}

f32 Vec::Normalise3()
{
    auto length = this->Length3();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->x *= 1.0f / length;
    this->y *= 1.0f / length;
    this->z *= 1.0f / length;

    return length;
}

f32 Vec::Normalise4()
{
    auto length = this->Length4();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->x *= 1.0f / length;
    this->y *= 1.0f / length;
    this->z *= 1.0f / length;
    this->w *= 1.0f / length;

    return length;
}

f32 Vec::NormaliseToLargestComponent2()
{
    auto largest = this->FindLargestComponent();

    this->x /= largest;
    this->y /= largest;

    return largest;
}

f32 Vec::NormaliseToLargestComponent3()
{
    auto largest = this->FindLargestComponent();

    this->x /= largest;
    this->y /= largest;
    this->z /= largest;

    return largest;
}

f32 Vec::NormaliseToLargestComponent4()
{
    auto largest = this->FindLargestComponent();

    this->x /= largest;
    this->y /= largest;
    this->z /= largest;
    this->w /= largest;

    return largest;
}

f32 Vec::Dot(const class Vec& vec) const
{
    return this->x * vec.x + this->y + vec.y + this->z + vec.z + this->w + vec.w;
}

const class Vec& Vec::Cross3(const class Vec& vec) const
{
    auto result = new Vec();

    result->x = this->y * vec.z - this->z * vec.y;
    result->y = this->z * vec.x - this->x * vec.z;
    result->z = this->x * vec.y - this->y * vec.x;

    return *result;
}

i8 Vec::PointIntersect3Plane(class Vec& plane0, class Vec& plane1, class Vec& plane2, class Vec& point)
{
    // Custom implementation

    const float LIMIT = 1e-4f;

    // Clone all
    auto p0 = new Vec(plane0);
    auto p1 = new Vec(plane1);
    auto p2 = new Vec(plane2);

    // Normalise all
    p0->Normalise3();
    p1->Normalise3();
    p2->Normalise3();

    auto crossed = p0->Cross3(*p1);
    auto det     = crossed.Dot(*p2);

    point.x = 0.0f;
    point.y = 0.0f;
    point.z = 0.0f;
    point.w = 0.0f;

    if(det <= 0.0f)
    {
        return false;
    }

    auto element0          = p1->Cross3(*p2) * -(p0->Length3());
    auto element1          = p2->Cross3(*p0) * p1->Length3();
    auto element2          = p0->Cross3(*p1) * p2->Length3();
    auto intersectionPoint = (element0 - element1 - element2) / det;

    point.x = intersectionPoint.x;
    point.y = intersectionPoint.y;
    point.z = intersectionPoint.z;
    point.w = intersectionPoint.w;

    return true;
}

f32 Vec::FindLargestComponent() const
{
    auto largest = abs(this->x);

    auto absY = abs(this->y);
    auto absZ = abs(this->z);
    auto absW = abs(this->w);

    if(largest < absY)
    {
        largest = absY;
    }

    if(largest < absZ)
    {
        largest = absZ;
    }

    if(largest < absW)
    {
        largest = absW;
    }

    return largest;
}
