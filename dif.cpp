#include "dif.h"
#define G 6.67e-11
// derivative f(X(t), t) = dX/dt in point t.

glm::dvec3 force(const glm::dvec3& r1, const double m1, const glm::dvec3& r2,
                 const double m2, const double d) {
    return G * m1 * m2 * (r2 - r1) / d;
}

std::vector<RigidBody> f_rigidbody(const std::vector<RigidBody>& rb, int num,
                                   double time, Context c) {
    std::vector<RigidBody> dt(num);
    std::vector<std::vector<double>> distances(num, std::vector<double>(num));
    // RigidBody dt;  // result
    for (int i = 0; i < num; ++i) {
        dt[i].r = rb[i].v;
        for (int j = 0; j < num; ++j) {
            distances[i][j] = glm::distance(rb[i].r, rb[j].r);
            distances[i][j] = std::max(distances[i][j], 1e-11);
            distances[i][j] *= distances[i][j] * distances[i][j];
        }
    }
    for (int i = 0; i < num; ++i) {
        glm::dvec3 force_sum(0, 0, 0);
        for (int j = 0; j < num; ++j) {
            if (j == i) continue;
            force_sum += force(rb[i].r, c.masses[i], rb[j].r, c.masses[j],
                               distances[i][j]);
        }
        dt[i].v = force_sum / c.masses[i];
    }
    return dt;
}

RigidBody sum(const RigidBody& a, const RigidBody& b) {
    RigidBody r;
    r.r = a.r + b.r;
    r.v = a.v + b.v;
    return r;
}

std::vector<RigidBody> vec_sum(const std::vector<RigidBody>& a,
                               const std::vector<RigidBody>& b, int num) {
    std::vector<RigidBody> r(num);
    for (int i = 0; i < num; ++i) {
        r[i] = sum(a[i], b[i]);
    }
    return r;
}

RigidBody mul(const RigidBody& a, double num) {
    RigidBody r;
    r.r = a.r * num;
    r.v = a.v * num;
    return r;
}

std::vector<RigidBody> vec_mul(const std::vector<RigidBody>& a,
                               const std::vector<double>& b, int num) {
    std::vector<RigidBody> r(num);
    for (int i = 0; i < num; ++i) {
        r[i] = mul(a[i], b[i]);
    }
    return r;
}

std::vector<RigidBody> vec_mul(const std::vector<RigidBody>& a, double b,
                               int num) {
    std::vector<RigidBody> r(num);
    for (int i = 0; i < num; ++i) {
        r[i] = mul(a[i], b);
    }
    return r;
}

std::vector<RigidBody> operator+(const std::vector<RigidBody>& a,
                                 const std::vector<RigidBody>& b) {
    std::vector<RigidBody> r(a.size());
    for (int i = 0; i < a.size(); ++i) {
        r[i] = sum(a[i], b[i]);
    }
    return r;
}

std::vector<RigidBody> operator*(const std::vector<RigidBody>& a, double b) {
    std::vector<RigidBody> r(a.size());
    for (int i = 0; i < a.size(); ++i) {
        r[i] = mul(a[i], b);
    }
    return r;
}

double step(std::vector<RigidBody>& rb, int num, double h, double cur_time,
            Context c) {
    std::vector<RigidBody> k1 = f_rigidbody(rb, num, cur_time, c);

    std::vector<RigidBody> k2 =
        f_rigidbody(rb + k1 * (h / 2.0), num, cur_time + h / 2.0, c);

    std::vector<RigidBody> k3 =
        f_rigidbody(rb + k2 * (h / 2.0), num, cur_time + h / 2.0, c);

    std::vector<RigidBody> k4 = f_rigidbody(rb + k3 * h, num, cur_time + h, c);

    rb = rb + (k1 + k2 * 2 + k3 * 2 + k4) * (h / 6.0);

    glm::dvec3 temp = rb[0].r;
    for (int i = 0; i < num; ++i) {
        rb[i].r -= temp;
    }
    return 0.0;
}
