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

