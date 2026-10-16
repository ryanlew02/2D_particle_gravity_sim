#pragma once
#include <Vec2.h>

class Particle {

public:
    Vec2 pos;
    Vec2 v;
    float radius;
    float restitution = 0.95f;
    float density, mass;


    Particle(float x, float y, float vx, float vy, float radius, float density = 1.0f);

    void accelerate(const Vec2& acc);

    void updatePos();

    void handleCollisions();

    //checks if 2 circles are colliding
    [[nodiscard]] bool isColliding(const Particle& otherParticle) const;

    //apply's a force to current object
    void applyForce(const Vec2& force);
};