#ifndef BVH_H
#define BVH_H

#include <algorithm>

#include "utility.h"
#include "hittable.h"
#include "hittable_list.h"

class bvh_node : public hittable {
    public:
        shared_ptr<hittable> left, right;

        aabb box;

    public:
        bvh_node() {}
        
        bvh_node(const hittable_list &list, double time_0, double time_1) {
            bvh_node(list.objects, 0, list.objects.size(), time_0, time_1);
        }

        bvh_node(
            std::vector<shared_ptr<hittable>> &src_objects,
            size_t start, size_t end, double time_0, double time_1
        ) {
            // Create a modifiable array of the source scene objects
            auto objects = src_objects;

            int axis = random_int(0, 2);

            auto comparator = (axis == 0) ? box_x_compare
                : (axis == 1) ? box_y_compare
                : box_z_compare;

            size_t object_span = end - start;

            if (object_span == 1) {
                left = right = objects[start];
            } else if (object_span == 2) {
                if (comparator(objects[start], objects[start + 1])) {
                    left = objects[start];
                    right = objects[start + 1];
                }
            } else {
                std::sort(
                    objects.begin() + start,
                    objects.begin() + end,
                    comparator
                );

                auto mid = start + object_span / 2;

                left = make_shared<bvh_node>(objects, start, mid, time_0, time_1);
                right = make_shared<bvh_node>(objects, mid, end, time_0, time_1);
            }

            aabb box_left, box_right;

            if (
                !left->bouding_box(time_0, time_1, box_left) or
                !right->bouding_box(time_0, time_1, box_right)
            )
                std::cerr << "No bouding box in bvh_node constructor.\n";
            
            box = surrounding_box(box_left, box_right);

        }

        virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override {
            if (!box.hit(r, t_min, t_max))
                return false;

            bool hit_left = left->hit(r, t_min, t_max);
            bool hit_right = right->hit(r, t_min, t_max);

            return hit_left or hit_right;
        }
           
        virtual bool bounding_box(double time0, double time1, aabb &output_box) const override {
            output_box = box;

            return true;
        }
};

inline bool box_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis) {
    aabb box_a, box_b;

    if (!a->bouding_box(0, 0, box_a) or !b->bouding_box(0, 0, box_b))
        std::cerr << "No bounding box in bvh_node constructor.\n";

    return box_a.min().vec[axis] < box_b.min().vec[axis];

}

bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 0);
}

bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 1);
}

bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
    return box_compare(a, b, 2);
}

#endif // BVH_H