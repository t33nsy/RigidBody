#ifndef _t33nsy_DIF
#define _t33nsy_DIF
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>

struct Context {
    double mass;
    glm::dvec3 pos1, pos2, pos3, pos4;
    double m1, m2, m3, m4;
};

struct RigidBody {
    glm::dvec3 r;
    glm::dvec3 v;
    glm::dvec3 f;
};

double step(RigidBody &rb, const Context &ctx, double h, double cur_time);
#endif /* _t33nsy_DIF */
