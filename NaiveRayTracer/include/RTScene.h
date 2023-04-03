/**************************************************

*****************************************************/
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_normalized_axis.hpp>
#include <math.h>

#include <memory>
#include <stdlib.h>
#include <vector>
#include <string>
#include <unordered_map>
#include <stack>

#include "Camera.h"
#include "SurfaceShader.h"
#include "Light.h"
#include "RTGeometry.h" // originally Geometry.h
#include "Material.h" // 
#include "RTModel.h"

#ifndef __RTSCENE_H__
#define __RTSCENE_H__

class RTNode {
public:
    std::vector<std::shared_ptr<RTNode>> childnodes;
    std::vector<glm::mat4> childtransforms;
    std::vector<std::shared_ptr<RTModel>> models;
    std::vector<glm::mat4> modeltransforms;
};

class RTScene {
public:
    std::unique_ptr<Camera> camera;
    std::unique_ptr<SurfaceShader> shader;
    // The following are containers of objects serving as the object palettes.
    // The containers store pointers so that they can also store derived class objects.
    std::unordered_map<std::string, std::shared_ptr<RTGeometry>> geometry;
    std::unordered_map<std::string, std::shared_ptr<Material>> material;
    std::unordered_map<std::string, std::shared_ptr<RTModel>> model;
    std::unordered_map<std::string, std::shared_ptr<Light>> light;
    std::vector<Triangle> triangle_soup; //list of triangles in world or camera coordinate
    // The container of nodes will be the scene graph after we connect the nodes by setting the child_nodes.
    std::unordered_map< std::string, std::shared_ptr<RTNode>> node;
    
    RTScene(){
        // the default scene graph already has one node named "world."
        node["world"] = std::make_shared<RTNode>();
    }
    
    void init( void );
	void buildTriangleSoup( void );

    // destructor
    ~RTScene(){
        // The containers of pointers own the object pointed to by the pointers.
        // All the objects should be deleted when the object palette is destructed.
        // light
        std::cout << "RTScene destructor called" << std::endl;
        // for(std::pair<std::string,Light*> entry : light ){
        //     delete entry.second;
        // }
        // // geometry
        // for(std::pair<std::string,RTGeometry*> entry : geometry ){
        //     delete entry.second;
        // }
        // // material
        // for(std::pair<std::string,Material*> entry : material ){
        //     delete entry.second;
        // }
        // // model
        // for(std::pair<std::string,RTModel*> entry : model ){
        //     delete entry.second;
        // }
        // // model
        // for(std::pair<std::string,RTNode*> entry : node ){
        //     delete entry.second;
        // }
        // delete camera;
        // delete shader;
    }
};

#endif 
