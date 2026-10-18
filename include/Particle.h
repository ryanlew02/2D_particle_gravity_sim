#pragma once
#include "Vec2.h"
#include <array>



class Particle {

public:
    Vec2 pos;
    Vec2 v;
    float radius;
    float restitution = 1.0f;
    float density, mass;
    std::array<float, 3> color = {0.0f, 0.0f, 0.0f};

    Particle(float x, float y, float vx, float vy, float radius, std::array<float, 3> color = {0.0f, 0.0f, 0.0f}, float density = 1.0f);

    void accelerate(const Vec2& acc);

    void updatePos();

    void handleCollisionsWithWalls();

    //checks if 2 circles are colliding
    [[nodiscard]] bool isColliding(const Particle& otherParticle) const;

    //apply's a force to current object
    void applyForce(const Vec2& force);
};