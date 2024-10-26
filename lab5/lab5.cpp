#include <GL/glut.h>
#include <cmath>
/*
	Вариант 16С: треангулировать Целиндр, настроить свет и отрисовать
*/
// Координаты вершин трапеции (в плоскости Z = 0)

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

void drawCircleManualTriangulation(float radius, int segments) {
	glDisable(GL_LIGHTING);
	glColor3f(1.0,0.5f,0.0f);

	float centerX = 0.0, centerY = 0.0f;
	float angleStep = 2.0f * 3.14159f / segments;

	glBegin(GL_TRIANGLES);
	for (int i = 0; i < segments; ++i){
		float angle1 = i * angleStep;
		float angle2 = (i+1) * angleStep;
		float x1 = radius * cos(angle1);
		float y1 = radius * sin(angle1);
		float x2 = radius * cos(angle2);
		float y2 = radius * sin(angle2);

		glVertex3f(centerX, centerY, 0.0f);
		glVertex3f(x1,y1,0.0f);
		glVertex3f(x2,y2,0.0f);
	}
	glEnd();

	glEnable(GL_LIGHTING);
}


void drawCircle(float radius, int segments) {
    glDisable(GL_LIGHTING);  // Отключаем освещение для круга
    glColor3f(1.0f, 0.5f, 0.0f);  // Оранжевый цвет круга

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 0.0f);  // Центральная точка круга

    for (int i = 0; i <= segments; ++i) {
        float angle = 2.0f * 3.14159f * i / segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex3f(x, y, 0.0f);
    }
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

    // Отрисовка круга
    glPushMatrix();
    drawCircleManualTriangulation(3.0f, 1000000);  // Радиус 3, 50 сегментов
    glPopMatrix();

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
    glutCreateWindow("Trapezoid and Circle with Lighting");

    init();  // Инициализация

    glutDisplayFunc(display);  // Функция отображения
    glutReshapeFunc(reshape);  // Функция изменения размера окна

    glutMainLoop();  // Главный цикл обработки событий
    return 0;
}
