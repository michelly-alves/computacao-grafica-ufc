#include <GL/glut.h>
#include <math.h>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

GLfloat baseRot = 0.0f;
GLfloat seg1Rot = 0.0f;
GLfloat seg2Rot = 45.0f;
GLfloat garraDir = 30.0f;
GLfloat garraEsq = 30.0f;

GLfloat targetBaseRot = 0.0f;
GLfloat targetSeg1Rot = 0.0f;
GLfloat targetSeg2Rot = 45.0f;
GLfloat targetGarraDir = 30.0f;
GLfloat targetGarraEsq = 30.0f;

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
const float FLOOR_Y = 0.0f; 


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

// --- Lógica do Braço ---

void getClawPosition(float &clawX, float &clawY, float &clawZ) {
    // Calcula a posição da ponta da garra no espaço do mundo
    float H = 0.25f + 0.3f; 
    float L1 = 3.0f;
    float L2 = 2.0f;
    float L3 = 0.15f + 0.15f;

    float seg1_rad = seg1Rot * M_PI / 180.0f;
    float seg1_2_rad = (seg1Rot + seg2Rot) * M_PI / 180.0f;
    
    float L_Junta = 0.15f + 0.2f + 0.25f;
    
    float local_y = H + (L1 * cos(seg1_rad)) + (L2 * cos(seg1_2_rad)) + (L_Junta * cos(seg1_2_rad));
    float local_z = (L1 * sin(seg1_rad)) + (L2 * sin(seg1_2_rad)) + (L_Junta * sin(seg1_2_rad));

    // Converter para coordenadas do mundo baseado na rotação da base
    float baseRot_rad = baseRot * M_PI / 180.0f;
    clawX = local_z * sin(baseRot_rad);
    clawY = local_y;
    clawZ = local_z * cos(baseRot_rad);
}

void drawArm() {
    glPushMatrix();

        setMaterial(0.4f, 0.4f, 0.45f, 30.0f);
        drawCube(2.0, 0.5, 2.0);

        
        glTranslatef(0.0f, 0.25f, 0.0f);
        glRotatef(baseRot, 0.0f, 1.0f, 0.0f);
        setMaterial(0.1f, 0.5f, 0.8f, 50.0f);
        glutSolidTorus(0.1, 0.7, 10, 20);

        
        glTranslatef(0.0f, 0.3f, 0.0f);
        glRotatef(seg1Rot, 1.0f, 0.0f, 0.0f);
        setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
        glutSolidSphere(0.35, 20, 20);
        
        setMaterial(0.6f, 0.6f, 0.6f, 60.0f);
        glTranslatef(0.0f, 1.5f, 0.0f);
        drawY_Cylinder(0.25, 3.0, 20);

        glTranslatef(0.0f, 1.5f, 0.0f);
        glRotatef(seg2Rot, 1.0f, 0.0f, 0.0f);
        setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
        glutSolidSphere(0.3, 20, 20);

        setMaterial(0.5f, 0.5f, 0.5f, 60.0f);
        glTranslatef(0.0f, 1.0f, 0.0f);
        drawY_Cylinder(0.2, 2.0, 20);

        glTranslatef(0.0f, 1.0f, 0.0f);
        setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
        glutSolidSphere(0.2, 20, 20);

        setMaterial(0.3f, 0.3f, 0.3f, 70.0f);
        glTranslatef(0.0f, 0.15f, 0.0f);
        drawCube(0.4, 0.3, 0.4);

        if (heldObjectIndex != -1) {
            Objeto &obj = objetos[heldObjectIndex];
            glPushMatrix();

                glTranslatef(0.0f, 0.45f, 0.0f); 
                setMaterial(obj.r, obj.g, obj.b, 30.0f);
                glutSolidSphere(obj.size / 2.0f, 20, 20); 
            glPopMatrix();
        }

        glPushMatrix();
            glTranslatef(0.0f, 0.0f, 0.2f);
            setMaterial(0.8f, 0.8f, 0.8f, 80.0f);
            glutSolidSphere(0.1, 10, 10); 
            
            setMaterial(0.9f, 0.9f, 0.9f, 80.0f); 
            glTranslatef(0.0f, 0.2f, 0.0f);
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
            glTranslatef(0.0f, 0.2f, 0.0f);
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
    glEnable(GL_COLOR_MATERIAL); 
    glDisable(GL_LIGHTING);
    
    glColor3f(0.1f, 0.15f, 0.2f);
    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-10.0f, -0.25f, -10.0f);
        glVertex3f(10.0f, -0.25f, -10.0f);
        glVertex3f(10.0f, -0.25f, 10.0f);
        glVertex3f(-10.0f, -0.25f, 10.0f);
    glEnd();

    glColor3f(0.3f, 0.4f, 0.5f);
    glBegin(GL_LINES);
    for(int i = -10; i <= 10; i++) {
        glVertex3f((float)i, -0.24f, -10.0f);
        glVertex3f((float)i, -0.24f, 10.0f);
        glVertex3f(-10.0f, -0.24f, (float)i);
        glVertex3f(10.0f, -0.24f, (float)i);
    }
    glEnd();

    glEnable(GL_LIGHTING);
    glDisable(GL_COLOR_MATERIAL);
}

void drawObjetos() {
    for (int i = 0; i < NUM_OBJETOS; i++) {
        
        if (!objetos[i].isHeld) {
            glPushMatrix();
            setMaterial(objetos[i].r, objetos[i].g, objetos[i].b, 30.0f);
            glTranslatef(objetos[i].x, objetos[i].y + (objetos[i].size / 2.0f), objetos[i].z);
            glutSolidSphere(objetos[i].size / 2.0f, 20, 20); 
            glPopMatrix();
        }
    }
}

void drawShadow() {
    glPushMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glColor4f(0.1f, 0.1f, 0.1f, 0.7f); 
        
        glTranslatef(0.0f, -0.24f, 0.0f); 
        glScalef(1.0, 0.0, 1.0); 
        drawArm();
        
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(7.0, 5.0, 10.0,  
              0.0, 2.0, 0.0,   
              0.0, 1.0, 0.0);  
    
    drawFloor();
    drawObjetos();
    drawShadow();
    drawArm();

    glutSwapBuffers();
}


void update(int value) {
    float lerpSpeed = 0.05f; 
    float gravity = 0.05f;  

    baseRot += (targetBaseRot - baseRot) * lerpSpeed;
    seg1Rot += (targetSeg1Rot - seg1Rot) * lerpSpeed;
    seg2Rot += (targetSeg2Rot - seg2Rot) * lerpSpeed;
    garraDir += (targetGarraDir - garraDir) * lerpSpeed;
    garraEsq += (targetGarraEsq - garraEsq) * lerpSpeed;

    for (int i = 0; i < NUM_OBJETOS; i++) {
        if (!objetos[i].isHeld && objetos[i].y > FLOOR_Y) {
            objetos[i].y -= gravity;
            if (objetos[i].y < FLOOR_Y) {
                objetos[i].y = FLOOR_Y;
            }
        }
    }

    
    float cx, cy, cz;
    getClawPosition(cx, cy, cz);


    if (heldObjectIndex == -1 && targetGarraDir == -30.0f && fabs(garraDir - targetGarraDir) < 5.0f) {
        for (int i = 0; i < NUM_OBJETOS; i++) {
            if (!objetos[i].isHeld) {
                float objY_center = objetos[i].y + (objetos[i].size / 2.0f);
                float dx = cx - objetos[i].x;
                float dy = cy - objY_center;
                float dz = cz - objetos[i].z;
                float distSq = dx*dx + dy*dy + dz*dz;
                
                if (distSq < 0.75f) { 
                    objetos[i].isHeld = true;
                    heldObjectIndex = i;
                    break;
                }
            }
        }
    } 
    else if (heldObjectIndex != -1 && targetGarraDir == 30.0f) {
        Objeto &obj = objetos[heldObjectIndex];
        obj.isHeld = false;
        
        
        obj.x = cx;
        obj.y = cy - (obj.size / 2.0f); 
        obj.z = cz;
        
        heldObjectIndex = -1;
    }

    glutPostRedisplay();
    glutTimerFunc(16, update, 0); 
}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'a': targetBaseRot += 15.0f; break;
        case 'd': targetBaseRot -= 15.0f; break;
        case 'w': targetSeg1Rot += 10.0f; break;
        case 's': targetSeg1Rot -= 10.0f; break;
        case 'e': targetSeg2Rot += 10.0f; break;
        case 'q': targetSeg2Rot -= 10.0f; break;
        case 'z':
            targetGarraDir = -30.0f;
            targetGarraEsq = -30.0f;
            break;
        case 'x':
            targetGarraDir = 30.0f;
            targetGarraEsq = 30.0f;
            break;
    }


    if (targetSeg1Rot > 50.f) targetSeg1Rot = 50.f;
    if (targetSeg1Rot < -90.f) targetSeg1Rot = -90.f; 
    if (targetSeg2Rot > 150.f) targetSeg2Rot = 150.f; 
    if (targetSeg2Rot < 0.f) targetSeg2Rot = 0.f; 
}

void reshape(int w, int h) {
    if (h == 0) h = 1;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0f, (float)w / (float)h, 0.5f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
}

void createMenu() {
    int menu = glutCreateMenu([](int){}); 
    glutAddMenuEntry("--- Controles ---", 0);
    glutAddMenuEntry("Rotacionar base: A / D", 1);
    glutAddMenuEntry("Mover braco (ombro): W / S", 2);
    glutAddMenuEntry("Mover antebraco (cotovelo): E / Q", 3);
    glutAddMenuEntry("Abrir garra: X", 4);
    glutAddMenuEntry("Fechar garra: Z", 5);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initObjetos() {
    objetos[0] = { 2.0f, FLOOR_Y, 2.0f, 0.5f, 1.0f, 0.6f, 0.6f, false }; 
    objetos[1] = { -2.0f, FLOOR_Y, 1.0f, 0.5f, 0.6f, 1.0f, 0.6f, false }; 
    objetos[2] = { 1.5f, FLOOR_Y, -1.5f, 0.5f, 0.6f, 0.6f, 1.0f, false }; 
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
    
    GLfloat model_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, model_ambient);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);

    quadric = gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    
    initObjetos();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_ALPHA);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Braco Mecanico");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, update, 0);
    createMenu();

    glutMainLoop();
    
    gluDeleteQuadric(quadric); 
    return 0;
}