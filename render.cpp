#include "render.h"

double OldTime = -1, DeltaTime;
std::vector<RigidBody> rb(4);
Context context;
double cameraAngle = 0.0f;
double cameraPitch = 0.0f;
double cameraDistance = 100.0f;
#define SIZE 1e6
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

    gluLookAt(1e11, 1e11, 1e11,  // Позиция камеры
              0, 0, 0,           // Точка, на которую смотрим
              0, 1, 1);          // Направление "вверх"

    // Обновление состояния
    step(rb, 4, DeltaTime, OldTime, context);
    std::cout << "ASTEROID: (" << rb[0].r.x << ", " << rb[0].r.y << ", "
              << rb[0].r.z << ")\n";

    DrawStatic(rb[0].r, SIZE);
    DrawStatic(rb[1].r, SIZE);
    DrawStatic(rb[2].r, SIZE);
    DrawStatic(rb[3].r, SIZE);

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
    context.masses[0] = 1e12;
    context.masses[1] = 5.972e24;
    context.masses[2] = 1.898e27;
    context.masses[3] = 5.683e26;

    rb[0] = {glm::dvec3(0, 0, 0), glm::dvec3(0, 30000, 0), glm::dvec3(0, 0, 0)};
    rb[1] = {glm::dvec3(1.496e11, 0, 0), glm::dvec3(0, 29780, 0),
             glm::dvec3(0, 0, 0)};
    rb[2] = {glm::dvec3(7.785e11, 0, 0), glm::dvec3(0, 13070, 0),
             glm::dvec3(0, 0, 0)};
    rb[3] = {glm::dvec3(1.429e12, 0, 0), glm::dvec3(0, 9690, 0),
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