#version 330 core

in vec4 position; // raw position in the model coord
in vec3 normal;   // raw normal in the model coord

uniform mat4 modelview; // from model coord to eye coord
uniform mat4 view;      // from world coord to eye coord

// Material parameters
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform vec4 emision;
uniform float shininess;

// Light source parameters
const int maximal_allowed_lights = 10;
uniform bool enablelighting;
uniform int nlights;

uniform vec4 lightpositions[ maximal_allowed_lights ];
// Lj
uniform vec4 lightcolors[ maximal_allowed_lights ];

// Output the frag color
out vec4 fragColor;


void main (void){
    if (!enablelighting){
        // Default normal coloring (you don't need to modify anything here)
        vec3 N = normalize(normal);
        fragColor = vec4(0.5f*N + 0.5f , 1.0f);
    } else {
        
        // HW3: You will compute the lighting here.
        fragColor = emision;
        
        vec4 tmp = vec4(0,0,0,0);
        // calculate "n"
        mat3 A = mat3(0.0f);
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                A[j][k] = modelview[j][k];
            }
        }
        A = transpose(inverse(A));
        vec3 n = normalize(A * normal);
        vec4 p_cam = modelview * position;
        vec4 eye_pos = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        // calculate "v"
        vec4 v_tmp = normalize(eye_pos - p_cam);
        vec3 v = v_tmp.xyz;
        for (int i = 0; i < maximal_allowed_lights; i++) {
            vec4 Lj = lightcolors[i];

            // calculate "l"
            vec4 q_world = lightpositions[i];
            vec4 q_cam = view * q_world;
            vec3 l = normalize(p_cam.w * q_cam.xyz - q_cam.w * p_cam.xyz);

            // calculate "h"
            vec3 h = normalize(v + l);  

            tmp += (ambient + diffuse * max(dot(n, l), 0) + specular * pow(max(dot(n, h), 0), shininess)) * Lj;
        }
        fragColor += tmp;
    }
}
