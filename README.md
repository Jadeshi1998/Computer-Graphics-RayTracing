# Ray Tracing Project – COSC363 Assignment 2

Author: Yumeng Shi  
Student ID: 74341182

## 📌 Overview

This project implements a ray tracer that renders a static 3D scene containing various geometric objects and lighting effects. It is written in C++ using OpenGL and includes features like:

- Reflections and transparency
- Shadows (including soft shadows from transparent objects)
- Textured surfaces and patterned planes
- Anti-aliasing (super-sampling + adaptive subdivision)
- Spotlights, multiple light sources, and fog
- Object-space transformations (translation, scaling, rotation)
- Support for cones, cylinders, spheres, and custom geometry

---

## ✨ Key Features

- ✅ Transparent objects using alpha blending and Snell’s law for refraction
- ✅ Mirror-like plane reflection
- ✅ Adaptive anti-aliasing (2×2 sample grid with recursion)
- ✅ Patterned and textured surfaces, including a textured cylinder
- ✅ Fog based on distance blending
- ✅ Spotlights with cut-off angle
- ✅ Object-space transformations: scale, rotate, translate shapes
- ✅ Two light sources and realistic multiple shadows

---

## 🛠️ Project Structure
.
├── RayTracer.cpp # Main file – handles scene setup & rendering
├── Sphere.h/.cpp # Sphere object and intersection logic
├── Plane.h/.cpp # Planes for box walls
├── Cone.h/.cpp # Custom cone intersection logic
├── Cylinder.h/.cpp # Cylinder with cap and texture
├── SceneObject.h/.cpp # Abstract base for all scene objects
├── Ray.h # Ray struct for tracing
├── TextureOpenIL.h # Texture loading helper
├── Texture images/ # Contains image textures for objects
├── README.md # Project description and instructions


---

## 🖥️ How to Run

### ✅ Prerequisites

- C++ compiler supporting C++11 or newer
- OpenGL & GLUT installed
- [DevIL (OpenIL)](http://openil.sourceforge.net/) library for texture support

### ▶️ Run
-[OpenGL set up](https://github.com/Jadeshi1998/OpenGL_Install/blob/main/OpenGL_VisualStudio.pdf) Check this for how to run.

