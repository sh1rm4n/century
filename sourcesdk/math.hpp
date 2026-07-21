//
// Created by sh1rm4n on 5/16/26.
//

#ifndef CENTURY_MATH_HPP
#define CENTURY_MATH_HPP

#include <algorithm>
#include <cmath>
#include <cstdint>

class Color {
public:
    std::uint8_t color[4];
    float fCol[4];
    int r, g, b, a;
    bool rainbow;

    Color(uint8_t R, uint8_t G, uint8_t B, uint8_t A) {
        color[0] = R;
        color[1] = G;
        color[2] = B;
        color[3] = A;
        r = R; g = G; b = B; a = A;
        fCol[0] = (float)R/255.f; fCol[1] = (float)G / 255.f; fCol[2] = (float)B / 255.f; fCol[3] = (float)A / 255.f;
        rainbow = false;
    }
    Color(uint8_t R, uint8_t G, uint8_t B) {
        color[0] = R;
        color[1] = G;
        color[2] = B;
        color[3] = 255;
        r = R; g = G; b = B; a = 255;
        fCol[0] = (float)R / 255.f; fCol[1] = (float)G / 255.f; fCol[2] = (float)B / 255.f; fCol[3] = 1.f;
        rainbow = false;
    }
};

class Vector {
public:
    float x{0.f}, y{0.f}, z{0.f};

	constexpr Vector() noexcept = default;
    constexpr Vector(const Vector&) noexcept = default;
    constexpr Vector(Vector&&) noexcept = default;
    constexpr Vector& operator=(const Vector&) noexcept = default;
    constexpr Vector& operator=(Vector&&) noexcept = default;
    ~Vector() noexcept = default;

    constexpr Vector(float x_, float y_, float z_) noexcept : x(x_), y(y_), z(z_) {}

    [[nodiscard]] constexpr Vector operator+(const Vector& other) const noexcept {
        return { x + other.x, y + other.y, z + other.z };
    }

    [[nodiscard]] constexpr Vector operator-(const Vector& other) const noexcept {
        return { x - other.x, y - other.y, z - other.z };
    }

    [[nodiscard]] constexpr Vector operator*(const Vector& other) const noexcept {
        return { x * other.x, y * other.y, z * other.z };
    }

    [[nodiscard]] constexpr Vector operator/(const Vector& other) const noexcept {
        return { x / other.x, y / other.y, z / other.z };
    }

    [[nodiscard]] constexpr Vector operator*(float num) const noexcept {
        return { x * num, y * num, z * num };
    }

    [[nodiscard]] constexpr Vector operator/(float num) const noexcept {
        const float inv = 1.0f / num;
        return { x * inv, y * inv, z * inv };
    }

    constexpr Vector& operator+=(const Vector& other) noexcept {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    constexpr Vector& operator-=(const Vector& other) noexcept {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    constexpr Vector& operator*=(const Vector& other) noexcept {
        x *= other.x; y *= other.y; z *= other.z;
        return *this;
    }

    constexpr Vector& operator/=(const Vector& other) noexcept {
        x /= other.x; y /= other.y; z /= other.z;
        return *this;
    }

    constexpr Vector& operator*=(float num) noexcept {
        x *= num; y *= num; z *= num;
        return *this;
    }

    constexpr Vector& operator/=(float num) noexcept {
        const float inv = 1.0f / num;
        x *= inv; y *= inv; z *= inv;
        return *this;
    }

    [[nodiscard]] auto operator<=>(const Vector&) const noexcept = default;

    [[nodiscard]] constexpr float Length2D() const noexcept {
        return std::hypotf(x, y);
    }

    [[nodiscard]] constexpr float LengthSqr() const noexcept {
        return x * x + y * y + z * z;
    }

    [[nodiscard]] constexpr float Length() const noexcept {
        return std::sqrtf(LengthSqr());
    }

    [[nodiscard]] constexpr float DistToSqr(const Vector& other) const noexcept {
        return (*this - other).LengthSqr();
    }

    [[nodiscard]] constexpr float DistTo(const Vector& other) const noexcept {
        return (*this - other).Length();
    }

    [[nodiscard]] constexpr Vector Cross(const Vector& other) const noexcept {
        return {
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        };
    }

    [[nodiscard]] constexpr float Dot(const Vector& other) const noexcept {
        return x * other.x + y * other.y + z * other.z;
    }

    constexpr float Normalize() noexcept {
        const float len = Length();
        if (len > 0.0f) {
            const float inv_len = 1.0f / len;
            x *= inv_len;
            y *= inv_len;
            z *= inv_len;
        } else {
            x = y = z = 0.0f;
        }
        return len;
    }

    [[nodiscard]] constexpr Vector Scale(float factor) const noexcept {
        return { x * factor, y * factor, z * factor };
    }
};

class alignas(16) VectorAligned : public Vector {
    public:
    // Inherit base Vector constructors
    using Vector::Vector;

    // 4th-dimensional padding initialized directly at member level
    float w{ 0.0f };


    // 1. Explicitly enable copying
    constexpr VectorAligned(const VectorAligned&) noexcept = default;
    constexpr VectorAligned& operator=(const VectorAligned&) noexcept = default;

    // 2. Explicitly enable moving
    constexpr VectorAligned(VectorAligned&&) noexcept = default;
    constexpr VectorAligned& operator=(VectorAligned&&) noexcept = default;

    // 3. Explicit baseline Vector conversion constructors
    constexpr VectorAligned(const Vector& other) noexcept
        : Vector{ other.x, other.y, other.z }, w{ 0.0f } {}

    constexpr VectorAligned& operator=(const Vector& other) noexcept {
        x = other.x;
        y = other.y;
        z = other.z;
        w = 0.0f;
        return *this;
    }
};

class Angle {
public:
    float p{0.f}, y{0.f}, r{0.f};

    constexpr Angle() noexcept = default;
    constexpr Angle(float pitch, float yaw, float roll) noexcept : p(pitch), y(yaw), r(roll) {}

    constexpr Angle(const Angle&) noexcept = default;
    constexpr Angle(Angle&&) noexcept = default;
    constexpr Angle& operator=(const Angle&) noexcept = default;
    constexpr Angle& operator=(Angle&&) noexcept = default;
    ~Angle() = default;

    // --- Binary Operators (Fixed: Returns by value instead of a dangling Angle&&) ---
    [[nodiscard]] constexpr Angle operator+(const Angle& other) const noexcept {
        return Angle(p + other.p, y + other.y, r + other.r);
    }

    [[nodiscard]] constexpr Angle operator-(const Angle& other) const noexcept {
        return Angle(p - other.p, y - other.y, r - other.r);
    }

    [[nodiscard]] constexpr Angle operator*(const Angle& other) const noexcept {
        return Angle(p * other.p, y * other.y, r * other.r);
    }

    [[nodiscard]] constexpr Angle operator/(const Angle& other) const noexcept {
        // FIXED BUG: Changed 'y / other.p' to 'y / other.y'
        return Angle(p / other.p, y / other.y, r / other.r);
    }

    [[nodiscard]] constexpr Angle operator*(float num) const noexcept {
        return Angle(p * num, y * num, r * num);
    }

    [[nodiscard]] constexpr Angle operator/(float num) const noexcept {
        return Angle(p / num, y / num, r / num);
    }

    // --- Compound Assignment Operators ---
    constexpr Angle& operator+=(const Angle& other) noexcept {
        p += other.p; y += other.y; r += other.r;
        return *this;
    }

    constexpr Angle& operator-=(const Angle& other) noexcept {
        p -= other.p; y -= other.y; r -= other.r;
        return *this;
    }

    constexpr Angle& operator*=(const Angle& other) noexcept {
        p *= other.p; y *= other.y; r *= other.r;
        return *this;
    }

    constexpr Angle& operator/=(const Angle& other) noexcept {
        p /= other.p; y /= other.y; r /= other.r;
        return *this;
    }

    constexpr Angle& operator*=(float num) noexcept {
        p *= num; y *= num; r *= num;
        return *this;
    }

    constexpr Angle& operator/=(float num) noexcept {
        p /= num; y /= num; r /= num;
        return *this;
    }

    // --- Normalization Methods ---
    constexpr void Normalize() noexcept {
        p = std::clamp(p, -89.f, 89.f);
        y = Normalize180(y);
        r = 0.f;
    }

    [[nodiscard]] static float Normalize180(float ang) noexcept {
        ang = std::fmod(ang + 180.f, 360.f);
        if (ang < 0.f) {
            ang += 360.f;
        }
        return ang - 180.f;
    }

    // Fixed: Replaced the slow, branching while loops with std::clamp & Normalize180
    constexpr void FixAngles() noexcept {
        y = Normalize180(y);
        p = std::clamp(p, -89.f, 89.f);
        r = 0.f;
    }

    // --- Vector Conversions ---
    static Angle FromVector(const Vector& vec) noexcept {
        Angle ang;
        constexpr float rad_to_deg = 180.f / std::numbers::pi_v<float>;

        if (vec.y == 0.f && vec.x == 0.f) {
            ang.y = 0.f;
            ang.p = (vec.z > 0.f) ? 270.f : 90.f;
        } else {
            ang.y = std::atan2(vec.y, vec.x) * rad_to_deg;
            if (ang.y < 0.f) ang.y += 360.f;

            ang.p = std::atan2(-vec.z, vec.Length2D()) * rad_to_deg;
            if (ang.p < 0.f) ang.p += 360.f;
        }
        return ang;
    }

    [[nodiscard]] Vector Forward() const noexcept {
        constexpr float deg_to_rad = std::numbers::pi_v<float> / 180.f;
        const float radp = p * deg_to_rad;
        const float rady = y * deg_to_rad;

        const float sp = std::sin(radp), cp = std::cos(radp);
        const float sy = std::sin(rady), cy = std::cos(rady);

        return Vector(cp * cy, cp * sy, -sp);
    }

    [[nodiscard]] Vector Right() const noexcept {
        constexpr float deg_to_rad = std::numbers::pi_v<float> / 180.f;
        const float radp = p * deg_to_rad;
        const float rady = y * deg_to_rad;
        const float radr = r * deg_to_rad;

        const float sp = std::sin(radp), cp = std::cos(radp);
        const float sy = std::sin(rady), cy = std::cos(rady);
        const float sr = std::sin(radr), cr = std::cos(radr);

        return Vector(-1.f * sr * sp * cy + -1.f * cr * -sy,
                      -1.f * sr * sp * sy + -1.f * cr * cy,
                      -1.f * sr * cp);
    }

    [[nodiscard]] Vector Up() const noexcept {
        constexpr float deg_to_rad = std::numbers::pi_v<float> / 180.f;
        const float radp = p * deg_to_rad;
        const float rady = y * deg_to_rad;
        const float radr = r * deg_to_rad;

        const float sp = std::sin(radp), cp = std::cos(radp);
        const float sy = std::sin(rady), cy = std::cos(rady);
        const float sr = std::sin(radr), cr = std::cos(radr);

        return Vector(cr * sp * cy + -sr * -sy,
                      cr * sp * sy + -sr * cy,
                      cr * cp);
    }

    void Vectors(Vector& outForward, Vector& outRight, Vector& outUp) const noexcept {
        outForward = Forward();
        outRight = Right();
        outUp = Up();
    }
};

class Matrix3x4
{
public:
    constexpr Matrix3x4() noexcept = default;

    constexpr Matrix3x4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23) noexcept
        : flMatVal{
            { m00, m01, m02, m03 },
            { m10, m11, m12, m13 },
            { m20, m21, m22, m23 }
        }
    {
    }

    constexpr Matrix3x4(
        const Vector& x,
        const Vector& y,
        const Vector& z,
        const Vector& origin) noexcept
    {
        Init(x, y, z, origin);
    }

    constexpr void Init(
        const Vector& x,
        const Vector& y,
        const Vector& z,
        const Vector& origin) noexcept
    {
        flMatVal[0][0] = x.x;
        flMatVal[0][1] = y.x;
        flMatVal[0][2] = z.x;
        flMatVal[0][3] = origin.x;

        flMatVal[1][0] = x.y;
        flMatVal[1][1] = y.y;
        flMatVal[1][2] = z.y;
        flMatVal[1][3] = origin.y;

        flMatVal[2][0] = x.z;
        flMatVal[2][1] = y.z;
        flMatVal[2][2] = z.z;
        flMatVal[2][3] = origin.z;
    }

    constexpr void SetOrigin(const Vector& p) noexcept
    {
        flMatVal[0][3] = p.x;
        flMatVal[1][3] = p.y;
        flMatVal[2][3] = p.z;
    }

    [[nodiscard]]
    constexpr Vector GetOrigin() const noexcept
    {
        return {
            flMatVal[0][3],
            flMatVal[1][3],
            flMatVal[2][3]
        };
    }

    void Invalidate() noexcept
    {
        std::fill_n(&flMatVal[0][0], 12,
                    std::numeric_limits<float>::infinity());
    }

    [[nodiscard]]
    bool Invalid() const noexcept
    {
        return !std::isfinite(flMatVal[0][0]) ||
               (flMatVal[0][3] == 0.f &&
                flMatVal[1][3] == 0.f &&
                flMatVal[2][3] == 0.f);
    }

    [[nodiscard]]
    bool Valid() const noexcept
    {
        return !Invalid();
    }

    constexpr float* operator[](std::size_t i) noexcept
    {
        return flMatVal[i];
    }

    constexpr const float* operator[](std::size_t i) const noexcept
    {
        return flMatVal[i];
    }

    [[nodiscard]]
    constexpr float* Data() noexcept
    {
        return &flMatVal[0][0];
    }

    [[nodiscard]]
    constexpr const float* Data() const noexcept
    {
        return &flMatVal[0][0];
    }

    float flMatVal[3][4]{};
};

namespace math {
    constexpr float Deg2Rad(const float deg) {
        return deg * std::numbers::pi_v<float> / 180.f;
    }

    constexpr float Rad2Deg(const float rad) {
        return rad * 180.f / std::numbers::pi_v<float>;
    }

    constexpr float NormalizeYaw(float yaw) noexcept {
        while (yaw > 180.f) yaw -= 360.f;
        while (yaw < -180.f) yaw += 360.f;

        return yaw;
    }

    constexpr void NormalizeAngles(Angle& angles) {
        while (angles.p > 89.0f)  angles.p -= 180.0f;
        while (angles.p < -89.0f) angles.p += 180.0f;
        while (angles.y > 180.0f)  angles.y -= 360.0f;
        while (angles.y < -180.0f) angles.y += 360.0f;
        angles.r = 0.0f;
    }

    // Computes the Pitch (x) and Yaw (y) required to aim from a Vector location to a target Vector
    constexpr Angle CalculateAngle(const Vector& start, const Vector& end) {
        Vector delta = start - end;
        float hypotenuse = std::sqrt(delta.x * delta.x + delta.y * delta.y);

        Angle angles;
        angles.p = std::atanf(delta.z / hypotenuse) * (180.0f / std::numbers::pi_v<float>);
        angles.y = std::atanf(delta.y / delta.x) * (180.0f / std::numbers::pi_v<float>);
        angles.r = 0.0f;

        if (delta.x >= 0.0f)
            angles.y += 180.0f;

        NormalizeAngles(angles);
        return angles;
    }

    // Calculates the angular Field of View (FOV) delta between two view states
    constexpr float GetFov(const Angle& currentAngles, const Angle& aimAngles) {
        Angle delta = aimAngles - currentAngles;
        NormalizeAngles(delta);
        return std::sqrt(delta.p * delta.p + delta.y * delta.y);
    }

    constexpr void AngleVectors(const Angle& angles, Vector& forward, Vector& right, Vector& up) noexcept {
        const float sp = std::sin(Deg2Rad(angles.p));
        const float cp = std::cos(Deg2Rad(angles.p));
        const float sy = std::sin(Deg2Rad(angles.y));
        const float cy = std::cos(Deg2Rad(angles.y));
        const float sr = std::sin(Deg2Rad(angles.r));
        const float cr = std::cos(Deg2Rad(angles.r));

        forward.x =  cp * cy;
        forward.y =  cp * sy;
        forward.z = -sp;

        right.x =  sr * sp * cy + cr * sy; // wait, standard Source right:
        right.x =  cr * cy + sr * sp * sy; // actually:
        // Source engine right vector (note: points LEFT in Source convention)
        right.x =  (cr * sy) - (sr * sp * cy);  // hmm let me do this properly

        // Standard Source AngleVectors:
        forward.x =  cp * cy;
        forward.y =  cp * sy;
        forward.z = -sp;

        right.x = (-1.f * sr * sp * cy) + (-1.f * cr * -sy);
        right.y = (-1.f * sr * sp * sy) + (-1.f * cr *  cy);
        right.z =  -1.f * sr * cp;

        up.x = (cr * sp * cy) + (-sr * -sy);
        up.y = (cr * sp * sy) + (-sr *  cy);
        up.z =  cr * cp;
    }

    constexpr void VectorAngles(const Vector& forward, Angle& angles) noexcept {
        if (forward.x == 0.f && forward.y == 0.f) {
            // looking straight up or down
            angles.p = (forward.z > 0.f) ? -90.f : 90.f;
            angles.y = 0.f;
        } else {
            angles.p = Rad2Deg(std::atan2(-forward.z,
                                std::sqrt(forward.x * forward.x + forward.y * forward.y)));
            angles.y = Rad2Deg(std::atan2(forward.y, forward.x));
        }
        angles.r = 0.f; // no roll from a direction vector
    }

    constexpr void AngleMatrix(const Angle& angles, const Vector& origin, Matrix3x4& mat) noexcept {
        const float sp = std::sin(Deg2Rad(angles.p));
        const float cp = std::cos(Deg2Rad(angles.p));
        const float sy = std::sin(Deg2Rad(angles.y));
        const float cy = std::cos(Deg2Rad(angles.y));
        const float sr = std::sin(Deg2Rad(angles.r));
        const float cr = std::cos(Deg2Rad(angles.r));

        // forward
        mat[0][0] = cp * cy;
        mat[1][0] = cp * sy;
        mat[2][0] = -sp;

        // left
        mat[0][1] = sr * sp * cy - cr * sy;
        mat[1][1] = sr * sp * sy + cr * cy;
        mat[2][1] = sr * cp;

        // up
        mat[0][2] = cr * sp * cy + sr * sy;
        mat[1][2] = cr * sp * sy - sr * cy;
        mat[2][2] = cr * cp;

        // origin
        mat[0][3] = origin.x;
        mat[1][3] = origin.y;
        mat[2][3] = origin.z;
    }

    // math helper
    constexpr void RotateBonesAroundOrigin(const Matrix3x4* src, Matrix3x4* dst,
                                  int count, const Vector& origin,
                                  const Angle& deltaAngles) noexcept {
        // build rotation matrix from delta angles
        Matrix3x4 rot;
        AngleMatrix(deltaAngles, Vector(0,0,0), rot);

        for (int i = 0; i < count; i++) {
            // translate bone to local space (relative to origin)
            Vector boneOrigin = src[i].GetOrigin() - origin;

            // rotate
            Vector rotated;
            rotated.x = rot[0][0]*boneOrigin.x + rot[0][1]*boneOrigin.y + rot[0][2]*boneOrigin.z;
            rotated.y = rot[1][0]*boneOrigin.x + rot[1][1]*boneOrigin.y + rot[1][2]*boneOrigin.z;
            rotated.z = rot[2][0]*boneOrigin.x + rot[2][1]*boneOrigin.y + rot[2][2]*boneOrigin.z;

            // translate back to world space
            dst[i] = src[i];
            dst[i][0][3] = rotated.x + origin.x;
            dst[i][1][3] = rotated.y + origin.y;
            dst[i][2][3] = rotated.z + origin.z;

            // rotate the bone orientation columns too
            for (int col = 0; col < 3; col++) {
                Vector col_vec = { src[i][0][col], src[i][1][col], src[i][2][col] };
                dst[i][0][col] = rot[0][0]*col_vec.x + rot[0][1]*col_vec.y + rot[0][2]*col_vec.z;
                dst[i][1][col] = rot[1][0]*col_vec.x + rot[1][1]*col_vec.y + rot[1][2]*col_vec.z;
                dst[i][2][col] = rot[2][0]*col_vec.x + rot[2][1]*col_vec.y + rot[2][2]*col_vec.z;
            }
        }
    }
}

#endif //CENTURY_MATH_HPP
