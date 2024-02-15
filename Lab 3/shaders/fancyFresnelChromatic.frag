#version 330 core

// This shader has been adapted from the code found on https://www.scss.tcd.ie/Michael.Manzke/CS7055/GLSL/GLSL-3rdEd-refraction.pdf.


in vec3 vertexPosition;             // Position from the VS.
in vec3 vertexNormal;	            // Normal from the VS.
in vec3 vertexColor;	            // Color from the VS.
in vec2 vertexUV;                   // UV coordinates from the VS.
in vec3 Reflect;
in vec3 RefractR;
in vec3 RefractG;
in vec3 RefractB;
in float Ratio;

uniform vec4 lightColor;		    // Light color.
uniform vec3 lightPos;			    // Light position.
uniform vec3 cameraPosition;	    // Position of the camera.
uniform sampler2D baseColor;        // The color texture.
uniform samplerCube skybox;         // The skybox.

uniform float etaR;                  // The ratio.
uniform float etaG;                  // The ratio.
uniform float etaB;                  // The ratio.
uniform float fresnelPower;
uniform vec3 objectColor;
uniform float mixColor;

out vec4 outColor; // Outputs color in RGBA.

void main() {
    
    vec3 refractColor = vec3(0.0, 0.0, 0.0);
    refractColor.r = vec3(texture(skybox, RefractR)).r;
    refractColor.g = vec3(texture(skybox, RefractG)).g;
    refractColor.b = vec3(texture(skybox, RefractB)).b;
    
    vec3 reflectColor = vec3(texture(skybox, Reflect));
    
    vec3 color = mix(refractColor, reflectColor, Ratio);
    
    outColor = vec4(mix(objectColor, color, mixColor), 1.0);
    
}
