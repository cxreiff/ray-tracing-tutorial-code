#ifndef QUAD_H
#define QUAD_H

#include "hittable.hpp"
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

#endif
