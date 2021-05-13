#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "aabb.h"
#include "hittable.h"

class moving_sphere : public hittable {
    public:
        point3 center_0, center_1;
        double time_0, time_1;
        double radius;
        shared_ptr<material> mat_ptr;

    public:
        moving_sphere() {}
        moving_sphere(
            point3 cen0, point3 cen1, 
            double _time0, double _time1, 
            double r, 
            shared_ptr<material> m
        ) {
            center_0 = cen0;
            center_1 = cen1;

            time_0 = _time0;
            time_1 = _time1;

            radius = r;

            mat_ptr = m;
        }

        virtual bool hit(
            const ray &r, 
            double t_min, double t_max, 
            hit_record &rec
        ) const override {
            vec3 oc = r.origin() - center(r.time());

            auto a = r.direction().length_squared();
            auto half_b = dot(oc, r.direction());
            auto c = oc.length_squared() - radius*radius;

            auto discriminant = half_b*half_b - a*c;
            if (discriminant < 0) return false;
            auto sqrtd = sqrt(discriminant);

            // Find the nearest root that lies in the acceptable range.
            auto root = (-half_b - sqrtd) / a;
            if (root < t_min || t_max < root) {
                root = (-half_b + sqrtd) / a;
                if (root < t_min || t_max < root)
                    return false;
            }

            rec.t = root;
            rec.p = r.at(rec.t);
            
            auto outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            
            rec.mat_ptr = mat_ptr;

            return true;
        }

        point3 center(double time) const {
            return center_0 + ((time - time_0) / (time_1 - time_0)) * (center_1 - center_0);
        }

        virtual bool bounding_box(double _time_0, double _time_1, aabb &output_box) const override {
            aabb box_0(
                center(_time_0) - vec3(radius, radius, radius),
                center(_time_0) + vec3(radius, radius, radius)
            );

            aabb box_1(
                center(_time_1) - vec3(radius, radius, radius),
                center(_time_1) + vec3(radius, radius, radius)
            );

            output_box = surrounding_box(box_0, box_1);

            return true;
        }
    
};

#endif // MOVING_SPHERE_H