#include <GL/glut.h>
#include <cmath>

/*
    Вариант 16С: треангулировать Цилиндр, настроить свет и отрисовать
*/

// Направление света
GLfloat lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
// Глобальные переменные для биндов
int segments = 30; // Начальное количество сегментов
bool axes_flag = true;
float cameraAngleX = 0.0f; // Угол вращения вокруг оси X
float cameraAngleY = 0.0f; // Угол вращения вокруг оси Y
float cameraDistance = 20.0f; // Расстояние камеры от центра
float lightAngleX = 0.0f; // Угол вращения вокруг оси X
float lightAngleY = 0.0f; // Угол вращения вокруг оси Y
float height = 5.0f;
float radius = 3.0f;

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

    glShadeModel(GL_SMOOTH);  // Гладкая интерполяция цвета
}

void drawAxes() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Очистить буфер цвета и глубины
    glDisable(GL_LIGHTING);  // Отключаем освещение для осей координат

    glBegin(GL_LINES);
    // Ось X (красная)
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1000.0f, 0.0f, 0.0f);
    glVertex3f(1000.0f, 0.0f, 0.0f);
    // Ось Y (зеленая)
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1000.0f, 0.0f);
    glVertex3f(0.0f, 1000.0f, 0.0f);
    // Ось Z (синяя)
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -1000.0f);
    glVertex3f(0.0f, 0.0f, 1000.0f);
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

    // Позиция камеры на основе углов
    float cameraX = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    float cameraY = cameraDistance * sin(cameraAngleX);
    float cameraZ = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
    gluLookAt(cameraX, cameraY, cameraZ,   // Положение камеры
              0.0, 0.0, 0.0,              // Точка, на которую направлена камера
              0.0, 1.0, 0.0);             // Направление "вверх"

    GLfloat lightPosition[] = {
        (GLfloat)(10.0f * cos(lightAngleY) * cos(lightAngleX)), // Приведение к GLfloat
        (GLfloat)(10.0f * sin(lightAngleX)),                     // Приведение к GLfloat
        (GLfloat)(10.0f * sin(lightAngleY) * cos(lightAngleX)), // Приведение к GLfloat
        1.0f                                                     // Вектор направления
    };

    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); // Установка позиции света

	
    // Рисуем оси координат
    if (axes_flag){
	drawAxes();
	}
    // Отрисовка цилиндра
    glPushMatrix();
    drawCylinder(radius, height, segments); // Радиус 3, высота 5, количество сегментов
    glPopMatrix();

    glutSwapBuffers();  // Переключение буферов для плавной отрисовки
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);  // Установка размера окна
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / height, 1.0, 100.0); // Установка перспективы
}

void keyboard(unsigned char key, int x, int y) {
	(void)x;
	(void)y;
    switch (key) {
        case 'a': // Влево
            cameraAngleY -= 0.1f;
            break;
        case 'd': // Вправо
            cameraAngleY += 0.1f;
            break;
        case 'w': // Вверх
            cameraAngleX += 0.1f;
            break;
        case 's': // Вниз
            cameraAngleX -= 0.1f;
            break;
	    case 'q':
    		segments = std::max(3, segments -1);
      		glutPostRedisplay();
        	break;
    	case 'e':
      		segments++;
      		glutPostRedisplay();
      		break;
		case 'r':
			height++;
			glutPostRedisplay();
			break;
		case 'f':
			height = std::max(1.0f,height -1);
			glutPostRedisplay();
			break;
		case 't':
			radius = radius + 1.0f;
			glutPostRedisplay();
			break;
		case 'g':
			radius = std::max(1.0f,radius -1);
			glutPostRedisplay();
			break;
		case 'z':
			cameraDistance = std::max(1.0f, cameraDistance - 1);
			break;
		case 'x':
			cameraDistance++;
			break;
    	case 'i': 
            lightAngleX += 0.1f;
            break;
        case 'k':
            lightAngleX -= 0.1f;
            break;
        case 'l':
            lightAngleY -= 0.1f;
            break;
        case 'j':
            lightAngleY += 0.1f;
            break;
		case 'c':
			axes_flag = !axes_flag;
			break;
		case 27:
      		exit(0);
    }
    glutPostRedisplay(); // Обновление окна
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Инициализация GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Настройка режима отображения
    glutInitWindowSize(800, 600); // Размер окна
    glutCreateWindow("Цилиндр с вращением камеры"); // Создание окна
    init(); // Инициализация
    glutDisplayFunc(display); // Установка функции отображения
    glutReshapeFunc(reshape); // Установка функции изменения размера
    glutKeyboardFunc(keyboard); // Установка функции обработки клавиатуры
    glutMainLoop(); // Запуск основного цикла
    return 0;
}
