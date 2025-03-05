#ifndef _t33nsy_RENDER
#define _t33nsy_RENDER
#include <GL/glut.h>

#include <cmath>
#include <ctime>
#include <vector>

#include "dif.h"
#define WIDTH 1000
#define HEIGHT 600

void Display();
void DrawSomething(double size);
void DrawStatic(const glm::vec3& pos, double size);
void Idle();
void Reshape(int W, int H);
void Keyboard(unsigned char Key, int MouseX, int MouseY);
void Run(int argc, char* argv[]);

#endif /* _t33nsy_RENDER */
