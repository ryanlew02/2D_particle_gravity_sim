#include "Physics.h"
#include "Particle.h"
#include "Vec2.h"
#include <algorithm>
#include <valarray>



void handleCollision(Particle& a, Particle& b) {
    const Vec2 d = b.pos - a .pos;

    float dist = d.length();
    const float rSum = a.radius + b.radius;

    if (dist >= rSum) {
        return;
    }

    Vec2 normal;

    if (dist > 0.00001f) {
        normal = d / dist;
    } else {
        normal = {1.0f, 0.0f};
        dist = 0.0f;
    }

    const Vec2 rv = b.v - a.v;

    const float invMassA = 1.0f / a.mass;
    const float invMassB = 1.0f / b.mass;

    const float velocityAlongNormal = rv.dot(normal);

    const float e = std::min(a.restitution, b.restitution);

    if (velocityAlongNormal < 0.0f) {
        const float impulse = -(1.0f + e)
        * velocityAlongNormal
        / (invMassA + invMassB);

        const Vec2 impulseVec = normal * impulse;

        a.v -= impulseVec * invMassA;
        b.v += impulseVec * invMassB;
    }

    float pen = rSum - dist;

    if (pen > 0.0f) {
        const float correction = pen / (invMassA + invMassB);

        a.pos -= normal * invMassA * correction;
        b.pos += normal * invMassB * correction;
    }

}

void applyGravity(Particle& a, Particle& b) {
    constexpr float G = 0.6f;
    constexpr float SOFTENING = 0.02f;

    const Vec2 direction = b.pos - a.pos;

    const float distSq = direction.lengthSquared() + SOFTENING * SOFTENING;

    const float invDist = 1.0f / std::sqrt(distSq);

    const float forceMagnitude = G * a.mass * b.mass * invDist * invDist * invDist;

    const Vec2 force = direction * forceMagnitude;

    a.applyForce(force);
    b.applyForce(-force);
}