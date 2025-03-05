#include "render.h"

double OldTime = -1, DeltaTime;
RigidBody rb;
Context context;
double cameraAngle = 0.0f;
double cameraPitch = 0.0f;
double cameraDistance = 1.0f;
// Create new frame in memory and draw it in window
void Display() {
    // clear frame
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float cameraX = cameraDistance * cos(cameraPitch * M_PI / 180.0f) *
                    sin(cameraAngle * M_PI / 180.0f);
    float cameraY = cameraDistance * sin(cameraPitch * M_PI / 180.0f);
    float cameraZ = cameraDistance * cos(cameraPitch * M_PI / 180.0f) *
                    cos(cameraAngle * M_PI / 180.0f);

    // Устанавливаем камеру
    gluLookAt(cameraX, cameraY, cameraZ,  // Позиция камеры
              0, 0, 0,                    // Точка, на которую смотрим
              0, 1, 0);                   // Направление "вверх"
    step(rb, context, DeltaTime, OldTime);
    // put buffer from memory to screen

    glPushMatrix();
    glTranslated(rb.r.x, rb.r.y, rb.r.z);
    // glRotated(0, 0, 0, 0);
    glColor3f(1, 0.0f, 0.0f);
    DrawSomething(0.01);
    glPopMatrix();

    DrawStatic(context.pos1, 0.015);
    DrawStatic(context.pos2, 0.015);
    DrawStatic(context.pos3, 0.015);
    DrawStatic(context.pos4, 0.015);
    DrawStatic(rb.r, 0.005);

    glFlush();
    glutSwapBuffers();
}

void DrawStatic(const glm::vec3& pos, double size) {
    glPushMatrix();
    glColor3f(0.0f, 0.0f, 0.0f);
    glTranslated(pos.x, pos.y, pos.z);
    DrawSomething(size);
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
    context.m1 = 300000000000;
    context.m2 = 300000000000;
    context.m3 = 300000000000;
    context.m4 = 300000000000;
    context.pos1 = {0.15, 0, 0};
    context.pos2 = {0.3, 0, 0};
    context.pos3 = {0.2, 0.15, 0};
    context.pos4 = {0.15, 0.15, 0};
    context.mass = 330000000000;

    rb.r = {0, 0, 0};
    rb.v = {0, 0, 0};

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
    glClearColor(0.3, 0.5, 0.7, 0);

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