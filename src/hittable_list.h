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

private:
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

#endif //RAYTRACING_HITTABLE_LIST_H
