#ifndef AABB_H
#define AABB_H

#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <type_traits>

class aabb {
  public:
    interval x, y, z;

    aabb() {}

    aabb(const interval &ix, const interval &iy, const interval &iz)
        : x(ix), y(iy), z(iz) {}

    aabb(const point3 &a, const point3 &b) {
        x = interval(fmin(a[0], b[0]), fmax(a[0], b[0]));
        y = interval(fmin(a[1], b[1]), fmax(a[1], b[1]));
        z = interval(fmin(a[2], b[2]), fmax(a[2], b[2]));
    }

    aabb(const aabb &box0, const aabb &box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    aabb pad() {
        double delta = 0.0001;
        interval new_x = (x.size() >= delta) ? x : x.expand(delta);
        interval new_y = (y.size() >= delta) ? y : y.expand(delta);
        interval new_z = (z.size() >= delta) ? z : z.expand(delta);

        return aabb(new_x, new_y, new_z);
    }

    const interval &axis(int n) const {
        if (n == 1)
            return y;
        if (n == 2)
            return z;
        return x;
    }

    bool hit(const ray &r, interval ray_t) const {
        for (int n = 0; n < 3; n++) {
            auto invD = 1 / r.direction()[n];
            auto orig = r.origin()[n];

            auto t0 = (axis(n).min - orig) * invD;
            auto t1 = (axis(n).max - orig) * invD;

            if (invD < 0) {
                std::swap(t0, t1);
            }

            if (t0 > ray_t.min)
                ray_t.min = t0;
            if (t1 < ray_t.max)
                ray_t.max = t1;

            if (ray_t.max <= ray_t.min) {
                return false;
            }
        }
        return true;
    }
};

#endif
