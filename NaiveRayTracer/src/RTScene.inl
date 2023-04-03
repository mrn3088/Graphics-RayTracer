/**************************************************
Scene_copy.inl contains the definition of the scene graph
*****************************************************/
#include "RTScene.h"
#include "RTCube.h"
#include "RTObj.h"

using namespace glm;
void RTScene::init(void){
    // Create a geometry palette
    geometry["cube"] = std::make_shared<RTCube>();
    geometry["teapot"] = std::make_shared<RTObj>();
    std::cout << "init cube" << std::endl;

    
    geometry["cube"] -> init();
    geometry["teapot"] -> init("models/teapot.obj");
    std::cout << "init material" << std::endl;
    
    // Create a material palette
    material["wood"] = std::make_shared<Material>();
    material["wood"] -> ambient = vec4(0.1f,0.1f,0.1f,1.0f);
    material["wood"] -> diffuse = vec4(0.3f,0.15f,0.1f,1.0f);
    material["wood"] -> specular = vec4(0.3f,0.15f,0.1f,1.0f);
    material["wood"] -> shininess = 100.0f;
    
    material["ceramic"] = std::make_shared<Material>();
    material["ceramic"] -> ambient = vec4(0.02f, 0.07f, 0.2f, 1.0f);
    material["ceramic"] -> diffuse = vec4(0.1f, 0.25f, 0.7f, 1.0f);
    material["ceramic"] -> specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["ceramic"] -> shininess = 100.0f;
 
    material["silver"] = std::make_shared<Material>();
    material["silver"] -> ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);
    material["silver"] -> diffuse = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    material["silver"] -> specular = vec4(0.9f, 0.9f, 0.9f, 1.0f);
    material["silver"] -> shininess = 50.0f;
    
    material["turquoise"] = std::make_shared<Material>();
    material["turquoise"] -> ambient = vec4(0.1f, 0.2f, 0.17f, 1.0f);
    material["turquoise"] -> diffuse = vec4(0.2f, 0.375f, 0.35f, 1.0f);
    material["turquoise"] -> specular = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    material["turquoise"] -> shininess = 100.0f;
    
    
    material["bulb"] = std::make_shared<Material>();
    material["bulb"] -> ambient = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"] -> diffuse = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    material["bulb"] -> specular = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    material["bulb"] -> emision = vec4(1.0f,0.2f,0.1f,1.0f);
    material["bulb"] -> shininess = 200.0f;
    
    // Create a model palette
    model["teapot1"] = std::make_shared<RTModel>();
    model["teapot1"] -> geometry = geometry["teapot"];
    model["teapot1"] -> material = material["turquoise"];
    model["teapot2"] = std::make_shared<RTModel>();
    model["teapot2"] -> geometry = geometry["teapot"];
    model["teapot2"] -> material = material["ceramic"];
    std::cout << "init table" << std::endl;

    model["table piece"] = std::make_shared<RTModel>();
    model["table piece"] -> geometry = geometry["cube"];
    model["table piece"] -> material = material["wood"];

    model["bulb"] = std::make_shared<RTModel>(); 
    model["bulb"] -> geometry = geometry["cube"];
    model["bulb"] -> material = material["bulb"];
    
    // Create a light palette
    light["sun"] = std::make_shared<Light>();
    light["sun"] -> position = vec4(3.0f,2.0f,1.0f,0.0f);
    light["sun"] -> color = 1.0f * vec4(1.0f,1.0f,1.0f,1.0f);
    
    light["bulb"] = std::make_shared<Light>();
    light["bulb"] -> position = vec4(0.0f,2.0f,0.0f,0.0f);
    light["bulb"] -> color = 1.5f * vec4(1.0f,0.2f,0.1f,1.0f);
    
    // Build the scene graph
    node["table"] = std::make_shared<RTNode>();
    node["table top"] = std::make_shared<RTNode>();
    node["table leg"] = std::make_shared<RTNode>();
    node["teapot1"] = std::make_shared<RTNode>();
    node["teapot2"] = std::make_shared<RTNode>();
    
    
    node["table"] -> childnodes.push_back( node["table top"] );
    node["table"] -> childtransforms.push_back( translate(vec3(0.0f,1.2f,0.0f)) );
    node["table"] -> childnodes.push_back( node["table leg"] );
    node["table"] -> childtransforms.push_back( translate(vec3(-0.9f,0.0f,-0.4f)) );
    node["table"] -> childnodes.push_back( node["table leg"] );
    node["table"] -> childtransforms.push_back( translate(vec3(-0.9f,0.0f,0.4f)) );
    node["table"] -> childnodes.push_back( node["table leg"] );
    node["table"] -> childtransforms.push_back( translate(vec3(0.9f,0.0f,0.4f)) );
    node["table"] -> childnodes.push_back( node["table leg"] );
    node["table"] -> childtransforms.push_back( translate(vec3(0.9f,0.0f,-0.4f)) );
    
    node["table leg"] -> models.push_back( model["table piece"] );
    node["table leg"] -> modeltransforms.push_back( translate(vec3(0.0f,0.5f,0.0f)) * scale(vec3(0.2f,1.0f,0.2f)) );
    
    node["table top"] -> models.push_back( model["table piece"] );
    node["table top"] -> modeltransforms.push_back( translate(vec3(0.0f,-0.1f,0.0f)) * scale(vec3(2.0f,0.2f,1.0f)) );
    node["table top"] -> childnodes.push_back( node["teapot1"] );
    node["table top"] -> childtransforms.push_back( translate(vec3(-0.5f,0.0f,0.0f)) );
    node["table top"] -> childnodes.push_back( node["teapot2"] );
    node["table top"] -> childtransforms.push_back( translate(vec3( 0.5f,0.0f,0.0f)) * rotate( -120.0f*float(M_PI)/180.0f, vec3(0.0f, 1.0f, 0.0f) ) );
    
    node["teapot1"] -> models.push_back( model["teapot1"] );
    node["teapot1"] -> modeltransforms.push_back( scale(vec3(0.5f)) );
    node["teapot2"] -> models.push_back( model["teapot2"] );
    node["teapot2"] -> modeltransforms.push_back( scale(vec3(1.0f,1.5f,1.0f)) * scale(vec3(0.5f)) );

    node["world"] -> childnodes.push_back( node["table"] );
    node["world"] -> childtransforms.push_back( mat4(1.0f) );

    node["world"] -> models.push_back( model["bulb"] );
    node["world"] -> modeltransforms.push_back( translate(vec3(0.0f,2.0f,0.0f))*scale(vec3(0.1f)) );
    
    // Put a camera
    camera = std::move(std::unique_ptr<Camera>(new Camera()));
    camera -> target_default = vec3( 0.0f, 1.0f, 0.0f );
    camera -> eye_default = vec3( 0.0f, 1.0f, 5.0f );
    camera -> up_default = vec3( 0.0f, 1.0f, 0.0f );
    camera -> resetCamera();
    
}
