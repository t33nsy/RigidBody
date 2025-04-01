#ifndef _t33nsy_DIF
#define _t33nsy_DIF
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#define G 0.0000000000667

/***
 * структура контекста
 */
struct Context {
    std::vector<double> masses;
    std::vector<double> radiuses;
    std::vector<glm::dmat3x3> Ibodies;
    std::vector<glm::dmat3x3> Ibodies_inv;
};

/***
 * структура тела
 */
struct RigidBody {
    glm::dvec3 r;    // position
    glm::dvec3 f;    // force
    glm::dvec3 P;    // linear momentum
    glm::dvec3 L;    // angular momentum
    glm::dmat3x3 I;  // inertia tensor
    glm::dquat q;    // quaternion
};

/***
 * Функция вычисления производной
 * @param rb массив тел
 * @param num количество тел
 * @param time текущее время
 * @param c контекст
 */
std::vector<RigidBody> f_rigidbody(const std::vector<RigidBody>& rb, int num,
                                   double time, const Context& c);

/***
 * Функция вычисления силы между двумя телами
 * @param r1 позиция первого тела
 * @param m1 масса первого тела
 * @param r2 позиция второго тела
 * @param m2 масса второго тела
 * @param d расстояние между телами
 */
glm::dvec3 force(const glm::dvec3& r1, const double m1, const glm::dvec3& r2,
                 const double m2);

/***
 * Сумма составляющих двух тел
 * @param a первое тело
 * @param b второе тело
 */
RigidBody sum(const RigidBody& a, const RigidBody& b);

/***
 * Сумма составляющих двух массивов тел
 * @param a первый массив тел
 * @param b второй массив тел
 * @param num количество тел
 */
std::vector<RigidBody> vec_sum(const std::vector<RigidBody>& a,
                               const std::vector<RigidBody>& b, int num);

/***
 * Оператор сложения тел
 * @param a первое тело
 * @param b второе тело
 */
RigidBody operator+(const RigidBody& a, const RigidBody& b);

/***
 * Функция умножения тела на число
 * @param a первое тело
 * @param num число
 */
RigidBody mul(const RigidBody& a, double num);

/***
 * Оператор умножения тела на число
 * @param a первое тело
 * @param b число
 */
RigidBody operator*(const RigidBody& a, double b);

/***
 * Функция умножения массива тел на число
 * @param a массив тел
 * @param b число
 */
std::vector<RigidBody> vec_mul(const std::vector<RigidBody>& a, double b,
                               int num);

/***
 * Оператор сложения массивов тел
 * @param a первый массив тел
 * @param b второй массив тел
 */
std::vector<RigidBody> operator+(const std::vector<RigidBody>& a,
                                 const std::vector<RigidBody>& b);

/***
 * Оператор умножения массива тел на число
 * @param a массив тел
 * @param b число
 */
std::vector<RigidBody> operator*(const std::vector<RigidBody>& a, double b);

/***
 * Функция выделения памяти под константы
 * @param c контекст
 * @param num количество тел
 */
void contextReserve(Context& c, int num);

/***
 * Функция генерации тензоров инерции тел
 * @param c контекст
 * @param num количество тел
 */
void createInertiaTensors(Context& c, int num);

/***
 * Функция совершения шага методом Рунге-Кутты 4-го порядка
 * @param rb массив тел
 * @param num количество тел
 * @param h шаг
 * @param cur_time текущее время
 * @param c контекст
 */
double step(std::vector<RigidBody>& rb, int num, double h, double cur_time,
            Context c);
#endif /* _t33nsy_DIF */
