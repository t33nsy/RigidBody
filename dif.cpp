#include "dif.h"
#define G 6.67
// derivative f(X(t), t) = dX/dt in point t.
RigidBody f_rigidbody(const RigidBody& rb, const Context& context,
                      double time) {
    RigidBody dt;  // result
    dt.r = rb.v;
    double d1 = glm::distance(rb.r, context.pos1);
    glm::dvec3 f1 = G * context.m1 * (-rb.r + context.pos1) / (d1 * d1 * d1);
    double d2 = glm::distance(rb.r, context.pos2);
    glm::dvec3 f2 = G * context.m2 * (-rb.r + context.pos2) / (d2 * d2 * d2);
    double d3 = glm::distance(rb.r, context.pos3);
    glm::dvec3 f3 = G * context.m3 * (-rb.r + context.pos3) / (d3 * d3 * d3);
    double d4 = glm::distance(rb.r, context.pos4);
    glm::dvec3 f4 = G * context.m4 * (-rb.r + context.pos4) / (d4 * d4 * d4);
    dt.v = f1 + f2 + f3 + f4;
    // std::cout << "force = " << f1[0] << " " << f1[1] << " " << f1[2]
    //   << std::endl;
    dt.v /= context.mass;
    // std::cout << "force = " << dt.v[0] << " " << dt.v[1] << " " << dt.v[2]
    //   << std::endl;
    return dt;
}

RigidBody sum(const RigidBody& a, const RigidBody& b) {
    RigidBody r;
    r.r = a.r + b.r;
    r.v = a.v + b.v;
    return r;
}

RigidBody mul(const RigidBody& a, double num) {
    RigidBody r;
    r.r = a.r * num;
    r.v = a.v * num;
    return r;
}

double step(RigidBody& rb, const Context& ctx, double h, double cur_time) {
    RigidBody k1 = f_rigidbody(rb, ctx, cur_time);
    RigidBody k2 =
        f_rigidbody(sum(rb, mul(k1, h / 2.0)), ctx, cur_time + h / 2.0);
    RigidBody k3 =
        f_rigidbody(sum(rb, mul(k2, h / 2.0)), ctx, cur_time + h / 2.0);
    RigidBody k4 = f_rigidbody(sum(rb, mul(k3, h)), ctx, cur_time + h);
    rb = sum(rb,
             mul(sum(sum(k1, mul(k2, 2.0)), sum(mul(k3, 2.0), k4)), h / 6.0));
    std::cout << "r1 = " << rb.r[0] << " " << rb.r[1] << " " << rb.r[2]
              << " u = " << rb.v[0] << " " << rb.v[1] << " " << rb.v[2]
              << std::endl;
    return 0.0;
}