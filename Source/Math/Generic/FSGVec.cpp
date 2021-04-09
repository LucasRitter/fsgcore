#include <Math/FSGMath.hpp>
#include "Core/FSGAssert.hpp"
#include "Math/FSGVec.hpp"

CVec::CVec() {
    m_x = 0.0;
    m_y = 0.0;
    m_z = 0.0;
    m_w = 0.0;
}

f32 CVec::Length2() const {
    return FSGSqrt(m_x * m_x + m_y * m_y);
}

f32 CVec::Length3() const {
    return FSGSqrt(m_x * m_x + m_y * m_y + m_z * m_z);
}

f32 CVec::Length4() const {
    return FSGSqrt(m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

f32 CVec::Normalise2() {
    auto length = this->Length2();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->m_x *= (1.0 / length);
    this->m_y *= (1.0 / length);

    return length;
}

f32 CVec::Normalise3() {
    auto length = this->Length3();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->m_x *= (1.0 / length);
    this->m_y *= (1.0 / length);
    this->m_z *= (1.0 / length);

    return length;
}

f32 CVec::Normalise4() {
    auto length = this->Length4();
    FSG_ASSERT(length != 0, "Passed 0 length vector for normalisation!");

    this->m_x *= (1.0 / length);
    this->m_y *= (1.0 / length);
    this->m_z *= (1.0 / length);
    this->m_w *= (1.0 / length);

    return length;
}

