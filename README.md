# Ray Tracing Project 

Author: Yumeng Shi  

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

## 🛠️ Project Screen shot
- Read [Report](https://github.com/Jadeshi1998/Computer-Graphics-RayTracing/blob/main/report/COSC363-Assessment2.pdf)    
  <img width="277" alt="Screenshot 2025-06-17 at 08 08 49" src="https://github.com/user-attachments/assets/6a6c2fb1-70b0-4bfb-9a56-1d35d3aaad01" />
- Anti Alising Applied    
  <img width="656" alt="Screenshot 2025-06-17 at 08 09 30" src="https://github.com/user-attachments/assets/a5058339-3f37-4306-881c-be21d072081a" />

---

## 🖥️ How to Run

### ✅ Prerequisites

- C++ compiler supporting C++11 or newer
- OpenGL & GLUT installed
- [DevIL (OpenIL)](http://openil.sourceforge.net/) library for texture support

### ▶️ Run
- [OpenGL set up](https://github.com/Jadeshi1998/OpenGL_Install/blob/main/OpenGL_VisualStudio.pdf) Check this for how to run.

