# 🌿 Mysterious Garden Scene  

A photorealistic 3D OpenGL scene featuring an eerie garden with interactive elements, dynamic lighting, and atmospheric effects.  

---

## 📖 Table of Contents  
- [📌 Description](#-description)  
  - [💛 Motivation](#-motivation)  
  - [🎯 Why this project?](#-why-this-project)  
  - [✅ What problem does it solve?](#-what-problem-does-it-solve)  
  - [💡 What I learned](#-what-i-learned)  
- [🚀 Usage](#-usage)  
  - [🎮 Key Controls](#-key-controls)  
  - [🖼️ Scene Preview](#%EF%B8%8F-scene-preview)  
- [✨ Features](#-features)  
- [🛠️ Built With](#%EF%B8%8F-built-with)  

---

## 📌 Description  

This project creates an immersive 3D garden scene with a hidden greenhouse, animated elements, and atmospheric effects like fog and lightning. Designed to blend tranquility with subtle unease through visual storytelling.  

### 💛 Motivation  
Creating realistic 3D environments manually is time-consuming. This project automates complex effects (lighting, shadows) while offering tools for artistic expression.  

### 🎯 Why this project?  
I aimed to master **OpenGL shaders**, **lighting models**, and **real-time animations** while building an engaging, interactive scene.  

### ✅ What problem does it solve?  
It demonstrates how to efficiently render complex 3D scenes with dynamic effects, optimized for performance.  

### 💡 What I learned  
- **OpenGL/GLFW** for 3D rendering and window management.  
- **Phong Lighting Model** for directional and point lights.  
- **Shadow Mapping** for realistic dynamic shadows.  
- **Fragment Discarding** to optimize plant rendering.  
- **GLM** for matrix transformations and animations.  

---

## 🚀 Usage  

### 🎮 Key Controls  
| Key          | Action                                  |  
|--------------|-----------------------------------------|  
| **W/S**      | Move camera forward/backward           |  
| **Q/E**      | Rotate camera horizontally             |  
| **R/F**      | Rotate camera vertically               |  
| **A**        | Trigger door animation                 |  
| **J/L**      | Rotate directional light               |  
| **1/2/3**    | Switch view modes (Normal/Wireframe/Points) |  
| **P**        | Start automated scene tour             |  

### 🖼️ Scene Preview  

<img width="600" alt="image" src="https://github.com/user-attachments/assets/0445ca79-1921-43b9-b376-bb8fe35584ab" />
<img width="600" alt="image" src="https://github.com/user-attachments/assets/7b96b0c9-dd8b-4eff-a099-c0cc6b52910a" />
<img width="600" alt="image" src="https://github.com/user-attachments/assets/306d7a67-1822-4e1e-8a44-e7318d3d1aa4" />

---

## ✨ Features  
✅ **Dynamic Lighting**  
- Directional light (sun simulation) + 4 point lights with attenuation.

<img width="600" src="https://github.com/user-attachments/assets/63f615be-9f12-4786-a847-6dea720dff81" />


✅ **Atmospheric Effects**  
- Fog for depth illusion and eerie ambiance.  
- Lightning flashes with thunder sound effects.

<img width="600" src="https://github.com/user-attachments/assets/77303be6-de6f-4024-9cc6-5725e37f6e19" />

✅ **Optimized Rendering**  
- Fragment discarding for transparent/alpha textures (plants, greenhouse glass).

<img width="600" src="https://github.com/user-attachments/assets/4afe5356-3229-4408-a04e-b43eca0c652c" />
<img width="600" src="https://github.com/user-attachments/assets/4743cd90-3545-4525-8f0d-36430e2ad137" />

- Shadow mapping for realistic directional light shadows.

<img width="600" alt="image" src="https://github.com/user-attachments/assets/659240c6-f3ae-4428-9874-df7395e34673" />


✅ **Interactive Animations**  
- Door/chain mechanics with smooth rotations.
  
<img width="600" alt="image" src="https://github.com/user-attachments/assets/7b2b6680-e01f-4b27-bd4f-11081dde086f" />

  
- Automated camera tour with linear interpolation.

✅ **Real-Time Controls**  
- Switch between normal, wireframe, and point-view modes.

<img width="600" src="https://github.com/user-attachments/assets/2fda71d5-c33e-4ef9-9c7b-15731c566121" />


---

## 🛠️ Built With  
| Technology  | Description                          |  
|-------------|--------------------------------------|  
| **OpenGL**  | Core 3D rendering                   |  
| **GLFW**    | Window and input management         |  
| **GLM**     | Matrix/vector operations            |  
| **Blender** | 3D modeling (greenhouse, props)     |  
| **Audacity**| Sound effect editing (thunder)      |  
