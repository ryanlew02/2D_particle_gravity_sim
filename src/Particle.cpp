#include "Particle.h"
#include "Constants.h"

Particle::Particle(const float x, const float y, float vx, float vy, float radius, std::array<float, 3> color,  float density)
    : pos(x, y), v(vx, vy), radius(radius), color(color), density(density), mass(density * 3.14159265 * radius * radius) {}

void Particle::accelerate(const Vec2& acc) {
    v += acc * FIXED_DT;
}

void Particle::updatePos() { 
    pos += v * FIXED_DT;
}

//this handles collisions with walls
void Particle::handleCollisionsWithWalls() {

    const float min_boundry = -1.0f + radius;
    const float max_boundry = 1.0f - radius;

    if (pos.x < min_boundry) {
        pos.x = min_boundry;

        if (v.x < 0.0f) {
            v.x  *= -restitution;
        }
    } else if (pos.x > max_boundry) {
        pos.x = max_boundry;

        if (v.x  > 0.0f) {
            v.x  *= -restitution;
        }
    }
    if (pos.y < min_boundry) {
        pos.y = min_boundry;

        if (v.y < 0.0f) {
            v.y *= -restitution;
        }
    } else if (pos.y > max_boundry) {
        pos.y = max_boundry;

        if (v.y > 0.0f) {
            v.y *= -restitution;
        }
    }
}

//checks if 2 circles are colliding
bool Particle::isColliding(const Particle& otherParticle) const {
    const Vec2 diff = pos - otherParticle.pos;
    const float rDist = radius + otherParticle.radius;

    return diff.lengthSquared() <= rDist * rDist;
}

//apply's a force to current object
void Particle::applyForce(const Vec2& force) {

    Vec2 acceleration = force / mass;
    v += acceleration * FIXED_DT;

}