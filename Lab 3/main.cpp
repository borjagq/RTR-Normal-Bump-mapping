/**
 * @file main.cpp
 * @brief Main aladdin 3d file.
 * @version 1.0.0 (2022-10-21)
 * @date 2022-10-21
 * @author Borja García Quiroga <garcaqub@tcd.ie>
 *
 *
 * Copyright (c) Borja García Quiroga, All Rights Reserved.
 *
 * The information and material provided below was developed as partial
 * requirements for the MSc in Computer Science at Trinity College Dublin,
 * Ireland.
 */

#define GLM_ENABLE_EXPERIMENTAL

#include "main.h"

#include <math.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/common.hpp"
#include "glm/gtx/string_cast.hpp"

#include "classes/camera/camera.h"
#include "classes/cubemap/cubemap.h"
#include "classes/light/light.h"
#include "classes/object/object.h"
#include "classes/shader/shader.h"
#include "classes/skybox/skybox.h"
#include "structs/bounding_box/bounding_box.h"

void clean() {

	// Delete all the shaders.
	for (size_t i = 0; i < shaders.size(); i++)
		shaders[i].remove();
    
    // Terminate ImGUI.
    ImGui_ImplGlfwGL3_Shutdown();
    
    // Close GL context and any other GLFW resources.
    glfwTerminate();

}

void clear() {
    
    // Specify the color of the background
    glClearColor(background.x, background.y, background.z, background.w);

    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
}

void displayElements() {
    
    // Get the program internal time.
    auto current_time = std::chrono::system_clock::now();
    double real_time = std::chrono::duration<double>(current_time.time_since_epoch()).count();
    internal_time = real_time - time_start;
    
    // Get info from the model.
    bgq_opengl::BoundingBox bb = objects[current_object].getBoundingBox();
    glm::vec3 centre = (bb.min + bb.max) / 2.0f;
    glm::vec3 size = bb.max - bb.min;
    float max_dim = std::max(size.x, std::max(size.y, size.z));
    float scale_rat = NORM_SIZE / max_dim;
    
    for (int i = 1; i < shaders.size(); i++) {
        
        // Pass the parameters to the shaders.
        shaders[i].activate();
        shaders[i].passLight(scene_light);
        
        objects[current_object].resetTransforms();
        
        // Rotate
        objects[current_object].rotate(0.0, 1.0, 0.0, internal_time * 20.0);
        objects[current_object].rotate(0.0, 1.0, 0.0, 360.0 / (shaders.size() - 1) * i);
        
        // Center the object and get it in the right position.
        objects[current_object].translate(0.0f, 0.0f, 1.0f);
        
        // Resize it to normalize it.
        objects[current_object].scale(scale_rat, scale_rat, scale_rat);
        
        // Center the object and get it in the right position.
        objects[current_object].translate(-centre.x, -centre.y, -centre.z);
        
        // Pass variables to the shaders.
        shaders[i].passFloat("lightPower", 10.0f);
        shaders[i].passFloat("minAmbientLight", 0.5f);
        shaders[i].passFloat("materialShininess", 0.5f);
        shaders[i].passFloat("coordMult", coord_multiplier);
        shaders[i].passFloat("bumpMult", bump_multiplier);
        
        // Pass the textures.
        shaders[i].passTexture(base_colors[current_texture]);
        shaders[i].passTexture(bump_maps[current_texture]);
        shaders[i].passTexture(normal_maps[current_texture]);

        // Draw the object.
        objects[current_object].draw(shaders[i], cameras[current_camera]);
        
    }
    
    // Print the skybox.
    skyboxes[0].draw(shaders[0], cameras[current_camera]);
        
}

void displayGUI() {
    
    // Init ImGUI for rendering.
    ImGui_ImplGlfwGL3_NewFrame();
    
    // Begin the new widget.
    ImGui::Begin("Parameters");
    
    // Pass these values to the shaders.
    ImGui::Text("Object");
    ImGui::RadioButton("Torus", &current_object, 0);
    ImGui::RadioButton("Sphere", &current_object, 1);
    ImGui::RadioButton("Glass", &current_object, 2);
    
    // Pass these values to the shaders.
    ImGui::Text("Texture");
    ImGui::RadioButton("Bricks", &current_texture, 0);
    ImGui::RadioButton("Foam", &current_texture, 1);
    ImGui::RadioButton("Rock", &current_texture, 2);
    ImGui::RadioButton("Tiles", &current_texture, 3);
    
    ImGui::SliderFloat("Size", &coord_multiplier, 0.01, 10.0);
    ImGui::SliderFloat("Bump", &bump_multiplier, 0.01, 20.0);

    ImGui::End();
    
    // Render ImGUI.
    ImGui::Render();
    
}

void handleKeyEvents() {
    
    // Key W will move camera 0 forward.
    if (ImGui::IsKeyPressed('W')) {

        // Move forward.
        cameras[current_camera].moveFront();
        
    }

    // Key S will move camera 0 backwards.
    if (ImGui::IsKeyPressed('S')) {

        // Move backwards.
        cameras[current_camera].moveBack();

    }

    // Key D will move camera 0 to the right.
    if (ImGui::IsKeyPressed('D')) {

        // Move right.
        cameras[current_camera].moveRight();

    }

    // Key A will move camera 0 to the right.
    if (ImGui::IsKeyPressed('A')) {

        // Move to the left.
        cameras[current_camera].moveLeft();

    }

    // Key R will move camera up.
    if (ImGui::IsKeyPressed('R')) {

        // Move up.
        cameras[current_camera].moveUp();

    }

    // Key F will move camera down.
    if (ImGui::IsKeyPressed('F')) {

        // Move up.
        cameras[current_camera].moveDown();

    }

    // Key E will rotate camera left.
    if (ImGui::IsKeyPressed('E')) {

        // Move up.
        cameras[current_camera].rotateLeft();

    }

    // Key Q will rotate the camera right.
    if (ImGui::IsKeyPressed('Q')) {

        // Move up.
        cameras[current_camera].rotateRight();

    }

    // Key T will bring the camera up.
    if (ImGui::IsKeyPressed('T')) {

        // Move up.
        cameras[current_camera].rotateUp();

    }

    // Key G will bring the camera down.
    if (ImGui::IsKeyPressed('G')) {

        // Move up.
        cameras[current_camera].rotateDown();

    }
    
}

void initElements() {
    
	// Create a white light in the center of the world.
    scene_light = bgq_opengl::Light(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    
    // Load the skyboxes.
    std::vector<std::string> faces {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };
    
    // Load the textures.
    bgq_opengl::Cubemap skycubemap(faces, "skybox", 1);
    bgq_opengl::Skybox skybox(skycubemap);
    skyboxes.push_back(skybox);
    
    bgq_opengl::Shader sky_shader("skybox.vert", "skybox.frag");
    shaders.push_back(sky_shader);
    
	// Get the shaders.
    bgq_opengl::Shader blinn_phong("blinn_phong.vert", "blinn_phong.frag");
    shaders.push_back(blinn_phong);
    
	bgq_opengl::Shader bump_map("bump_map.vert", "bump_map.frag");
    shaders.push_back(bump_map);
    
    bgq_opengl::Shader normal_map("normal_map.vert", "normal_map.frag");
    shaders.push_back(normal_map);
    
	// Creates the first camera object
    bgq_opengl::Camera camera(glm::vec3(0.0f, 0.75f, 3.0f), glm::vec3(0.0f, -0.25f, -1.0f), 45.0f, 0.1f, 300.0f, WINDOW_WIDTH, WINDOW_HEIGHT);
	cameras.push_back(camera);
    
    // Load the textures.
    base_colors.push_back(bgq_opengl::Texture("bricks_color.png", "baseColor", 2));
    base_colors.push_back(bgq_opengl::Texture("foam_color.png", "baseColor", 2));
    base_colors.push_back(bgq_opengl::Texture("rock_color.png", "baseColor", 2));
    base_colors.push_back(bgq_opengl::Texture("tiles_color.png", "baseColor", 2));

    bump_maps.push_back(bgq_opengl::Texture("bricks_bump.png", "bumpMap", 3));
    bump_maps.push_back(bgq_opengl::Texture("foam_bump.png", "bumpMap", 3));
    bump_maps.push_back(bgq_opengl::Texture("rock_bump.png", "bumpMap", 3));
    bump_maps.push_back(bgq_opengl::Texture("tiles_bump.png", "bumpMap", 3));

    normal_maps.push_back(bgq_opengl::Texture("bricks_normal.png", "normalMap", 4));
    normal_maps.push_back(bgq_opengl::Texture("foam_normals.png", "normalMap", 4));
    normal_maps.push_back(bgq_opengl::Texture("rock_normals.png", "normalMap", 4));
    normal_maps.push_back(bgq_opengl::Texture("tiles_normals.png", "normalMap", 4));

    // Load the objects.
    bgq_opengl::Object torus("torus.glb", "Assimp");
    torus.setShininess(200.0);
    objects.push_back(torus);
    
    bgq_opengl::Object sphere("sphere.glb", "Assimp");
    sphere.setShininess(200.0);
    objects.push_back(sphere);
    
    bgq_opengl::Object glass("glass.glb", "Assimp");
    glass.setShininess(200.0);
    objects.push_back(glass);
    
}

void initEnvironment(int argc, char** argv) {
    
    // start GL context and O/S window using the GLFW helper library
    if (!glfwInit()) {
        
        std::cerr << "ERROR: could not start GLFW3" << std::endl;
        exit(1);

    }
    
    // MacOS initializations.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create the window.
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME, NULL, NULL);
    if (!window) {
        
        std::cerr << "Error 121-1001 - Failed to create the window." << std::endl;
        glfwTerminate();
        exit(1);

    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLEW and OpenGL.
    GLenum res = glewInit();

    // Check for any errors.
    if (res != GLEW_OK) {

        std::cerr << "Error 121-1002 - GLEW could not be initialized:" << glewGetErrorString(res) << std::endl;
        exit(1);

    }
    
    // Get version info.
    std::cerr << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cerr << "OpenGL version supported " << glGetString(GL_VERSION) << std::endl;
    std::cerr << "GLSL version supported " << (char *) glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    // Setup ImGui binding
    ImGui_ImplGlfwGL3_Init(window, true);

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
    
    // Init the initial time.
    auto current_time = std::chrono::system_clock::now();
    time_start = std::chrono::duration<double>(current_time.time_since_epoch()).count();

}

int main(int argc, char** argv) {

	// Initialise the environment.
    initEnvironment(argc, argv);
    
	// Initialise the objects and elements.
	initElements();

	// Main loop.
    while(!glfwWindowShouldClose(window)) {
        
        // Clear the scene.
        clear();
        
        // Get the window size.
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        cameras[current_camera].setWidth(width);
        cameras[current_camera].setHeight(height);

        // Handle key events.
        handleKeyEvents();
        
        // Display the scene.
        displayElements();
        
        // Make the things to print everything.
        displayGUI();
        
        // Update other events like input handling.
        glfwPollEvents();
        glfwSwapBuffers(window);
        
    }

	// Clean everything and terminate.
	clean();

}
