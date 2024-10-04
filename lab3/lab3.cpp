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
    camera_pos[0] = 2 * cos(t) + 2*sin(t);
    camera_pos[1] = 2 * sin(t);
    camera_pos[2] = -10.0f;
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

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    // Включаем текстурирование
    glEnable(GL_TEXTURE_2D);
    createCheckerboardTexture();
}

void drawPrism() {
    // Рисуем нижнюю трапецию с текстурными координатами
    glBindTexture(GL_TEXTURE_2D, texture);
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2fv(textureCoords[i]);  // Текстурные координаты
        glVertex3fv(baseVertices[i]);     // Нижняя трапеция
    }
    glEnd();

    // Рисуем верхнюю трапецию с текстурными координатами
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        glTexCoord2fv(textureCoords[i]);  // Те же текстурные координаты
        glVertex3fv(topVertices[i]);      // Верхняя трапеция
    }
    glEnd();

// Рисуем боковые грани призмы с новыми текстурными координатами
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;

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
