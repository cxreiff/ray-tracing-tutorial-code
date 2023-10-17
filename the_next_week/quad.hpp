#ifndef QUAD_H
#define QUAD_H

#include "hittable.hpp"
#include "hittable_list.hpp"
#include "rtweekend.hpp"
#include "vec3.hpp"

class quad : public hittable {
  public:
    quad(
        const point3 &_Q,
        const vec3 &_u,
        const vec3 &_v,
        shared_ptr<material> _mat
    )
        : Q(_Q), u(_u), v(_v), mat(_mat) {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);
    }

    virtual void set_bounding_box() { bbox = aabb(Q, Q + u + v).pad(); }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        auto denominator = dot(normal, r.direction());

        if (fabs(denominator) < 1e-8) {
            return false;
        }

        auto t = (D - dot(normal, r.origin())) / denominator;

        if (!ray_t.contains(t)) {
            return false;
        }

        auto intersection = r.at(t);
        vec3 planar_hit_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hit_vector, v));
        auto beta = dot(w, cross(u, planar_hit_vector));

        if (!is_interior(alpha, beta, rec)) {
            return false;
        }

        rec.t = t;
        rec.p = intersection;
        rec.material = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b, hit_record &rec) const {
        if (a < 0 || 1 < a || b < 0 || 1 < b) {
            return false;
        }

        rec.u = a;
        rec.v = b;
        return true;
    }

  private:
    point3 Q;
    vec3 u, v;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
    vec3 w;
};

inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> material) {
    auto sides = make_shared<hittable_list>();

    auto min = point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
    auto max = point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

    auto dx = vec3(max.x() - min.x(), 0, 0);
    auto dy = vec3(0, max.y() - min.y(), 0);
    auto dz = vec3(0, 0, max.z() - min.z());

    sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()), dx, dy, material));
    sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz,  dy, material));
    sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx,  dy, material));
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dz,  dy, material));
    sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()),  dx, -dz, material));
    sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()),  dx,  dz, material));

    return sides;
}

#endif
