#include <Primitives.hpp>

class CVec {
public:
    CVec();

    f32 m_x;
    f32 m_y;
    f32 m_z;
    f32 m_w;

    f32 Length2() const;
    f32 Length3() const;
    f32 Length4() const;

    bool operator!=(const class CVec &t_vec);
    const class CVec &operator/=(const f32 &t_factor);
    const class CVec &operator/(const f32 &t_factor);
    const class CVec &operator*=(const f32 &t_factor);
    const class CVec &operator*(const f32 &t_factor);

    void Mul3(const class CMatrix &t_matrix);
    void Mul4(const class CMatrix &t_matrix);

    f32 Normalise2();
    f32 Normalise3();
    f32 Normalise4();

    f32 NormaliseToLargestComponent2();
    f32 NormaliseToLargestComponent3();
    f32 NormaliseToLargestComponent4();

    static i8 PointIntersect3Plane(class CVec &t_a, class CVec &t_b, class CVec &t_c, class CVec &t_d);
};

static const CVec &operator/(const class CVec &t_vec, const f32 &t_factor);