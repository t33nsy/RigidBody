#ifndef _t33nsy_DIF
#define _t33nsy_DIF
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
struct Context {
    std::vector<double> masses;
};

struct RigidBody {
    glm::dvec3 r;
    glm::dvec3 v;
    glm::dvec3 f;
    glm::quat q;
};

double step(std::vector<RigidBody>& rb, int num, double h, double cur_time,
            Context c);
#endif /* _t33nsy_DIF */
