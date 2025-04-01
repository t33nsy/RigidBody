#ifndef _t33nsy_RENDER
#define _t33nsy_RENDER
#include <GL/glut.h>

#include <cmath>
#include <ctime>
#include <fstream>
#include <vector>

#include "dif.h"
#define WIDTH 1000
#define HEIGHT 600

/***
 * Функция обработки отрисовки
 */
void Display();

/***
 * Функция обработки прорисовки объектов
 */
void Idle();

/***
 * Функция обработки изменения размера окна
 */
void Reshape(int W, int H);

/***
 * Функция обработки нажатия клавиш
 */
void Keyboard(unsigned char Key, int MouseX, int MouseY);

/***
 * Функция запуска программы
 * @param argc Количество аргументов командной строки
 * @param argv Аргументы командной строки
 */
void Run(int argc, char* argv[]);

#endif /* _t33nsy_RENDER */
