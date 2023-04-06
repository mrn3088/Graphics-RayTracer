/**************************************************
RTScene.cpp contains the implementation of the draw command
*****************************************************/
#include "RTScene.h"
#include "RTCube.h"
#include "RTObj.h"

// The scene init definition 
#include "RTScene.inl"


using namespace glm;
void RTScene::buildTriangleSoup(void){
    camera -> computeMatrices();
    // Define stacks for depth-first search (DFS)
    std::stack <std::shared_ptr<RTNode>> dfs_stack;
    std::stack <mat4>  matrix_stack; // HW3: You will update this matrix_stack during the depth-first search while loop.
    
    // Initialize the current state variable for DFS
    auto cur = node["world"]; // root of the tree
    auto cur_VM = camera -> view; // HW3: You will update this current modelview during the depth first search.  Initially, we are at the "world" node, whose modelview matrix is just camera's view matrix.
    
    // HW3: The following is the beginning of the depth-first search algorithm.
    // HW3: The depth-first search for the node traversal has already been implemented (cur, dfs_stack).
    // HW3: All you have to do is to also update the states of (cur_VM, matrix_stack) alongside the traversal.  You will only need to modify starting from this line.
    dfs_stack.push(cur);
    /**
     * TODO: (HW3 hint: you should do something here)
     */
    matrix_stack.push(cur_VM);
    
    // Compute total number of connectivities in the graph; this would be an upper bound for
    // the stack size in the depth first search over the directed acyclic graph
    int total_number_of_edges = 0; 
    for ( const auto &n : node ) total_number_of_edges += n.second->childnodes.size();
    
    // If you want to print some statistics of your scene graph
    // std::cout << "total numb of nodes = " << node.size() << std::endl;
    // std::cout << "total number of edges = " << total_number_of_edges << std::endl;
    
    while( ! dfs_stack.empty() ){
        // Detect whether the search runs into infinite loop by checking whether the stack is longer than the number of edges in the graph.
        if ( dfs_stack.size() > total_number_of_edges ){
            std::cerr << "Error: The scene graph has a closed loop." << std::endl;
            exit(-1);
        }
        
        // top-pop the stacks
        cur = dfs_stack.top();  dfs_stack.pop();
        /**
         * TODO: (HW3 hint: you should do something here)
         */
        cur_VM = matrix_stack.top(); matrix_stack.pop();
        
        // draw all the models at the current node
        for ( size_t i = 0; i < cur -> models.size(); i++ ){
            // Prepare to draw the geometry. Assign the modelview and the material.
            
            /**
             * TODO: (HW3 hint: you should do something here)
             */
            
            mat4 curr_modelview = cur_VM * (cur -> modeltransforms[i]); // TODO: HW3: Without updating cur_VM, modelview would just be camera's view matrix.
			mat3 curr_A_transpose_inverse = inverse(transpose(mat3(curr_modelview)));
            auto curr_material  = (cur -> models[i]) -> material;
            
            // The draw command -- replace shader
            // formulas from slide "AffineGeometry"
            std::for_each(cur -> models[i] -> geometry -> elements.begin(), 
            cur -> models[i] -> geometry -> elements.end(), 
            [&](Triangle& tri){
                vec4 pos1_ori = vec4(tri.P[0], 1), pos2_ori = vec4(tri.P[1], 1), pos3_ori = vec4(tri.P[2], 1);
                vec3 norm1_ori = tri.N[0], norm2_ori = tri.N[1], norm3_ori = tri.N[2];
                vec4 pos1_trans = curr_modelview * pos1_ori, pos2_trans = curr_modelview * pos2_ori, pos3_trans = curr_modelview * pos3_ori;
                vec3 norm1_trans = curr_A_transpose_inverse * norm1_ori, norm2_trans = curr_A_transpose_inverse * norm2_ori, norm3_trans = curr_A_transpose_inverse * norm3_ori;
                std::vector<vec3> ve = {vec3(pos1_trans.x, pos1_trans.y, pos1_trans.z), vec3(pos2_trans.x, pos2_trans.y, pos2_trans.z), vec3(pos3_trans.x, pos3_trans.y, pos3_trans.z)};
                std::vector<vec3> ne = {norm1_trans, norm2_trans, norm3_trans};
                triangle_soup.emplace_back(ve, ne, curr_material);
            });
            
        }
        
        // Continue the DFS: put all the child nodes of the current node in the stack
        for ( size_t i = 0; i < cur -> childnodes.size(); i++ ){
            dfs_stack.push( cur -> childnodes[i] );
            /**
             * TODO: (HW3 hint: you should do something here)
             */
            matrix_stack.push(cur_VM * (cur -> childtransforms[i]));
        }
        
    } // End of DFS while loop.
    // HW3: Your code will only be above this line.
}