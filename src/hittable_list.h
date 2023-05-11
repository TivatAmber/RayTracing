//
// Created by Stellaris on 2023/5/8.
//

#ifndef RAYTRACING_HITTABLE_LIST_H
#define RAYTRACING_HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list: public hittable {
public:
    hittable_list() {}
    hittable_list(std::shared_ptr<hittable> obj) {add(obj);}

    void clear() {objects.clear();}
    void add(std::shared_ptr<hittable> obj) {objects.push_back(obj);}

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

public:
    std::vector< std::shared_ptr<hittable> > objects;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for (const auto& obj : objects) {
        if (obj->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1, aabb &output_box) const {
    if (objects.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for (const auto& obj : objects) {
        if (!obj->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(temp_box, output_box);
        first_box = false;
    }

    return true;
}

#endif //RAYTRACING_HITTABLE_LIST_H
