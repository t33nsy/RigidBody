#include "dif.h"

glm::dvec3 force(const glm::dvec3& r1, const double m1, const glm::dvec3& r2,
                 const double m2, const double d) {
    return G * m1 * m2 * (r2 - r1) / d;
}

std::vector<RigidBody> f_rigidbody(const std::vector<RigidBody>& rb, int num,
                                   double time, const Context& c) {
    std::vector<RigidBody> dt(num);
    std::vector<std::vector<double>> distances(num, std::vector<double>(num));
    glm::dmat3x3 R;
    glm::dvec3 omega;
    for (int i = 0; i < num; ++i) {
        for (int j = 0; j < num; ++j) {
            distances[i][j] = glm::distance(rb[i].r, rb[j].r);
            distances[i][j] = std::max(distances[i][j], 1e-11);
            distances[i][j] *= distances[i][j] * distances[i][j];
        }
    }
    glm::dvec3 tau_sum(0, 0, 0);
    for (int i = 0; i < num; ++i) {
        // pos' = P
        dt[i].r = rb[i].P;
        // quat to temp rotate
        R = glm::dmat3x3(rb[i].q);
        // w = R * I-1 * R^T * L
        omega = R * c.Ibodies_inv[i] * glm::transpose(R) * rb[i].L;
        // q' = 0.5 * omega * q
        dt[i].q = 0.5 * glm::dquat(0, omega) * rb[i].q;
        dt[i].q = glm::normalize(dt[i].q);
        glm::dvec3 force_sum(0, 0, 0);
        glm::dvec3 torq_sum(0, 0, 0);
        for (int j = 0; j < num; ++j) {
            if (j == i) continue;
            glm::dvec3 t = force(rb[i].r, c.masses[i], rb[j].r, c.masses[j],
                                 distances[i][j]);
            force_sum += t;
            torq_sum += glm::cross(rb[i].r - rb[j].r, t);
        }
        // v' = F / m
        dt[i].P = force_sum / c.masses[i];
        // L' = tau
        dt[i].L = torq_sum;
        tau_sum += dt[i].L;
    }
    return dt;
}

RigidBody sum(const RigidBody& a, const RigidBody& b) {
    RigidBody r;
    r.r = a.r + b.r;
    r.f = a.f + b.f;
    r.q = a.q + b.q;
    r.I = a.I + b.I;
    r.L = a.L + b.L;
    r.P = a.P + b.P;
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

RigidBody operator+(const RigidBody& a, const RigidBody& b) {
    return sum(a, b);
}

RigidBody mul(const RigidBody& a, double num) {
    RigidBody r;
    r.r = a.r * num;
    r.f = a.f * num;
    r.q = a.q * num;
    r.I = a.I * num;
    r.L = a.L * num;
    r.P = a.P * num;
    return r;
}

RigidBody operator*(const RigidBody& a, double b) { return mul(a, b); }

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

void contextReserve(Context& c, int num) {
    c.masses.resize(num);
    c.Ibodies.resize(num);
    c.Ibodies_inv.resize(num);
    c.radiuses.resize(num);
}

void createInertiaTensors(Context& c, int num) {
    for (int i = 0; i < num; ++i) {
        c.Ibodies[i][0][0] =
            2.0f / 5.0f * c.masses[i] * c.radiuses[i] * c.radiuses[i];
        c.Ibodies_inv[i][0][0] =
            5.0f / 2.0f / c.masses[i] / c.radiuses[i] / c.radiuses[i];
    }
}

double step(std::vector<RigidBody>& rb, int num, double h, double cur_time,
            Context c) {
    std::vector<RigidBody> k1 = f_rigidbody(rb, num, cur_time, c);

    std::vector<RigidBody> k2 =
        f_rigidbody(rb + k1 * (h / 2.0), num, cur_time + h / 2.0, c);

    std::vector<RigidBody> k3 =
        f_rigidbody(rb + k2 * (h / 2.0), num, cur_time + h / 2.0, c);

    std::vector<RigidBody> k4 = f_rigidbody(rb + k3 * h, num, cur_time + h, c);

    for (int i = 0; i < num; ++i) {
        rb[i] = rb[i] + (k1[i] + k2[i] * 2 + k3[i] * 2 + k4[i]) * (h / 6.0);
    }

    // glm::dvec3 temp = rb[0].r;
    // for (int i = 0; i < num; ++i) {
    //     rb[i].r -= temp;
    // }
    return 0.0;
}
