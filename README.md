# Real-time Normal vs Bump Mapping

A visualisation and comparison of normal and bump mapping models in OpenGL and how they alter the original appearance.

## About this repo

This repository contains an OpenGL environment rendering a scene consisting of 3 objects rotating around a common axis. Each of these objects shows the results of a different shading model: simple Blinn-Phong, Blinn-Phong + Normal mapping, and Blinn-Phong + Bump mapping. The scene also contains a skybox that is irrelevant to the intended purpose. This skybox has been downloaded from the humus website.

The torus and sphere models have been created using Blender. The same program also generated normals, tangents, bitangents and UV coordinates. The third object, which represents a glass, was downloaded from UPC-BarcelonaTech’s graphics repository. Finally, all the textures, level maps and normal maps have been downloaded from the site textures.com.

## Demo video

[![Watch the video](https://img.youtube.com/vi/FO-DkjnZ-B4/0.jpg)](https://www.youtube.com/watch?v=FO-DkjnZ-B4)

## Getting Started

### Dependencies

- [Xcode 14.3.1](https://developer.apple.com/xcode/)
- OpenGL 4.1 (included in macOS)
- [Glew 2.2.0](https://glew.sourceforge.net)
- [GLFW 3.3.8](https://github.com/glfw/glfw)
- [Assimp 5.3.1](https://github.com/assimp/assimp)
- [GLM 1.0.0](https://github.com/g-truc/glm)

### Installing

1. Clone this repository.

```sh
git clone https://github.com/borjagq/RTR-Normal-Bump-mapping/
```

2. Open the project ```Lab3.xcodeproj```

3. Build and run

# License

This project is licensed under the MIT License - see the [LICENSE](https://github.com/borjagq/RTR-Normal-Bump-mapping/LICENSE/) file for details

# Acknowledgments

This repository incorporates essential components from the open-source libraries [stb](https://github.com/nothings/stb) and [Dear ImGui](https://github.com/ocornut/imgui). These libraries provide the file read/write and GUI functionalities.  For further details about these libraries, visit their GitHub repositories.
