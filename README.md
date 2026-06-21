# Rendering Portals in OpenGL

A 3D rendering engine built with C++ and OpenGL that implements a seamless recursive portal effect. This engine was developed as an academic dissertation project and demonstrates advanced rendering techniques to connect separate spaces in 3D environments.

## Features

* **Recursive Portal Rendering:** Utilizes the stencil buffer and a recursive render loop to accurately mask and draw views through multiple portal iterations.
* **Oblique Frustum Culling:** Modifies the projection matrix to cleanly clip geometry behind the portal surface, preventing near-plane artifacts and z-fighting.
* **Custom Model Loader:** Loads complex 3D `.obj` models (such as the teapot, floor prototype, and dust2 layout) directly into the scene.
* **Skybox Rendering:** Surrounds the scene with an environment map to provide a continuous background behind the 3D geometry.
* **Interactive First-Person Camera:** Navigate the scene using standard WASD keys and mouse look (hold `Tab` to sprint).

## Setup Guide

### Prerequisites
* **Visual Studio 2022**

### Installation & Building
1. Clone the repository:
   ```bash
   git clone [https://github.com/8joseph/opengl-portals.git](https://github.com/8joseph/opengl-portals.git)
   ```
2. Open the .sln solution file in Visual Studio 2022.
3. Ensure your build configuration is set to x64 (both Debug and Release modes are fully supported).
4. Build and run the project.
