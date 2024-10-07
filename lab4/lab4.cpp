#include <GL/glut.h>
#include <cmath>
/*
 Вариант 4
 основание фигуры - трапеция
 Параметры траектории: x^2 - 2xy + 2y^2 - 4 = 0, z = - 10 
*/
// Координаты вершин первой трапеции (первая фигура)
GLfloat baseVertices1[4][3] = {
    {-1.0f, -0.5f, 0.0f},  // Нижняя левая вершина
    {1.0f, -0.5f, 0.0f},   // Нижняя правая вершина
    {0.8f, 0.5f, 0.0f},    // Верхняя правая вершина
    {-0.8f, 0.5f, 0.0f}    // Верхняя левая вершина
};

// Верхние вершины для первой трапеции
GLfloat topVertices1[4][3];

// Координаты вершин второй трапеции (вторая фигура)
GLfloat baseVertices2[4][3] = {
    {-0.5f, -0.2f, 0.5f},  // Нижняя левая вершина
    {1.5f, -0.2f, 0.5f},   // Нижняя правая вершина
    {1.3f, 0.8f, 0.5f},    // Верхняя правая вершина
    {-0.3f, 0.8f, 0.5f}    // Верхняя левая вершина
};

// Верхние вершины для второй трапеции
GLfloat topVertices2[4][3];

// Цвета для обеих фигур
GLfloat colors1[4][3] = {
    {1.0f, 0.0f, 0.0f},  // Красный для первой фигуры
    {0.0f, 1.0f, 0.0f},  // Зеленый для первой фигуры
    {0.0f, 0.0f, 1.0f},  // Синий для первой фигуры
    {1.0f, 1.0f, 0.0f}   // Желтый для первой фигуры
};

GLfloat colors2[4][3] = {
    {0.0f, 1.0f, 1.0f},  // Голубой для второй фигуры
    {1.0f, 0.0f, 1.0f},  // Фиолетовый для второй фигуры
    {0.5f, 0.5f, 0.5f},  // Серый для второй фигуры
    {1.0f, 0.5f, 0.0f}   // Оранжевый для второй фигуры
};

// Параметры для анимации камеры
GLfloat t = 0.0f;
GLfloat camera_pos[3] = {0.0f, 0.0f, -10.0f}; // Начальное положение камеры
GLfloat lightPos[4] = {0.0f, 0.0f, 10.0f, 1.0f}; // Начальное положение света

// Функция для вычисления положения камеры по траектории
void update_camera_position() {
    camera_pos[0] = 2 * cos(t) + 2 * sin(t);  // x(t) = 2 * cos(t) + 2* sin(t)
    camera_pos[1] = 2 * sin(t);               // y(t) = 2 * sin(t)
    camera_pos[2] = -10.0f;                   // z остается постоянным

    t += 0.01f;
    if (t > 2 * M_PI) {
        t -= 2 * M_PI;
    }

    glutPostRedisplay();
}

void init() {
    // Инициализация верхних вершин трапеций (сдвиг по оси Z)
    for (int i = 0; i < 4; i++) {
        topVertices1[i][0] = baseVertices1[i][0];
        topVertices1[i][1] = baseVertices1[i][1];
        topVertices1[i][2] = baseVertices1[i][2] + 1.0f;  // Верхняя часть первой трапеции на уровне Z=1

        topVertices2[i][0] = baseVertices2[i][0];
        topVertices2[i][1] = baseVertices2[i][1];
        topVertices2[i][2] = baseVertices2[i][2] + 1.0f;  // Верхняя часть второй трапеции на уровне Z=1.5
    }

    glEnable(GL_DEPTH_TEST);    // Включаем тест глубины
    glDepthFunc(GL_LESS);       // Разрешаем отрисовку только тех фрагментов, которые ближе к камере
    glEnable(GL_LIGHTING);      // Включаем освещение
    glEnable(GL_LIGHT0);        // Включаем источник света

    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Белый цвет освещения
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);     // Позиция света

    glEnable(GL_COLOR_MATERIAL);   // Включаем цвет материала
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);   // Нормализуем нормали для корректного освещения
    glShadeModel(GL_SMOOTH);  // Гладкое освещение
}

// Функция для отрисовки трапеции и боковых граней
void drawPrism(GLfloat baseVertices[4][3], GLfloat topVertices[4][3], GLfloat colors[4][3]) {
    // Рисуем основание (нижнюю часть трапеции)
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor3fv(colors[i]);
        glVertex3fv(baseVertices[i]);
    }
    glEnd();

    // Рисуем верхнюю часть трапеции
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor3fv(colors[i]);
        glVertex3fv(topVertices[i]);
    }
    glEnd();

    // Рисуем боковые грани
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        glColor3fv(colors[i]);
        glVertex3fv(baseVertices[i]);
        glVertex3fv(baseVertices[next]);
        glVertex3fv(topVertices[next]);
        glVertex3fv(topVertices[i]);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Очищаем буфер цвета и глубины

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Устанавливаем камеру
    gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2],  // Положение камеры
              0.0, 0.0, 0.0,   // Точка, на которую направлена камера
              0.0, 1.0, 0.0);  // Направление "вверх"

    // Устанавливаем свет
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    // Рисуем первую трапецию
    drawPrism(baseVertices1, topVertices1, colors1);

    // Рисуем вторую трапецию
    drawPrism(baseVertices2, topVertices2, colors2);

    glutSwapBuffers();  // Меняем буферы местами для двойной буферизации
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 500.0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Overlapping Trapezoidal Prisms");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update_camera_position);

    glutMainLoop();
    return 0;
}
