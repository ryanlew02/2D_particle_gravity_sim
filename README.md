
# 2D Particle Gravity Simulation (HANDWRITTEN)

A real-time 2D gravitational physics simulation built from scratch using C++ and OpenGL.

This project simulates the gravitational interactions between particles with varying masses, velocities, and sizes. It uses Newtonian gravity to calculate particle motion and OpenGL to render the simulation in real time.

If you have any ideas for this project or things that I should have done better, let me know!
I am always looking for ways to improve my code. 

## Features

- Real-time gravitational interactions between particles
- Newtonian gravity calculations
- Particle collision detection and response
- Customizable particle mass, density, radius, and velocity
- Different particle colors
- Fixed timestep physics simulation
- Hardware-accelerated rendering using OpenGL
- Object-oriented architecture separating physics, rendering, and application logic

## Technologies

- **C++17** - Core simulation and physics
- **OpenGL** - Graphics rendering
- **GLFW** - Window creation and input handling
- **GLAD** - OpenGL function loading
- **CMake** - Build system

## Physics

The simulation uses Newton's law of universal gravitation:

$$
F = G \frac{m_1m_2}{r^2}
$$

Where:
- `F` is the gravitational force
- `G` is the gravitational constant
- `m1` and `m2` are the particle masses
- `r` is the distance between the particles

Each particle's acceleration is calculated from the gravitational forces acting on it, and its velocity and position are updated at a fixed timestep.

Particle mass is calculated using its density and area:

$$
m = \rho \pi r^2
$$

## Getting Started

### Prerequisites

- C++17-compatible compiler
- CMake 3.16 or newer
- Git
- OpenGL-compatible graphics hardware and drivers

GLFW is automatically downloaded through CMake.

### Build Instructions

Clone the repository:

```bash
git clone https://github.com/ryanlew02/2D_particle_gravity_sim.git
cd 2D_particle_gravity_sim
```

Configure and build:

```bash
cmake -S . -B build
cmake --build build
```

Run the simulation:

**Windows:**
```bash
.\build\simulation.exe
```

**macOS / Linux:**
```bash
./build/simulation
```

Note: The executable location may vary depending on your CMake generator and build configuration.

## Project Structure

```text
2D_particle_gravity_sim/
├── include/
│   ├── App.h
│   ├── Window.h
│   ├── Renderer.h
│   ├── Particle.h
│   ├── Physics.h
│   ├── Vec2.h
│   └── Constants.h
├── src/
│   ├── main.cpp
│   ├── App.cpp
│   ├── Window.cpp
│   ├── Renderer.cpp
│   ├── Particle.cpp
│   ├── Physics.cpp
│   └── gl.c
├── CMakeLists.txt
└── README.md
```

### Architecture

- **App** — Manages the application lifecycle and simulation loop.
- **Window** — Handles GLFW window creation and management.
- **Renderer** — Manages OpenGL shaders and particle rendering.
- **Physics** — Calculates gravitational interactions and collision responses.
- **Particle** — Stores individual particle properties and motion.
- **Vec2** — Provides 2D vector operations.

## Author

Created by [Ryan Lewandowski](https://github.com/ryanlew02).

Built as a personal project to explore C++, physics simulations, graphics programming, and software architecture.
