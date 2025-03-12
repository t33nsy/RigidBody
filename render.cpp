#include "render.h"

#define SIZE 0.07
#define NUM 5
#define DELIM 1e5
#define POLY 10
double OldTime = -1, DeltaTime;
std::vector<RigidBody> rb(NUM);
Context context;
glm::dvec3 cameradist = {-1, -1, -1};
int fl = 0;

// Create new frame in memory and draw it in window
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

    // std::cout << "L: " << rb[1].L[0] << ", " << rb[1].L[1] << ", " << rb[1].L[2]
    //           << std::endl;

    for (int i = 0; i < NUM; ++i) {
        glPushMatrix();

        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslated(rb[i].r.x / DELIM, rb[i].r.y / DELIM, rb[i].r.z / DELIM);

        rb[i].q = glm::normalize(rb[i].q);
        glRotated(
            glm::degrees(2.0f * std::atan2(std::sqrt(rb[i].q.x * rb[i].q.x +
                                                     rb[i].q.y * rb[i].q.y +
                                                     rb[i].q.z * rb[i].q.z),
                                           rb[i].q.w)),
            rb[i].q.x, rb[i].q.y, rb[i].q.z);
        glutWireSphere(context.radiuses[i], POLY, POLY);

        glPopMatrix();
    }

    // Обновление экрана
    glFlush();
    glutSwapBuffers();
}

void Idle() {
    // update time
    long Time;
    if (OldTime == -1) OldTime = clock();
    Time = clock();
    DeltaTime = (double)(Time - OldTime) / CLOCKS_PER_SEC / 1000;
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
    if (Key == 27) exit(0);
    glutPostRedisplay();
}

void Run(int argc, char* argv[]) {
    contextReserve(context, NUM);
    // Массы (в кг)
    context.masses[0] = 1.989e30;   // Звезда (например, Солнце)
    context.masses[1] = 3.3e23;     // Планета (например, Меркурий)
    context.masses[2] = 2.4478e16;  // Астероид
    context.masses[3] = 4.867e24;   // Планета (например, Венера)
    context.masses[4] = 5.972e24;   // Планета (например, Земля)

    // Радиусы (в условных единицах)
    context.radiuses[0] = 0.1;   // Радиус звезды
    context.radiuses[1] = 0.04;  // Радиус Меркурия
    context.radiuses[2] = 0.07;  // Радиус астероида
    context.radiuses[3] = 0.09;  // Радиус Венеры
    context.radiuses[4] = 0.1;   // Радиус Земли

    // Положение, импульс, кватернион и момент импульса для каждого тела
    rb[0].r = glm::dvec3(0, 0, 0);     // Звезда в центре
    rb[0].P = glm::dvec3(0, 0, 0);     // Нет начального импульса
    rb[0].q = glm::dquat(1, 0, 0, 0);  // Единичный кватернион (нет вращения)
    rb[0].L = glm::dvec3(0, 0.12, 0);  // Момент импульса звезды

    rb[1].r = glm::dvec3(4.6e5, 8.89e4, 2.87e4);    // Положение Меркурия
    rb[1].P = glm::dvec3(-1.81e5, 4.97e6, 2.15e4);  // Импульс Меркурия
    rb[1].q = glm::dquat(1, 0, 0, 0);               // Нет вращения
    rb[1].L = glm::dvec3(0.32, 0, 0.02);            // Момент импульса Меркурия

    rb[2].r = glm::dvec3(-3.8235e5, 1.4067e4, -4.558e4);  // Положение астероида
    rb[2].P = glm::dvec3(4.79e5, -1.992e6, 6.1e6);        // Импульс астероида
    rb[2].q = glm::dquat(0, 1, 0, 0);  // Кватернион для вращения
    rb[2].L = glm::dvec3(0, 0.01, 0);  // Момент импульса астероида

    rb[3].r = glm::dvec3(1.08e6, 0, 0);  // Положение Венеры
    rb[3].P = glm::dvec3(0, 3.5e6, 0);   // Импульс Венеры
    rb[3].q = glm::dquat(0, 0, 1, 0);    // Кватернион для вращения
    rb[3].L = glm::dvec3(0, 0.15, 0);    // Момент импульса Венеры

    rb[4].r = glm::dvec3(1.496e6, 0, 0);  // Положение Земли
    rb[4].P = glm::dvec3(0, 2.98e6, 0);   // Импульс Земли
    rb[4].q = glm::dquat(0, 0, 0, 1);     // Кватернион для вращения
    rb[4].L = glm::dvec3(0, 0.2, 0);      // Момент импульса Земли

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
    glClearColor(0.1, 0.5, 0.7, 0);

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