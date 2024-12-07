#include <GL/glut.h>
#include <cmath>
#include <cstdio>

/*
    Вариант 16С: треангулировать Цилиндр, настроить свет и отрисовать
    Изменения: добавлено создание и наложение шахматной текстуры на цилиндр.
*/

GLfloat lightDir[] = {0.0f, 0.0f, 1.0f, 0.0f};
int segments = 30; 
bool axes_flag = true;
bool arrow_flag = true;
float cameraAngleX = 0.0f; 
float cameraAngleY = 0.0f; 
float cameraDistance = 20.0f; 
float lightAngleX = 0.0f; 
float lightAngleY = 0.0f; 
float lightDirAngleX = 0.0f; 
float lightDirAngleY = 0.0f; 
float height = 5.0f;
float radius = 3.0f;

// ---------- Новые переменные для текстуры ----------
#define TEX_SIZE 64
GLubyte textureData[TEX_SIZE][TEX_SIZE][3]; 
GLuint textureID; 

void createCheckerTexture() {
    for (int i = 0; i < TEX_SIZE; i++) {
        for (int j = 0; j < TEX_SIZE; j++) {
            int c = ((i/8 + j/8) % 2) * 255; 
            textureData[i][j][0] = (GLubyte)c;
            textureData[i][j][1] = (GLubyte)c;
            textureData[i][j][2] = (GLubyte)c;
        }
    }
}

void init() {
    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_LIGHTING);    
    glEnable(GL_LIGHT0);      

    GLfloat lightColor[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 45.0f); 
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0f); 
    GLfloat lightDirInit[] = {0.0f, 0.0f, -1.0f};
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDirInit);
    GLfloat lightPosition[] = {0.0f, 5.0f, 5.0f, 1.0f}; 
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glEnable(GL_COLOR_MATERIAL);  
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glShadeModel(GL_SMOOTH);  
    
    // ---------- Создание и настройка текстуры ----------
    glEnable(GL_TEXTURE_2D); 
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    createCheckerTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_SIZE, TEX_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, textureData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void drawAxes() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  
    glDisable(GL_LIGHTING);  
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
    glEnable(GL_LIGHTING);
}

void calculateNormal(float x1, float y1, float z1, 
                     float x2, float y2, float z2, 
                     float x3, float y3, float z3, 
                     float &nx, float &ny, float &nz) {
    float ax = x2 - x1;
    float ay = y2 - y1;
    float az = z2 - z1;
    float bx = x3 - x1;
    float by = y3 - y1;
    float bz = z3 - z1;

    nx = ay * bz - az * by;
    ny = az * bx - ax * bz;
    nz = ax * by - ay * bx;

    float length = sqrt(nx * nx + ny * ny + nz * nz);
    if (length != 0.0f) {
        nx /= length;
        ny /= length;
        nz /= length;
    }
}

void renderText(float x, float y, const char* text) {
    glDisable(GL_LIGHTING);  
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);  

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glRasterPos2f(x, y); 
    for (const char* c = text; *c != '\0'; ++c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c); 
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawArrow(GLfloat lightPosition[], GLfloat lightDirection[]) {
    glDisable(GL_LIGHTING);
    GLfloat arrowEnd[3] = {
        lightPosition[0] + lightDirection[0] * 5.0f,
        lightPosition[1] + lightDirection[1] * 5.0f,
        lightPosition[2] + lightDirection[2] * 5.0f
    };

    glBegin(GL_LINES);
    glColor3f(1.0f, 1.0f, 0.0f); 
    glVertex3f(lightPosition[0], lightPosition[1], lightPosition[2]);
    glVertex3f(arrowEnd[0], arrowEnd[1], arrowEnd[2]);
    glEnd();

    glPushMatrix();
    glTranslatef(arrowEnd[0], arrowEnd[1], arrowEnd[2]); 
    GLfloat dirLength = sqrt(lightDirection[0]*lightDirection[0] +
                             lightDirection[1]*lightDirection[1] +
                             lightDirection[2]*lightDirection[2]);

    if (dirLength > 0.0f) {
        GLfloat normalizedDir[3] = {
            lightDirection[0] / dirLength,
            lightDirection[1] / dirLength,
            lightDirection[2] / dirLength
        };

        GLfloat angleZ = (GLfloat)(acos(normalizedDir[2]) * 180.0 / M_PI);
        GLfloat axisX = -normalizedDir[1]; 
        GLfloat axisY = normalizedDir[0]; 
        glRotatef(angleZ, axisX, axisY, 0.0f);
    }

    glColor3f(1.0f, 0.0f, 0.0f); 
    glutSolidCone(0.2, 0.5, 10, 10);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawCircleTriangles(float radius, int segments, float h, bool upward) {
    glBegin(GL_TRIANGLES);
    float centerX = 0.0f, centerY = h, centerZ = 0.0f;
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);
        if (upward) {
            glNormal3f(0.0f, 1.0f, 0.0f);
        } else {
            glNormal3f(0.0f,-1.0f,0.0f);
        }

        // Для крышек тоже зададим текстурные координаты радиально
        // Центр крышки
        glTexCoord2f(0.5f, 0.5f); 
        glVertex3f(centerX, centerY, centerZ);

        // Точка 1
        float u1 = 0.5f + (x1/(radius*2.0f));
        float v1 = 0.5f + (z1/(radius*2.0f));
        glTexCoord2f(u1, v1);
        glVertex3f(x1, centerY, z1);

        // Точка 2
        float u2 = 0.5f + (x2/(radius*2.0f));
        float v2 = 0.5f + (z2/(radius*2.0f));
        glTexCoord2f(u2, v2);
        glVertex3f(x2, centerY, z2);
    }
    glEnd();
}

void drawCylinder(float radius, float height, int segments) {
    // Боковые грани
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; ++i) {
        float angle1 = 2.0f * M_PI * i / segments;
        float angle2 = 2.0f * M_PI * (i + 1) / segments;
        float x1 = radius * cos(angle1);
        float z1 = radius * sin(angle1);
        float x2 = radius * cos(angle2);
        float z2 = radius * sin(angle2);

        float nx, ny, nz;
        calculateNormal(x1, 0.0f, z1, 
                        x1, height, z1, 
                        x2, height, z2, 
                        nx, ny, nz);
        glNormal3f(nx, ny, nz);

        // Текстурные координаты по окружности:
        // u: от 0 до 1 вдоль угла, v: от 0 до 1 вдоль высоты
        float u1 = (float)i / (float)segments;
        float u2 = (float)(i+1) / (float)segments;

        // Треугольник 1
        glTexCoord2f(u1, 0.0f);
        glVertex3f(x1, 0.0f, z1);

        glTexCoord2f(u1, 1.0f);
        glVertex3f(x1, height, z1);

        glTexCoord2f(u2, 1.0f);
        glVertex3f(x2, height, z2);

        // Треугольник 2
        glTexCoord2f(u1, 0.0f);
        glVertex3f(x1, 0.0f, z1);

        glTexCoord2f(u2, 1.0f);
        glVertex3f(x2, height, z2);

        glTexCoord2f(u2, 0.0f);
        glVertex3f(x2, 0.0f, z2);
    }
    glEnd();

    // Верхняя грань
    drawCircleTriangles(radius, segments, height, true);
    // Нижняя грань
    drawCircleTriangles(radius, segments, 0.0f, false);
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / height, 1.0, 100.0); 
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float cameraX = cameraDistance * cos(cameraAngleY) * cos(cameraAngleX);
    float cameraY = cameraDistance * sin(cameraAngleX);
    float cameraZ = cameraDistance * sin(cameraAngleY) * cos(cameraAngleX);
    gluLookAt(cameraX, cameraY, cameraZ,  
              0.0, 0.0, 0.0,             
              0.0, 1.0, 0.0);            

    GLfloat lightPosition[] = {
        (GLfloat)(10.0f * cos(lightAngleY) * cos(lightAngleX)),
        (GLfloat)(10.0f * sin(lightAngleX)),
        (GLfloat)(10.0f * sin(lightAngleY) * cos(lightAngleX)),
        1.0f 
    };

    GLfloat dynamicLightDir[] = {
        (GLfloat)(cos(lightDirAngleY) * cos(lightDirAngleX)),
        (GLfloat)(sin(lightDirAngleX)),
        (GLfloat)(sin(lightDirAngleY) * cos(lightDirAngleX))
    };

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); 
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dynamicLightDir); 

    if (axes_flag) {
        drawAxes();
    }

    glColor3f(1.0f,1.0f,1.0f); // Цвет материала (теперь текстура будет главной)
    glBindTexture(GL_TEXTURE_2D, textureID);

    glPushMatrix();
    drawCylinder(radius, height, segments); 
    glPopMatrix();

    if (arrow_flag) {
        drawArrow(lightPosition, dynamicLightDir);
    }

    char buffer[50];
    sprintf(buffer, "Segments: %d",segments);
    renderText(10,580,buffer);
    glutSwapBuffers();  
}

void keyboard(unsigned char key, int x, int y) {
    (void)x;
    (void)y;
    switch (key) {
        case 'a': cameraAngleY -= 0.1f; break;
        case 'd': cameraAngleY += 0.1f; break;
        case 'w': cameraAngleX += 0.1f; break;
        case 's': cameraAngleX -= 0.1f; break;
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
            lightAngleX += 0.1f; break;
        case 'k':
            lightAngleX -= 0.1f; break;
        case 'l':
            lightAngleY -= 0.1f; break;
        case 'j':
            lightAngleY += 0.1f; break;
        case 'y': 
            lightDirAngleX += 0.1f; break;
        case 'u':
            lightDirAngleX -= 0.1f; break;
        case 'o':
            lightDirAngleY -= 0.1f; break;
        case 'p':
            lightDirAngleY += 0.1f; break;
        case 'c':
            axes_flag = !axes_flag;
            break;
        case 'v':
            arrow_flag = !arrow_flag;
            break;
        case 27:
            exit(0);
    }
    glutPostRedisplay(); 
}

int main(int argc, char** argv) {
    glutInit(&argc, argv); 
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); 
    glutInitWindowSize(800, 600); 
    glutCreateWindow("Цилиндр с текстурой шахматного поля"); 
    init(); 
    glutDisplayFunc(display); 
    glutReshapeFunc(reshape); 
    glutKeyboardFunc(keyboard); 
    glutMainLoop(); 
    return 0;
}
