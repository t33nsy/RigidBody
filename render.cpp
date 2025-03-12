#include "render.h"

#define SIZE 0.07
#define NUM 3
#define DELIM 1e5
#define POLY 10
double OldTime = -1, DeltaTime;
std::vector<RigidBody> rb(4);
Context context;
double cameraAngle = 0.0f;
double cameraPitch = 0.0f;
double cameraDistance = 100.0f;
glm::dvec3 cameradist = {-2.5, -2.5, -2.5};
int fl = 0;

// Create new frame in memory and draw it in window
void Display() {  // Очистка экрана и буфера глубины
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    // Настройка камеры
    // float cameraX = cameraDistance * cos(cameraPitch * M_PI / 180.0f) *
    //                 sin(cameraAngle * M_PI / 180.0f);
    // float cameraY = cameraDistance * sin(cameraPitch * M_PI / 180.0f);
    // float cameraZ = cameraDistance * cos(cameraPitch * M_PI / 180.0f) *
    //                 cos(cameraAngle * M_PI / 180.0f);

    gluLookAt(rb[0].r.x + cameradist.x, rb[0].r.y + cameradist.y,
              rb[0].r.z + cameradist.z,         // Позиция камеры
              rb[0].r.x, rb[0].r.y, rb[0].r.z,  // Точка, на которую смотрим
              0, -1, 0);                        // Направление "вверх"

    // Обновление состояния
    step(rb, NUM, DeltaTime, OldTime, context);

    for (int i = 0; i < NUM; ++i) {
        if (fl % 30)
            std::cout << i << ": (" << rb[i].r.x << ", " << rb[i].r.y << ", "
                      << rb[i].r.z << ")\n";
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslated(rb[i].r.x / DELIM, rb[i].r.y / DELIM, rb[i].r.z / DELIM);
        glutWireSphere(SIZE, POLY, POLY);
        glPopMatrix();
    }
    ++fl;

    // Обновление экрана
    glFlush();
    glutSwapBuffers();
}

void DrawStatic(const glm::vec3& pos, double size) {
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslated(pos.x, pos.y, pos.z);
    glutWireSphere(size, 20, 20);
    glPopMatrix();
}

void DrawSomething(double size) {
    glBegin(GL_QUADS);  // Begin drawing the color cube with 6 quads
    // glColor3f(1, 0.0f, 0.0f);

    glVertex3f(size, size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, size, size);
    glVertex3f(size, size, size);

    // Bottom face (y = -size)
    glVertex3f(size, -size, size);
    glVertex3f(-size, -size, size);
    glVertex3f(-size, -size, -size);
    glVertex3f(size, -size, -size);

    // Front face  (z = size)
    glVertex3f(size, size, size);
    glVertex3f(-size, size, size);
    glVertex3f(-size, -size, size);
    glVertex3f(size, -size, size);

    // Back face (z = -size)
    glVertex3f(size, -size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, size, -size);
    glVertex3f(size, size, -size);

    // Left face (x = -size)
    glVertex3f(-size, size, size);
    glVertex3f(-size, size, -size);
    glVertex3f(-size, -size, -size);
    glVertex3f(-size, -size, size);

    // Right face (x = size)
    glVertex3f(size, size, -size);
    glVertex3f(size, size, size);
    glVertex3f(size, -size, size);
    glVertex3f(size, -size, -size);
    glEnd();  // End of drawing color-cube
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
    if (Key == 'a') {
        cameraAngle -= 1.0f;  // Вращение влево
    } else if (Key == 'd') {
        cameraAngle += 1.0f;  // Вращение вправо
    } else if (Key == 'w') {
        cameraPitch += 1.0f;  // Наклон вверх
    } else if (Key == 's') {
        cameraPitch -= 1.0f;  // Наклон вниз
    }
    glutPostRedisplay();
}

void Run(int argc, char* argv[]) {
    context.masses.resize(NUM);
    context.masses[0] = 1.989e30;
    context.masses[1] = 3.3e23;
    context.masses[2] = 2.4478e16;
    // context.masses[0] = 1;
    // context.masses[1] = 1.6601e-7;
    // context.masses[2] = 2.4478e-6;
    // context.masses[3] = 3.0035e-6;

    rb[0] = {glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0)};
    rb[1] = {glm::dvec3(4.6e5, 8.89e4, 2.87e4),
             glm::dvec3(-1.81e5, 4.97e6, 2.15e4), glm::dvec3(0, 0, 0)};
    rb[2] = {glm::dvec3(-3.8235e5, 1.4067e4, -4.558e4),
             glm::dvec3(4.79e5, -1.992e6, -6.1e5), glm::dvec3(0, 0, 0)};
    // rb[1] = {glm::dvec3(0.3075, 0.05943, 0.01917),
    //          glm::dvec3(-0.01048, 0.02877, 0.00124), glm::dvec3(0, 0, 0)};
    // rb[2] = {glm::dvec3(-0.38235, -0.14067, -0.04558),
    //          glm::dvec3(0.00479, -0.01992, -0.00061), glm::dvec3(0, 0, 0)};
    // rb[3] = {glm::dvec3(-0.17455, 0.96737, 0),
    //          glm::dvec3(-0.01742, -0.00312, 0), glm::dvec3(0, 0, 0)};

    // for (int i = 0; i < NUM; ++i) {
    //     context.masses[i] *= 1000;
    //     rb[i].v *= 1000;
    // }
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