#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "rtweekend.hpp"

class material;

class hit_record {
  public:
    point3 p;
    vec3 normal;
    shared_ptr<material> material;
    double t;
    double u;
    double v;
    bool front_face;

    void set_face_normal(const ray &r, const vec3 &outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray &r, interval ray_t, hit_record &rec) const = 0;

    virtual aabb bounding_box() const = 0;
};

class translate : public hittable {
  public:
    translate(shared_ptr<hittable> p, const vec3 &displacement)
        : object(p), offset(displacement) {
        bbox = object->bounding_box() + offset;
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override {
        ray offset_r(r.origin() - offset, r.direction(), r.time());

        if (!object->hit(offset_r, ray_t, rec)) {
            return false;
        }

        rec.p += offset;

        return true;
    }

    aabb bounding_box() const override { return bbox; }

  private:
    shared_ptr<hittable> object;
    vec3 offset;
    aabb bbox;
};

#endif
