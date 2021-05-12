#ifndef AABB_H
#define AABB_H

#include "utility.h"

class aabb {
    public:
        point3 minimum, maximum;

    public:
        aabb() {}
        aabb(const point3 &a, const point3 &b) {
            minimum = a;
            maximum = b;
        }

        point3 min() const {
            return minimum;
        }

        point3 max() const {
            return maximum;
        }

        bool hit(const ray &r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
                auto invD = 1.0f / r.direction()[a];

                auto t0 = (minimum[a] - r.origin()[a]) * invD;
                auto t1 = (maximum[a] - r.origin()[a]) * invD;

                if (invD < 0.0f)
                    std::swap(t0, t1);

                t_min = t0 > t_min ? t0 : t_min;
                t_max = t1 < t_max ? t1 : t_max;

                if (t_max <= t_min)
                    return false;
            }
            return true;
        }    
};

aabb surrounding_box(aabb box_0, aabb box_1) {
    point3 small(
        fmin(box_0.min().x(), box_1.min().x()),
        fmin(box_0.min().y(), box_1.min().y()),
        fmin(box_0.min().z(), box_1.min().z())
    );

    point3 big(
        fmin(box_0.max().x(), box_1.max().x()),
        fmin(box_0.max().y(), box_1.max().y()),
        fmin(box_0.max().z(), box_1.max().z())
    );

    return aabb(small, big);
}

#endif // AABB_H