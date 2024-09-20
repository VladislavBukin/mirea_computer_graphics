#include <GL/glut.h>

// Координаты вершин трапеции (в плоскости Z = 0)
GLfloat vertices[4][3] = {
    {-5.0f, 0.0f, 0.0f},  // Нижняя левая вершина
    {5.0f, 0.0f, 0.0f},   // Нижняя правая вершина
    {3.0f, 4.0f, 0.0f},    // Верхняя правая вершина
    {-3.0f, 4.0f, 0.0f}    // Верхняя левая вершина
};

// Цвета для вершин
GLfloat colors[4][3] = {
    {1.0f, 0.0f, 0.0f},  // Красный
    {0.0f, 1.0f, 0.0f},  // Зеленый
    {0.0f, 0.0f, 1.0f},  // Синий
    {1.0f, 1.0f, 0.0f}   // Желтый
};

// Нормали к вершинам
GLfloat normals[4][3] = {
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f},
    {0.0f, 0.0f, 1.0f}
};

// Направление света
GLfloat lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};

void init() {
    glEnable(GL_DEPTH_TEST);  // Включить тест глубины
    glEnable(GL_LIGHTING);    // Включить освещение
    glEnable(GL_LIGHT0);      // Включить источник света 0

    // Установка света
    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};  // Белый свет
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);

    glEnable(GL_COLOR_MATERIAL);  // Включить цвет материала
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    
    glEnable(GL_NORMALIZE);  // Нормализация векторов нормалей
    glShadeModel(GL_SMOOTH);  // Гладкая интерполяция цвета
}

void drawAxes() {
    glDisable(GL_LIGHTING);  // Отключаем освещение для осей координат

    glBegin(GL_LINES);
    
    // Ось X (красная)
    glColor3f(1.0f, 0.0f, 0.0f);  
    glVertex3f(-10.0f, 0.0f, 0.0f);
    glVertex3f(10.0f, 0.0f, 0.0f);

    // Ось Y (зеленая)
    glColor3f(0.0f, 1.0f, 0.0f);  
    glVertex3f(0.0f, -10.0f, 0.0f);
    glVertex3f(0.0f, 10.0f, 0.0f);

    // Ось Z (синяя)
    glColor3f(0.0f, 0.0f, 1.0f);  
    glVertex3f(0.0f, 0.0f, -10.0f);
    glVertex3f(0.0f, 0.0f, 10.0f);

    glEnd();

    glEnable(GL_LIGHTING);  // Включаем освещение обратно
}
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Очистить буфер цвета и глубины

    // Настройка камеры
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(20.0, 20.0, 20.0,  // Положение камеры ближе к объекту
              0.0, 0.0, 0.0,    // Точка, на которую направлена камера
              0.0, 1.0, 0.0);   // Направление "вверх"

    // Рисуем оси координат
	drawAxes();

	// Отрисовка трапеции как двух треугольников
    glBegin(GL_TRIANGLES);
    
    // Первый треугольник
    glColor3fv(colors[0]);
    glNormal3fv(normals[0]);
    glVertex3fv(vertices[0]);  // Нижняя левая

    glColor3fv(colors[1]);
    glNormal3fv(normals[1]);
    glVertex3fv(vertices[1]);  // Нижняя правая

    glColor3fv(colors[2]);
    glNormal3fv(normals[2]);
    glVertex3fv(vertices[2]);  // Верхняя правая

    // Второй треугольник
    glColor3fv(colors[0]);
    glNormal3fv(normals[0]);
    glVertex3fv(vertices[0]);  // Нижняя левая

    glColor3fv(colors[2]);
    glNormal3fv(normals[2]);
    glVertex3fv(vertices[2]);  // Верхняя правая

    glColor3fv(colors[3]);
    glNormal3fv(normals[3]);
    glVertex3fv(vertices[3]);  // Верхняя левая

    glEnd();

    glutSwapBuffers();  // Переключение буферов для плавной отрисовки
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);  // Установить окно просмотра
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 500.0);  // Параметры перспективы
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Trapezoid with Lighting");

    init();  // Инициализация

    glutDisplayFunc(display);  // Функция отображения
    glutReshapeFunc(reshape);  // Функция изменения размера окна

    glutMainLoop();  // Главный цикл обработки событий
    return 0;
}
