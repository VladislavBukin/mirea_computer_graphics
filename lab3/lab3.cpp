#include <GL/glut.h>
#include <cmath>

// Текстура ID
GLuint textureID;

// Координаты вершин нижней трапеции
GLfloat baseVertices[4][3] = {
    {-1.0f, -0.5f, 0.0f},  // Нижняя левая вершина
    {1.0f, -0.5f, 0.0f},   // Нижняя правая вершина
    {0.8f, 0.5f, 0.0f},    // Верхняя правая вершина
    {-0.8f, 0.5f, 0.0f}    // Верхняя левая вершина
};

// Верхняя трапеция (полученная путем трансляции)
GLfloat topVertices[4][3];

// Параметры для анимации
GLfloat t = 0.0f;
GLfloat camera_pos[3] = {0.0f, 0.0f, -10.0f}; // Начальное положение камеры
GLfloat lightPos[4] = {0.0f, 0.0f, -10.0f, 1.0f}; // Начальное положение света

// Вычисление нормали по трём точкам
void calculateNormal(GLfloat v1[3], GLfloat v2[3], GLfloat v3[3], GLfloat normal[3]) {
    GLfloat u[3], v[3];

    u[0] = v2[0] - v1[0];
    u[1] = v2[1] - v1[1];
    u[2] = v2[2] - v1[2];

    v[0] = v3[0] - v1[0];
    v[1] = v3[1] - v1[1];
    v[2] = v3[2] - v1[2];

    // Векторное произведение U x V
    normal[0] = u[1] * v[2] - u[2] * v[1];
    normal[1] = u[2] * v[0] - u[0] * v[2];
    normal[2] = u[0] * v[1] - u[1] * v[0];

    // Нормализация нормали
    GLfloat length = sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
    if (length != 0.0f) {
        normal[0] /= length;
        normal[1] /= length;
        normal[2] /= length;
    }
}

// Загрузка текстуры с диска
void loadTexture(const char* filePath) {
    glEnable(GL_TEXTURE_2D);
    textureID = SOIL_load_OGL_texture(filePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
    if (textureID == 0) {
        printf("Ошибка загрузки текстуры: %s\n", SOIL_last_result());
        exit(1);
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

// Инициализация
void init(const char* texturePath) {
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

    loadTexture(texturePath); // Загрузка текстуры
}

// Отрисовка призмы с наложением текстуры и вычислением нормалей
void drawPrism() {
    GLfloat normal[3];

    // Нижняя трапеция
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBegin(GL_QUADS);
    calculateNormal(baseVertices[0], baseVertices[1], baseVertices[2], normal);
    glNormal3fv(normal);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(baseVertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(baseVertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(baseVertices[2]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(baseVertices[3]);
    glEnd();

    // Верхняя трапеция
    glBegin(GL_QUADS);
    calculateNormal(topVertices[0], topVertices[1], topVertices[2], normal);
    glNormal3fv(normal);
    glTexCoord2f(0.0f, 0.0f); glVertex3fv(topVertices[0]);
    glTexCoord2f(1.0f, 0.0f); glVertex3fv(topVertices[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex3fv(topVertices[2]);
    glTexCoord2f(0.0f, 1.0f); glVertex3fv(topVertices[3]);
    glEnd();

    // Боковые грани
    glBegin(GL_QUADS);
    for (int i = 0; i < 4; i++) {
        int next = (i + 1) % 4;
        calculateNormal(baseVertices[i], baseVertices[next], topVertices[next], normal);
        glNormal3fv(normal);
        glTexCoord2f(0.0f, 0.0f); glVertex3fv(baseVertices[i]);
        glTexCoord2f(1.0f, 0.0f); glVertex3fv(baseVertices[next]);
        glTexCoord2f(1.0f, 1.0f); glVertex3fv(topVertices[next]);
        glTexCoord2f(0.0f, 1.0f); glVertex3fv(topVertices[i]);
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

void update_camera_position() {
    camera_pos[0] = 2 * cos(t);
    camera_pos[1] = sin(t);
    camera_pos[2] = -10.0f;
    t += 0.01f;
    if (t > 2 * M_PI) {
        t -= 2 * M_PI;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("Использование: %s <путь к изображению>\n", argv[0]);
        return -1;
    }

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Textured Prism with Image");

    init(argv[1]);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(update_camera_position);

    glutMainLoop();
    return 0;
}
