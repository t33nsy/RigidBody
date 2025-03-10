#include "render.h"

double OldTime = -1, DeltaTime;
std::vector<RigidBody> rb(4);
Context context;
double cameraAngle = 0.0f;
double cameraPitch = 0.0f;
double cameraDistance = 100.0f;
#define SIZE 0.1
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

    gluLookAt(rb[0].r.x-5, rb[0].r.y-5, rb[0].r.z-5,  // Позиция камеры
              rb[0].r.x, rb[0].r.y, rb[0].r.z,           // Точка, на которую смотрим
              0, -1, 0);          // Направление "вверх"

    // Обновление состояния
    step(rb, 4, DeltaTime, OldTime, context);
    if (fl < 30){
        std::cout << "SUN: (" << rb[0].r.x << ", " << rb[0].r.y << ", "
        << rb[0].r.z << ")\n";
        std::cout << "p1: (" << rb[1].r.x << ", " << rb[1].r.y << ", "
        << rb[1].r.z << ")\n";
        std::cout << "p2: (" << rb[2].r.x << ", " << rb[2].r.y << ", "
        << rb[2].r.z << ")\n";
        std::cout << "p3: (" << rb[3].r.x << ", " << rb[3].r.y << ", "
        << rb[3].r.z << ")\n";
    }
    ++fl;

    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslated(rb[0].r.x, rb[0].r.y, rb[0].r.z);
    glutWireSphere(SIZE, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslated(rb[1].r.x, rb[1].r.y, rb[1].r.z);
    glutWireSphere(SIZE, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslated(rb[2].r.x, rb[2].r.y, rb[2].r.z);
    glutWireSphere(SIZE, 20, 20);
    glPopMatrix();
    glPushMatrix();
    glTranslated(rb[3].r.x, rb[3].r.y, rb[3].r.z);
    glutWireSphere(SIZE, 20, 20);
    glPopMatrix();

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
    context.masses.resize(4);
    context.masses[0] = 1.989e30;
    context.masses[1] = 3.301e24;
    context.masses[2] = 4.867e23;
    context.masses[3] = 5.972e24;

    rb[0] = {glm::dvec3(0, 0.03, 0.01), glm::dvec3(0, 0, 0), glm::dvec3(0, 0, 0)};
    rb[1] = {glm::dvec3(0.39, 0.03, 0), glm::dvec3(0, 6, 0),
             glm::dvec3(0, 0, 0)};
    rb[2] = {glm::dvec3(0.72, 0, 0), glm::dvec3(0, 10, 0),
             glm::dvec3(0, 0, 0)};
    rb[3] = {glm::dvec3(1, 0, 0), glm::dvec3(0, 8, 0),
             glm::dvec3(0, 0, 0)};
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