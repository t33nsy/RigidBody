#include "render.h"

#define SIZE 0.07
#define NUM 2
#define DELIM 1e5
#define RADIUS_DELIM 1e7
#define POLY 10
double OldTime = -1, DeltaTime, total_mass = 0;
std::vector<RigidBody> rb(NUM);
__uint128_t fl = 1;
Context context;
glm::dvec3 barycenter(0, 0, 0);
glm::dvec3 cameradist = {-2, -2, -2};
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

void Display() {  // Очистка экрана и буфера глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(rb[0].r.x + cameradist.x, rb[0].r.y + cameradist.y,
              rb[0].r.z + cameradist.z,  // Позиция камеры
              rb[0].r.x, rb[0].r.y,
              rb[0].r.z,  // Точка, на которую смотрим
              0, 1, 0);   // Направление "вверх"

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
        // std::cout << barycenter.x * barycenter.x + barycenter.y * barycenter.y +
        //                  barycenter.z * barycenter.z
        //           << "\n";
        bary << barycenter.x << ", " << barycenter.y << ", " << barycenter.z
             << "\n";
        std::cout << computeTotalEnergy(rb) / 1e39 << "\n";
    }

    glFlush();
    glutSwapBuffers();
}

void Idle() {
    // update time
    long Time;
    if (OldTime == -1) OldTime = clock();
    Time = clock();
    DeltaTime = (double)(Time - OldTime) / CLOCKS_PER_SEC / 100;
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
    // Массы (в кг)
    context.masses[0] = 1.989e30;  // Звезда (Солнце)
    context.masses[1] = 3.3e23;    // Меркурий
    // context.masses[2] = 2.4478e16;  // Астероид
    // context.masses[3] = 4.867e24;   // Венера
    // context.masses[4] = 5.972e24;   // Земля

    // Радиусы (в метрах)
    context.radiuses[0] = 1.957e6;   // Радиус звезды (Солнце)
    context.radiuses[1] = 1.4397e6;  // Радиус Меркурия
    // context.radiuses[2] = 1.0e6;     // Радиус астероида
    // context.radiuses[3] = 1.0518e6;  // Радиус Венеры
    // context.radiuses[4] = 1.371e6;   // Радиус Земли

    // Положение, импульс, кватернион и момент импульса для каждого тела
    rb[0].r = glm::dvec3(0, 0, 0);  // Звезда в центре
    rb[0].P = glm::dvec3(0, 0, 0);  // Нет начального импульса
    rb[0].q =
        glm::angleAxis(glm::radians(45.0), glm::dvec3(0, 0, 1));  // Звезда
    rb[0].L = glm::dvec3(0, 0, 1e5);  // Момент импульса звезды

    rb[1].r = glm::dvec3(4.6e5, 8.89e4, 2.87e4);    // Положение Меркурия
    rb[1].P = glm::dvec3(-1.81e5, 4.97e6, 2.15e4);  // Импульс Меркурия
    rb[1].q =
        glm::angleAxis(glm::radians(30.0), glm::dvec3(0, 1, 0));  // Меркурий
    rb[1].L = glm::dvec3(0, 1e4, 0);
    // // Момент импульса Меркурия

    // rb[2].r = glm::dvec3(-3.8235e5, 1.4067e4, -4.558e4);  // Положение
    // астероида rb[2].P = glm::dvec3(4.79e5, -1.992e6, 6.1e6);        //
    // Импульс астероида rb[2].q =
    //     glm::angleAxis(glm::radians(60.0), glm::dvec3(1, 0, 0));  // Астероид
    // rb[2].L = glm::dvec3(1e3, 0, 0);  // Момент импульса астероида

    // rb[3].r = glm::dvec3(1.08e6, 0, 0);  // Положение Венеры
    // rb[3].P = glm::dvec3(0, 3.5e6, 0);   // Импульс Венеры
    // rb[3].q =
    //     glm::angleAxis(glm::radians(90.0), glm::dvec3(0, 0, 1));  // Венера
    // rb[3].L = glm::dvec3(0, 0, 2e4);  // Момент импульса Венеры

    // rb[4].r = glm::dvec3(1.496e6, 0, 0);  // Положение Земли
    // rb[4].P = glm::dvec3(0, 2.98e6, 0);   // Импульс Земли
    // rb[4].q =
    //     glm::angleAxis(glm::radians(120.0), glm::dvec3(0, 1, 0));  // Земля
    // rb[4].L = glm::dvec3(0, 3e4, 0);  // Момент импульса Земли

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