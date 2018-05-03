#pragma once
#include <array>
namespace my {
    class Vec3f {
        public:
            static constexpr int dimension = 3;
            using value_type = float;

            Vec3f(float x = 0, float y = 0, float z = 0);

            float& operator[](const int i); // to write
            const float& operator[](const int i) const; // to read

            bool operator==(const Vec3f rhs) const;
            bool operator!=(const Vec3f rhs) const;

            Vec3f& operator+=(const Vec3f& rhs);
            Vec3f operator+(const Vec3f& rhs) const;
            Vec3f operator-() const;

        private:
            std::array<float, 3> a_;
    };
    float dot(const Vec3f lhs, const Vec3f rhs);
}