#ifndef _t33nsy_DIF
#define _t33nsy_DIF
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
struct Context {
    std::vector<double> masses;
    std::vector<double> radiuses;
    std::vector<glm::dmat3x3> Ibodies;
    std::vector<glm::dmat3x3> Ibodies_inv;
};

struct RigidBody {
    glm::dvec3 r;    // position
    glm::dvec3 f;    // force
    glm::dvec3 P;    // linear momentum
    glm::dvec3 L;    // angular momentum
    glm::dmat3x3 I;  // inertia tensor
    glm::dquat q;    // quaternion
};

void contextReserve(Context& c, int num);

double step(std::vector<RigidBody>& rb, int num, double h, double cur_time,
            Context c);
#endif /* _t33nsy_DIF */
