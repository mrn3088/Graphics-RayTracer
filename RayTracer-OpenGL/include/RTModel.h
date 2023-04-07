/**************************************************
A model is a geometry with material.
*****************************************************/
#include "RTGeometry.h"
#include "Material.h"
#include <memory>

#ifndef __RTMODEL_H__
#define __RTMODEL_H__

struct RTModel {
    std::shared_ptr<RTGeometry> geometry;
    std::shared_ptr<Material> material;
};

#endif 
