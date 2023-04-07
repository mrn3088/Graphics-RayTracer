/**
 * @brief 
 * File: main.cpp
 * Authors: Xiu Yuan, Ruinan Ma
 * Description: This file contains the main function 
 * for a program that displays a 3D scene and uses a ray tracer 
 * to render the scene.
 */


#include <stdlib.h>
#include <iostream>
// OSX systems need their own headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
// Use of degrees is deprecated. Use radians for GLM functions
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "Screenshot.h"
#include "Image.h"
#include "RayTracer.h"


static const int width = 800; 
static const int height = 600; 
static const char* title = "Scene viewer";
static const glm::vec4 background(0.1f, 0.2f, 1.0f, 1.0f);
static RTScene scene;
static Image image(width, height);

#include "hw3AutoScreenshots.h"

void printHelp(){
    std::cout << R"(
    Available commands:
      press 'H' to print this message again.
      press Esc to quit.
      press 'O' to save a screenshot.
      press the arrow keys to rotate camera. (Very slow for ray tracer)
      press 'A'/'Z' to zoom. (Very slow for ray tracer)
      press 'R' to reset camera.         
)";
}

void set_scene_camera(float zoom, float angle_right, float angle_up){
    scene.camera -> zoom(zoom);
    scene.camera -> rotateRight(angle_right);
    scene.camera -> rotateUp(angle_up);
}

void initialize(void){
    std::cout << "initializing" << std::endl;
    printHelp();
    glClearColor(background[0], background[1], background[2], background[3]); // background color
    glViewport(0,0,width,height);

    // initialize image
    image.init();
    // initialize scene
    scene.init();
    set_scene_camera(0.25f, 20.0f, 10.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    std::cout << "initialized done!" << std::endl;
}

void display(void){
    // re-ray tracing
    scene.buildTriangleSoup();
    RayTracer::Raytrace(*(scene.camera), &scene, image);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // draw image 
    image.draw();
    glutSwapBuffers();
    glFlush();
}

void saveScreenShot(const char* filename = "test.png"){
    int currentwidth = glutGet(GLUT_WINDOW_WIDTH);
    int currentheight = glutGet(GLUT_WINDOW_HEIGHT);
    Screenshot imag = Screenshot(currentwidth,currentheight);
    imag.save(filename);
}

void keyboard(unsigned char key, int x, int y){
    std::cout << "detecting key " << key << std::endl;
    switch(key){
        case 27: // Escape to quit
            exit(0);
            break;
        case 'h': // print help
            printHelp();
            break;
        case 'o': // save screenshot
            saveScreenShot();
            break;
        case 'r':
            std::cout << "reset" << std::endl;
            scene.camera -> aspect_default = float(glutGet(GLUT_WINDOW_WIDTH))/float(glutGet(GLUT_WINDOW_HEIGHT));
            scene.camera -> resetCamera();
            glutPostRedisplay();
            break;
        case 'a':
            std::cout << "zoom in" << std::endl;
            scene.camera -> zoom(0.9f);
            std::cout << scene.camera -> eye.x << std::endl;
            glutPostRedisplay();
            break;
        case 'z':
            std::cout << "zoom out" << std::endl;
            scene.camera -> zoom(1.1f);
            glutPostRedisplay();
            break;
        case ' ':
            hw3AutoScreenshots();
            glutPostRedisplay();
            break;
        default:
            glutPostRedisplay();
            break;
    }
}

void specialKey(int key, int x, int y){
    std::cout << "detecting special key " << key << std::endl;
    switch (key) {
        case GLUT_KEY_UP: // up
            std::cout << "up" << std::endl;
            scene.camera -> rotateUp(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_DOWN: // down
            std::cout << "down" << std::endl;
            scene.camera -> rotateUp(10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_RIGHT: // right
            std::cout << "right" << std::endl;
            scene.camera -> rotateRight(-10.0f);
            glutPostRedisplay();
            break;
        case GLUT_KEY_LEFT: // left
            std::cout << "left" << std::endl;
            scene.camera -> rotateRight(10.0f);
            glutPostRedisplay();
            break;
    }
}

int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    
#ifdef __APPLE__
    glutInitDisplayMode( GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#else
    glutInitContextVersion(3,1);
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
#endif
    glutInitWindowSize(width, height);
    glutCreateWindow(title);
#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    GLenum err = glewInit() ;
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
    }
#endif
    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    initialize();
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKey);
    
    std::cout << "Entering main loop" << std::endl;

    glutMainLoop();
	return 0;
}