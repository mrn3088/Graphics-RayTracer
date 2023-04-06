#ifndef HITTABLE_hpp
#define HITTABLE_hpp

#include "rtweekend.hpp"
#include "hit_record.hpp"

class hittable
{
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

#endif