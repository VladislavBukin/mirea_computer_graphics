#include <GL/glut.h>
#include <cmath>

// Координаты вершин нижней трапеции
GLfloat baseVertices[4][3] = {
    {-1.0f, -0.5f, 0.0f},  // Нижняя левая вершина
    {1.0f, -0.5f, 0.0f},   // Нижняя правая вершина
    {0.8f, 0.5f, 0.0f},    // Верхняя правая вершина
    {-0.8f, 0.5f, 0.0f}    // Верхняя левая вершина
};

// Цвета вершин
GLfloat colors[4][3] = {
    {1.0f, 0.0f, 0.0f},  // Красный
    {0.0f, 1.0f, 0.0f},  // Зеленый
    {0.0f, 0.0f, 1.0f},  // Синий
    {1.0f, 1.0f, 0.0f}   // Желтый
};

// Верхняя трапеция (полученная путем трансляции)
GLfloat topVertices[4][3];

// Параметры для анимации
GLfloat t = 0.0f;
GLfloat camera_pos[3] = {0.0f, 0.0f, -10.0f}; // Начальное положение камеры
GLfloat lightPos[4] = {0.0f, 0.0f, -10.0f, 1.0f}; // Начальное положение света

// Функция для вычисления положения камеры по траектории
void update_camera_position() {
    // Параметрические уравнения для траектории движения камеры
    camera_pos[0] = 2 * cos(t); // x(t) = 2 * cos(t)
    camera_pos[1] = sin(t);     // y(t) = sin(t)
    camera_pos[2] = -10.0f;     // z остается постоянным

    // Изменение параметра t для анимации
    t += 0.01f;
    if (t > 2 * M_PI) {
        t -= 2 * M_PI;
    }

    glutPostRedisplay();
}

void init() {
    // Инициализация верхней трапеции с трансляцией
    for (int i = 0; i < 4; i++) {
        topVertices[i][0] = baseVertices[i][0] + 1.0f;  // Трансляция x
        topVertices[i][1] = baseVertices[i][1] + 1.0f;  // Трансляция y
        topVertices[i][2] = baseVertices[i][2] + 5.0f;  // Трансляция z
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);
}

void drawPrism() {
    // Рисуем нижнюю и верхнюю трапеции
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor3fv(colors[i]);
        glVertex3fv(baseVertices[i]);  // Нижняя трапеция
    }
    glEnd();

    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glColor3fv(colors[i]);
        glVertex3fv(topVertices[i]);   // Верхняя трапеция
    }
    glEnd();

    // Рисуем боковые грани призмы
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Устанавливаем камеру
    gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2],  // Положение камеры
              0.0, 0.0, 0.0,   // Точка, на которую направлена камера
              0.0, 1.0, 0.0);  // Направление "вверх"

    // Устанавливаем свет
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    drawPrism();

    glutSwapBuffers();
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
    glutCreateWindow("3D Prism with Camera Animation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update_camera_position);

    glutMainLoop();
    return 0;
}
