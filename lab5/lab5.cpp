#include <GL/glut.h>
#include <cmath>

/*
    Вариант 16С: треангулировать Цилиндр, настроить свет и отрисовать
*/

// Направление света
GLfloat lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
// Глобальные переменные для биндов
int segments = 30; // Начальное количество сегментов


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

// Функция для вычисления нормали на основе трёх точек
void calculateNormal(float x1, float y1, float z1, 
                     float x2, float y2, float z2, 
                     float x3, float y3, float z3, 
                     float &nx, float &ny, float &nz) {
    // Векторы AB и AC
    float ax = x2 - x1;
    float ay = y2 - y1;
    float az = z2 - z1;
    float bx = x3 - x1;
    float by = y3 - y1;
    float bz = z3 - z1;

    // Векторное произведение AB x AC
    nx = ay * bz - az * by;
    ny = az * bx - ax * bz;
    nz = ax * by - ay * bx;

    // Нормализация
    float length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length != 0.0f) {
        nx /= length;
        ny /= length;
        nz /= length;
    }
}

void drawCircleTriangles(float radius, int segments, float height, bool upward) {
    glBegin(GL_TRIANGLES);
    
    // Центральная точка
    float centerX = 0.0f, centerY = height, centerZ = 0.0f;

    // Рассчёт углов и треугольников
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        // Координаты первой точки на окружности
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);

        // Координаты второй точки на окружности
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        // Верхняя грань
		if (upward) {
        	glNormal3f(0.0f, 1.0f, 0.0f); // Нормаль направлена вверх
		} else {
			glNormal3f(0.0f,-1.0f,0.0f);  // Нормаль направлена вниз 
		}
        glVertex3f(centerX, centerY, centerZ); 
        glVertex3f(x1, centerY, z1);           // Первая точка на окружности
        glVertex3f(x2, centerY, z2);           // Вторая точка на окружности
    }
    glEnd();
}

void drawCylinder(float radius, float height, int segments) {
    // Боковые грани
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;

        // Координаты первой точки на окружности
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        // Координаты второй точки на окружности
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        // Нормаль к грани (векторное произведение)
        float nx, ny, nz;
        calculateNormal(x1, 0.0f, z1, 
                         x1, height, z1, 
                         x2, height, z2, 
                         nx, ny, nz);
        glNormal3f(nx, ny, nz); // Установка нормали

        // Боковые грани
        // Треугольник 1
        glVertex3f(x1, 0.0f, z1);  // Низ первой точки
        glVertex3f(x1, height, z1); // Верх первой точки
        glVertex3f(x2, height, z2); // Верх второй точки

        // Треугольник 2
        glVertex3f(x1, 0.0f, z1);   // Низ первой точки
        glVertex3f(x2, height, z2); // Верх второй точки
        glVertex3f(x2, 0.0f, z2);   // Низ второй точки
    }
    glEnd();

    // Верхняя грань
    drawCircleTriangles(radius, segments, height, true); // Вызов для верхней грани

    // Нижняя грань
    drawCircleTriangles(radius, segments, 0.0f, false); // Вызов для нижней грани
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Очистить буфер цвета и глубины

    // Настройка камеры
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(20.0, 20.0, 20.0,  // Положение камеры
              0.0, 0.0, 0.0,    // Точка, на которую направлена камера
              0.0, 1.0, 0.0);   // Направление "вверх"

    // Рисуем оси координат
    drawAxes();

    // Отрисовка цилиндра
    glPushMatrix();
    drawCylinder(3.0f, 5.0f, segments); // Радиус 3, высота 5, количество сегментов
    glPopMatrix();

    glutSwapBuffers();  // Переключение буферов для плавной отрисовки
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);  // Установить окно просмотра
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat)w / (GLfloat)h, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

// Функция обработки клавиш
void keyboard(unsigned char key, int x , int y) {
	(void)x;
	(void)y;
	switch (key) {
		case 'q':
			segments = std::max(3, segments -1);
			glutPostRedisplay();
			break;
		case 'e':
			segments++;
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
	}
	glutPostRedisplay();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cylinder with Normals");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
	glutMainLoop();
    return 0;
}
