#include <GL/glut.h>
#include <cmath>

#define TEXTURE_WIDTH 64
#define TEXTURE_HEIGHT 64

GLubyte checkerboard[TEXTURE_HEIGHT][TEXTURE_WIDTH][3];  // Шахматная текстура

// Координаты вершин нижней трапеции
GLfloat baseVertices[4][3] = {
    {-1.0f, -0.5f, 0.0f},  // Нижняя левая вершина
    {1.0f, -0.5f, 0.0f},   // Нижняя правая вершина
    {0.8f, 0.5f, 0.0f},    // Верхняя правая вершина
    {-0.8f, 0.5f, 0.0f}    // Верхняя левая вершина
};

// Текстурные координаты для вершин
GLfloat textureCoords[4][2] = {
    {0.0f, 0.0f},   // Нижняя левая вершина
    {1.0f, 0.0f},   // Нижняя правая вершина
    {1.0f, 1.0f},   // Верхняя правая вершина
    {0.0f, 1.0f}    // Верхняя левая вершина
};

// Верхняя трапеция
GLfloat topVertices[4][3];

// Параметры для анимации
GLfloat t = 0.0f;
GLfloat camera_pos[3] = {0.0f, 0.0f, -10.0f}; // Начальное положение камеры
GLfloat lightPos[4] = {0.0f, 0.0f, -10.0f, 1.0f}; // Начальное положение света

GLuint texture;

// Параметры материала
GLfloat matDiffuse[] = {0.8f, 0.2f, 0.2f, 1.0f}; // Рассеянный свет
GLfloat matSpecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Спекулярный свет
GLfloat matShininess[] = {50.0f}; // Яркость

void createCheckerboardTexture() {
    // Генерация шахматной текстуры (черно-белая)
    for (int i = 0; i < TEXTURE_HEIGHT; i++) {
        for (int j = 0; j < TEXTURE_WIDTH; j++) {
            int c = (((i & 8) == 0) ^ ((j & 8) == 0)) * 255;
            checkerboard[i][j][0] = (GLubyte) c;  // Красный канал
            checkerboard[i][j][1] = (GLubyte) c;  // Зеленый канал
            checkerboard[i][j][2] = (GLubyte) c;  // Синий канал
        }
    }

    // Генерация текстуры
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Загрузка текстуры в OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, checkerboard);
}

void update_camera_position() {
    camera_pos[0] = 2 * cos(t) + 2 * sin(t);
    camera_pos[1] = 2 * sin(t);
    camera_pos[2] = -10.0f;
    
    // Анимация источника света
    lightPos[0] = 2 * cos(t) + 2 * sin(t);
    lightPos[1] = 2 * sin(t);
    t += 0.01f;
    if (t > 2 * M_PI) {
        t -= 2 * M_PI;
    }
    glutPostRedisplay();
}

void init() {
    for (int i = 0; i < 4; i++) {
        topVertices[i][0] = baseVertices[i][0] + 1.0f;
        topVertices[i][1] = baseVertices[i][1] + 1.0f;
        topVertices[i][2] = baseVertices[i][2] + 5.0f;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // Включаем текстурирование
    glEnable(GL_TEXTURE_2D);
    createCheckerboardTexture();
}

// Вычисление нормали на основе векторов
void computeNormal(GLfloat* v1, GLfloat* v2, GLfloat* v3, GLfloat* normal) {
    GLfloat u[3], v[3];

    // Вектор u = v2 - v1
    u[0] = v2[0] - v1[0];
    u[1] = v2[1] - v1[1];
    u[2] = v2[2] - v1[2];

    // Вектор v = v3 - v1
    v[0] = v3[0] - v1[0];
    v[1] = v3[1] - v1[1];
    v[2] = v3[2] - v1[2];

    // Векторное произведение u и v для получения нормали
    normal[0] = u[1] * v[2] - u[2] * v[1];
    normal[1] = u[2] * v[0] - u[0] * v[2];
    normal[2] = u[0] * v[1] - u[1] * v[0];

    // Нормализация нормали
    GLfloat length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length != 0) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
}

void drawPrism() {
    GLfloat normal[3];

    // Рисуем нижнюю трапецию с текстурными координатами и нормалью
    computeNormal(baseVertices[0], baseVertices[1], baseVertices[2], normal);
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    glNormal3fv(normal); // Нормаль для нижней грани
    for (int i = 0; i < 4; i++) {
        glTexCoord2fv(textureCoords[i]);  // Текстурные координаты
        glVertex3fv(baseVertices[i]);     // Нижняя трапеция
    }
    glEnd();

    // Рисуем верхнюю трапецию с текстурными координатами и нормалью
    computeNormal(topVertices[0], topVertices[1], topVertices[2], normal);
    glBegin(GL_QUADS);
    glNormal3fv(normal); // Нормаль для верхней грани
    for (int i = 0; i < 4; i++) {
        glTexCoord2fv(textureCoords[i]);  // Те же текстурные координаты
        glVertex3fv(topVertices[i]);      // Верхняя трапеция
    }
    glEnd();

    // Рисуем боковые грани призмы с нормалями
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        
        // Вычисляем нормаль для боковой грани
        computeNormal(baseVertices[i], baseVertices[next], topVertices[next], normal);
        glNormal3fv(normal); // Устанавливаем нормаль для боковой грани
        
        // Задаем текстурные координаты для боковых граней
        glTexCoord2f(0.0f, 0.0f);
        glVertex3fv(baseVertices[i]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex3fv(baseVertices[next]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex3fv(topVertices[next]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3fv(topVertices[i]);
    }
    glEnd();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Устанавливаем камеру
    gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2],  
              0.0, 0.0, 0.0,   
              0.0, 1.0, 0.0);

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
    glutCreateWindow("3D Prism with Checkerboard Texture");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update_camera_position);

    glutMainLoop();
    return 0;
}
