#include "render.h"

#define SIZE 0.07
#define NUM 10
#define DELIM 1
#define RADIUS_DELIM 1
#define POLY 10
double OldTime = -1, DeltaTime, total_mass = 0;
std::vector<RigidBody> rb(NUM);
__uint128_t fl = 0;
Context context;
glm::dvec3 barycenter(0, 0, 0);
glm::dvec3 cameradist = {0, 15, 0};
std::ofstream bary("bary.csv");
std::ofstream energy("energy.csv");

double computeKineticEnergy(const std::vector<RigidBody>& rb) {
    double kinetic_energy = 0.0;
    glm::dmat3x3 R;
    glm::dvec3 omega;
    for (size_t i = 0; i < rb.size(); ++i) {
        // Поступательное движение
        double translational_energy =
            glm::dot(rb[i].P, rb[i].P) / (2.0 * context.masses[i]);
        // Вращательное движение
        R = glm::dmat3x3(rb[i].q);
        omega = R * context.Ibodies_inv[i] * glm::transpose(R) * rb[i].L;
        double rotational_energy = 0.5 * glm::dot(rb[i].L, omega);
        kinetic_energy += translational_energy + rotational_energy;
    }
    return kinetic_energy;
}

double computePotentialEnergy(const std::vector<RigidBody>& rb) {
    double potential_energy = 0.0;

    for (size_t i = 0; i < rb.size() - 1; ++i) {
        for (size_t j = i + 1; j < rb.size(); ++j) {
            double distance = glm::distance(rb[i].r, rb[j].r);
            potential_energy -=
                G * context.masses[i] * context.masses[j] / distance;
        }
    }

    return potential_energy;
}

double computeTotalEnergy(const std::vector<RigidBody>& rb) {
    double kinetic = computeKineticEnergy(rb);
    double potential = computePotentialEnergy(rb);
    return kinetic + potential;
}

double test(const RigidBody& rb1, const RigidBody& rb2) {
    glm::dvec3 delta = rb1.r - rb2.r;
    double dist = glm::length(delta);
    double centr = context.masses[1] * glm::dot(rb1.P, rb2.P) / dist;
    double force = G * context.masses[0] * context.masses[1] / (dist * dist);
    return centr - force;
}

double invariant(const std::vector<RigidBody>& rb) {
    double ans;
    for (int i = 0; i < NUM - 1; ++i) {
        for (int j = i + 1; j < NUM; ++j) {
            ans += test(rb[i], rb[j]);
        }
    }
    return ans;
}
void Display() {  // Очистка экрана и буфера глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameradist.x, cameradist.y,
              cameradist.z,  // Позиция камеры
              0, 0, 0,       // Точка, на которую смотрим
              1, 0, 0);      // Направление "вверх"

    // Обновление состояния
    step(rb, NUM, DeltaTime, OldTime, context);

    glm::dvec3 ang_momentum = {0, 0, 0};
    glm::dvec3 temp_barycenter = glm::dvec3(0, 0, 0);
    for (int i = 0; i < NUM; ++i) {
        glPushMatrix();

        glColor3f(255.0f / 255.0f, 218.0f / 255.0f, 185.0f / 255.0f);
        glTranslated(rb[i].r.x / DELIM, rb[i].r.y / DELIM, rb[i].r.z / DELIM);

        rb[i].q = glm::normalize(rb[i].q);
        glRotated(
            glm::degrees(2.0f * std::atan2(std::sqrt(rb[i].q.x * rb[i].q.x +
                                                     rb[i].q.y * rb[i].q.y +
                                                     rb[i].q.z * rb[i].q.z),
                                           rb[i].q.w)),
            rb[i].q.x, rb[i].q.y, rb[i].q.z);
        glutWireSphere(context.radiuses[i] / RADIUS_DELIM, POLY, POLY);

        ang_momentum += rb[i].L;
        temp_barycenter += rb[i].r * context.masses[i];
        total_mass += context.masses[i];
        glPopMatrix();
    }
    barycenter = temp_barycenter / total_mass;
    ++fl;
    if (fl % 200 == 0) {
        std::cout << "Bary: " << barycenter.x << ", " << barycenter.y << ", "
                  << barycenter.z << "\n";
        bary << barycenter.x << ", " << barycenter.y << ", " << barycenter.z
             << "\n";
        double total_energy = computeTotalEnergy(rb);
        std::cout << "Total energy: " << total_energy << "\n";
        energy << total_energy << "\n";
    }

    glFlush();
    glutSwapBuffers();
}

void Idle() {
    // update time
    long Time;
    if (OldTime == -1) OldTime = clock();
    Time = clock();
    DeltaTime = (double)(Time - OldTime) / CLOCKS_PER_SEC / 1;
    OldTime = Time;

    // redraw frame
    glutPostRedisplay();
}

void Reshape(int W, int H) {
    // set camera model
    glViewport(0, 0, W, H);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (double)W / H, 1, 500);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Keyboard(unsigned char Key, int MouseX, int MouseY) {
    if (Key == 27) {
        bary.close();
        energy.close();
        exit(0);
    }
    glutPostRedisplay();
}

void Run(int argc, char* argv[]) {
    contextReserve(context, NUM);

    context.masses[0] = 1e11;
    context.masses[1] = 1;
    context.masses[2] = 2;
    context.masses[3] = 4;
    context.masses[4] = 10;
    context.masses[5] = 5;
    context.masses[6] = 18;
    context.masses[7] = 17;
    context.masses[8] = 15;
    context.masses[9] = 0.5;

    context.radiuses[0] = 1;
    context.radiuses[1] = 0.4;
    context.radiuses[2] = 0.5;
    context.radiuses[3] = 0.6;
    context.radiuses[4] = 0.4;
    context.radiuses[5] = 0.2;
    context.radiuses[6] = 0.5;
    context.radiuses[7] = 0.3;
    context.radiuses[8] = 0.4;
    context.radiuses[9] = 0.1;

    rb[0].r = glm::dvec3(0, 0, 0);
    rb[0].P = glm::dvec3(0, 0, 0);
    rb[0].q = glm::angleAxis(glm::radians(45.0), glm::dvec3(0, 0, 1));
    rb[0].L = glm::dvec3(0, 0, 0);

    rb[1].r = glm::dvec3(2.3, 0, 0);
    rb[1].P = glm::dvec3(0, 0, sqrt(2.9));
    rb[1].q = glm::angleAxis(glm::radians(30.0), glm::dvec3(0, 1, 0));
    rb[1].L = glm::dvec3(0, 0, 0);

    rb[2].r = glm::dvec3(0, 0, 6);
    rb[2].P = glm::dvec3(0, sqrt(6.67 / 6), 0);
    rb[2].q = glm::angleAxis(glm::radians(60.0), glm::dvec3(1, 0, 0));
    rb[2].L = glm::dvec3(0, 0, 0);

    rb[3].r = glm::dvec3(8.4, 0, 0);
    rb[3].P = glm::dvec3(0, 0, sqrt(6.67 / 8.4));
    rb[3].q = glm::angleAxis(glm::radians(90.0), glm::dvec3(0, 0, 1));
    rb[3].L = glm::dvec3(0, 0, 0);

    rb[4].r = glm::dvec3(0, 10, 0);
    rb[4].P = glm::dvec3(sqrt(6.67 / 10), 0, 0);
    rb[4].q = glm::angleAxis(glm::radians(120.0), glm::dvec3(0, 1, 0));
    rb[4].L = glm::dvec3(0, 0, 0);

    rb[5].r = glm::dvec3(0, 2.3, 0);
    rb[5].P = glm::dvec3(sqrt(2.9), 0, 0);
    rb[5].q = glm::angleAxis(glm::radians(120.0), glm::dvec3(0, 1, 0));
    rb[5].L = glm::dvec3(0, 0, 0);

    rb[6].r = glm::dvec3(6, 0, 0);
    rb[6].P = glm::dvec3(0, 1, 0.334166);
    rb[6].q = glm::angleAxis(glm::radians(120.0), glm::dvec3(0, 1, 0));
    rb[6].L = glm::dvec3(0, 0, 0);

    rb[7].r = glm::dvec3(0, 0, 8.4);
    rb[7].P = glm::dvec3(sqrt(6.67 / 8.4), 0, 0);
    rb[7].q = glm::angleAxis(glm::radians(120.0), glm::dvec3(0, 1, 0));
    rb[7].L = glm::dvec3(0, 0, 0);

    rb[8].r = glm::dvec3(10, 0, 0);
    rb[8].P = glm::dvec3(0, sqrt(6.67 / 10), 0);
    rb[8].q = glm::angleAxis(glm::radians(120.0), glm::dvec3(0, 1, 0));
    rb[8].L = glm::dvec3(0, 0, 0);

    rb[9].r = glm::dvec3(0, 0, 2.3);
    rb[9].P = glm::dvec3(sqrt(2.9), 0, 0);
    rb[9].q = glm::angleAxis(glm::radians(120.0), glm::dvec3(0, 1, 0));
    rb[9].L = glm::dvec3(0, 0, 0);

    // compute barycenter in t0
    glm::dvec3 temp_barycenter = glm::dvec3(0, 0, 0);
    for (int i = 0; i < NUM; ++i) {
        temp_barycenter += rb[i].r * context.masses[i];
        total_mass += context.masses[i];
    }
    barycenter = temp_barycenter / total_mass;

    // initialization
    glutInit(&argc, argv);
    // Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // Set size of window
    glutInitWindowSize(WIDTH, HEIGHT);
    // Windows position on screen
    glutInitWindowPosition(0, 0);
    // Creating window with name
    glutCreateWindow("lab1");

    // Fill background color
    glClearColor(40.0f / 255.0f, 55.0f / 255.0f, 70.0f / 255.0f, 1.0f);

    // Set functions for GLUT loop
    glutReshapeFunc(Reshape);
    glutDisplayFunc(Display);
    glutIdleFunc(Idle);
    glutKeyboardFunc(Keyboard);

    // Enable 3D mode
    glEnable(GL_DEPTH_TEST);

    // Start infinite loop
    glutMainLoop();
}