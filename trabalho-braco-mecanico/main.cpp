#include <GL/glut.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLfloat baseRot = 0.0f;
GLfloat seg1Rot = 0.0f;
GLfloat seg2Rot = 45.0f;
GLfloat garraDir = 30.0f;
GLfloat garraEsq = 30.0f;

GLUquadric* quadric;

struct Objeto {
    float x, y, z;
    float size;
    float r, g, b;
    bool isHeld;
};

#define NUM_OBJETOS 3
Objeto objetos[NUM_OBJETOS];
int heldObjectIndex = -1;

void setMaterial(GLfloat r, GLfloat g, GLfloat b, GLfloat shininess) {
    GLfloat ambient[] = {r * 0.2f, g * 0.2f, b * 0.2f, 1.0f};
    GLfloat diffuse[] = {r, g, b, 1.0f};
    GLfloat specular[] = {0.8f, 0.8f, 0.8f, 1.0f};
    
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}

void drawY_Cylinder(float radius, float height, int slices) {
    glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(0.0f, 0.0f, -height / 2.0f);
        gluCylinder(quadric, radius, radius, height, slices, slices);
        gluDisk(quadric, 0, radius, slices, 1);
        glTranslatef(0.0f, 0.0f, height);
        gluDisk(quadric, 0, radius, slices, 1);
    glPopMatrix();
}

void drawCube(float x, float y, float z) {
    glPushMatrix();
        glScalef(x, y, z);
        glutSolidCube(1.0);
    glPopMatrix();
}

void getClawPosition(float &clawX, float &clawY, float &clawZ) {
    float H = 0.25f + 0.3f;
    float L1 = 3.0f;
    float L2 = 2.0f;
    float L3 = 0.15f;

    float seg1_rad = seg1Rot * M_PI / 180.0f;
    float seg1_2_rad = (seg1Rot + seg2Rot) * M_PI / 180.0f;
    
    float local_y = H + (L1 * cos(seg1_rad)) + (L2 * cos(seg1_2_rad)) + (L3 * cos(seg1_2_rad));
    float local_z = (L1 * sin(seg1_rad)) + (L2 * sin(seg1_2_rad)) + (L3 * sin(seg1_2_rad));

    float baseRot_rad = baseRot * M_PI / 180.0f;
    clawX = local_z * sin(baseRot_rad);
    clawY = local_y;
    clawZ = local_z * cos(baseRot_rad);
}

void drawArm() {
    glPushMatrix();
        setMaterial(0.6f, 0.6f, 0.65f, 30.0f);
        drawCube(2.0, 0.5, 2.0);

        glTranslatef(0.0f, 0.25f, 0.0f);
        glRotatef(baseRot, 0.0f, 1.0f, 0.0f);
        setMaterial(0.8f, 0.3f, 0.1f, 50.0f);
        glutSolidTorus(0.1, 0.7, 10, 20);

        glTranslatef(0.0f, 0.3f, 0.0f);
        glRotatef(seg1Rot, 1.0f, 0.0f, 0.0f);
        
        setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
        glutSolidSphere(0.35, 20, 20);
        
        setMaterial(1.0f, 0.5f, 0.0f, 60.0f);
        glTranslatef(0.0f, 1.5f, 0.0f);
        drawY_Cylinder(0.25, 3.0, 20);

        glTranslatef(0.0f, 1.5f, 0.0f);
        glRotatef(seg2Rot, 1.0f, 0.0f, 0.0f);

        setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
        glutSolidSphere(0.3, 20, 20);

        setMaterial(1.0f, 0.6f, 0.2f, 60.0f);
        glTranslatef(0.0f, 1.0f, 0.0f);
        drawY_Cylinder(0.2, 2.0, 20);

        glTranslatef(0.0f, 1.0f, 0.0f);

        setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
        glutSolidSphere(0.2, 20, 20);

        setMaterial(0.5f, 0.5f, 0.5f, 70.0f);
        glTranslatef(0.0f, 0.15f, 0.0f);
        drawCube(0.4, 0.3, 0.4);

        if (heldObjectIndex != -1) {
            Objeto &obj = objetos[heldObjectIndex];
            glPushMatrix();
            glTranslatef(0.0f, -obj.size / 2.0f, 0.0f);
            setMaterial(obj.r, obj.g, obj.b, 30.0f);
            glutSolidSphere(obj.size, 20, 20); 
            glPopMatrix();
        }

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.2f);
            
            setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
            glutSolidSphere(0.1, 10, 10);
            
            setMaterial(0.9f, 0.9f, 0.9f, 80.0f);
            glTranslatef(0.0f, 0.2f, 0.1f);
            drawCube(0.1, 0.5, 0.1);

            glTranslatef(0.0f, 0.25f, 0.0f);
            
            glRotatef(garraEsq, 1.0f, 0.0f, 0.0f);
            glRotatef(-20.0f, 1.0f, 0.0f, 0.0f);
            
            glTranslatef(0.0f, 0.25f, 0.0f);
            drawCube(0.1, 0.5, 0.1);
        glPopMatrix();
        
        glPushMatrix();
            glTranslatef(0.0f, 0.0f, -0.2f);
            
            setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
            glutSolidSphere(0.1, 10, 10);
            
            setMaterial(0.9f, 0.9f, 0.9f, 80.0f);
            glTranslatef(0.0f, 0.2f, -0.1f);
            drawCube(0.1, 0.5, 0.1);

            glTranslatef(0.0f, 0.25f, 0.0f);
            
            glRotatef(garraDir, -1.0f, 0.0f, 0.0f);
            glRotatef(20.0f, 1.0f, 0.0f, 0.0f);
            
            glTranslatef(0.0f, 0.25f, 0.0f);
            drawCube(0.1, 0.5, 0.1);
        glPopMatrix();

    glPopMatrix();
}


void drawFloor() {
    glDisable(GL_LIGHTING);
    
    glColor3f(0.2f, 0.25f, 0.25f);
    glBegin(GL_QUADS);
        glVertex3f(-10.0f, -0.25f, -10.0f);
        glVertex3f(10.0f, -0.25f, -10.0f);
        glVertex3f(10.0f, -0.25f, 10.0f);
        glVertex3f(-10.0f, -0.25f, 10.0f);
    glEnd();

    glColor3f(0.4f, 0.4f, 0.4f);
    glBegin(GL_LINES);
    for(int i = -10; i <= 10; i++) {
        glVertex3f((float)i, -0.24f, -10.0f);
        glVertex3f((float)i, -0.24f, 10.0f);
        glVertex3f(-10.0f, -0.24f, (float)i);
        glVertex3f(10.0f, -0.24f, (float)i);
    }
    glEnd();

    glEnable(GL_LIGHTING);
}

void drawObjetos() {
    for (int i = 0; i < NUM_OBJETOS; i++) {
        if (!objetos[i].isHeld) {
            glPushMatrix();
            setMaterial(objetos[i].r, objetos[i].g, objetos[i].b, 30.0f);
            glTranslatef(objetos[i].x, objetos[i].y, objetos[i].z);
            glutSolidSphere(objetos[i].size / 2.0f, 20, 20); 
            glPopMatrix();
        }
    }
}

void drawShadow() {
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        
        glColor4f(0.1f, 0.1f, 0.1f, 0.8f);
        
        glTranslatef(0.0f, -0.23f, 0.0f);
        glScalef(1.0, 0.0, 1.0);
        drawArm();
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(7.0, 4.0, 10.0,
              0.0, 2.0, 0.0,
              0.0, 1.0, 0.0);
    
    drawFloor();
    drawObjetos();
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawShadow();
    glDisable(GL_BLEND);

    drawArm();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': baseRot += 5.0f; break;
        case 'd': baseRot -= 5.0f; break;
        case 'w': seg1Rot += 5.0f; break;
        case 's': seg1Rot -= 5.0f; break;
        case 'e': seg2Rot += 5.0f; break;
        case 'q': seg2Rot -= 5.0f; break;
        case 'z':
            if (garraDir > -30.0f) { garraDir -= 2.0f; garraEsq -= 2.0f; }
            
            if (heldObjectIndex == -1) {
                float cx, cy, cz;
                getClawPosition(cx, cy, cz);
                
                for (int i = 0; i < NUM_OBJETOS; i++) {
                    if (!objetos[i].isHeld) {
                        float dx = cx - objetos[i].x;
                        float dy = cy - objetos[i].y;
                        float dz = cz - objetos[i].z;
                        float distSq = dx*dx + dy*dy + dz*dz;
                        
                        if (distSq < 0.5f) {
                            objetos[i].isHeld = true;
                            heldObjectIndex = i;
                            break;
                        }
                    }
                }
            }
            break;
        case 'x':
            if (garraDir < 30.0f) { garraDir += 2.0f; garraEsq += 2.0f; }
            
            if (heldObjectIndex != -1) {
                float cx, cy, cz;
                getClawPosition(cx, cy, cz);
                
                objetos[heldObjectIndex].isHeld = false;
                objetos[heldObjectIndex].x = cx;
                objetos[heldObjectIndex].y = objetos[heldObjectIndex].size / 2.0f; 
                objetos[heldObjectIndex].z = cz;
                heldObjectIndex = -1;
            }
            break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, (float)w / (float)h, 0.5f, 200.0f);
}

void createMenu() {
    int menu = glutCreateMenu([](int){});
    glutAddMenuEntry("--- Controles ---", 0);
    glutAddMenuEntry("Rotacionar base: A / D", 1);
    glutAddMenuEntry("Mover braco (ombro): W / S", 2);
    glutAddMenuEntry("Mover antebraco (cotovelo): E / Q", 3);
    glutAddMenuEntry("Abrir garra: X (Soltar)", 4);
    glutAddMenuEntry("Fechar garra: Z (Pegar)", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initObjetos() {
    objetos[0] = { 2.0f, 0.25f, 2.0f, 0.5f, 1.0f, 0.6f, 0.6f, false }; 
    objetos[1] = { -2.0f, 0.25f, 1.0f, 0.5f, 0.6f, 1.0f, 0.6f, false }; 
    objetos[2] = { 1.5f, 0.25f, -1.5f, 0.5f, 0.6f, 0.6f, 1.0f, false }; 
}

void init() {
    glEnable(GL_DEPTH_TEST);

    GLfloat light_pos[] = {5.0f, 10.0f, 10.0f, 1.0f};
    GLfloat light_diffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat light_ambient[] = {0.3f, 0.3f, 0.3f, 1.0f};
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    
    glShadeModel(GL_SMOOTH);

    quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    initObjetos();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Braço Mecânico Estilizado");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    createMenu();

    glutMainLoop();
    
    gluDeleteQuadric(quadric); 
    return 0;
}

