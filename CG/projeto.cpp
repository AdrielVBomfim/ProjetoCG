/*
    Projeto "Eu, avião"

    Integrantes:    Adriel Venancio Bomfim
                    João Vitor Oliveira Cordeiro
                    Lucas Almeida Francisco


    Argumentos: 2 (raio da terra) 3.5 (posição x do sol) 1.0 (posicao y do sol) 2.35 (posicao z do sol)
 */
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

//Variáveis de posição da câmera
float posCameraX,posCameraY,posCameraZ;

//Argumento correspondente ao raio do planeta
GLdouble arg1;
//Argumentos correspondentes à posição x, y e z do Sol
GLfloat arg2, arg3, arg4;
//posicao da luz do sol
GLfloat luz_sol[] = {arg2, arg3, arg4, 1.0};
//posicao da luz ambiente
GLfloat luz_ambiente[] = {0.0, 0.0, 0.0, 1.0};

void iluminar(){

    //LUZ 0

    //define características para a fonte de luz 0
    //fonte de luz direcional (por que w==0?)
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    //atribui características para a fonte de luz 0
    //cor padrão: branco
    glLightfv(GL_LIGHT0, GL_POSITION, luz_sol);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);

    //LUZ 1
    //fonte de luz ambiente
    GLfloat light1_ambient[] = { 0.1, 0.1, 0.1, 1.0 };

    //atribui as características para a fonte de luz 1
    //(experimentem remover alguns dos componentes abaixo)
    glLightfv(GL_LIGHT1, GL_POSITION, luz_ambiente);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);

    //"acende" cada uma das luzes configuradas
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void init(void)
{
    glClearColor (0.0, 0.0, 0.0, 0.0);

    iluminar();
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);

    //inicializa posição da câmera
    posCameraX = 0.3;
    posCameraY = 0.1;
    posCameraZ = 0;
}


// Rotaciona a posição da camera em torno do eixo y
void specialKeys(int key, int x, int y)
{
    float angulo = 2*M_PI/180;
    switch (key) {
        case GLUT_KEY_LEFT :
            posCameraX =  posCameraX*cos(-angulo) + posCameraZ*sin(-angulo);
            posCameraZ = -posCameraX*sin(-angulo) + posCameraZ*cos(-angulo);
            break;
        case GLUT_KEY_RIGHT :
            posCameraX =  posCameraX*cos(angulo) + posCameraZ*sin(angulo);
            posCameraZ = -posCameraX*sin(angulo) + posCameraZ*cos(angulo);
            break;
    }
    glutPostRedisplay();
}

void display()
{
    GLfloat mat_diffuse[] = { 0.0, 0.4, 0.0, 1.0 };

    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    gluLookAt (posCameraX, posCameraY, posCameraZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glPushAttrib (GL_LIGHTING_BIT);

    //atribui característica difusa ao material
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

    glEnable(GL_LIGHTING);

    glColor3f (0.0, 0.4, 0.0);
    glutSolidSphere(0.65, 25, 25);

    glPopAttrib();

    glBegin(GL_LINES);
    glColor3f (1.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);

    glColor3f (0.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);

    glColor3f (0.0, 0.0, 1.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 1.0);
    glEnd();

    glTranslatef(arg2, arg3, arg4);
    glColor3f(1.0, 0.65, 0.0);
    glutSolidSphere(0.05, 25, 25);

    glLightfv(GL_LIGHT0, GL_POSITION, luz_sol);
    glLightfv(GL_LIGHT1, GL_POSITION, luz_ambiente);

    glPopMatrix();

    //troca de buffers, o flush é implícito aqui
    glutSwapBuffers();
}

//função chamada quando a tela é redimensionada
void reshape(int w, int h)
{
    glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char** argv)
{
    arg1 = std::atof(argv[1]);
    arg2 = std::atof(argv[2]);
    arg3 = std::atof(argv[3]);
    arg4 = std::atof(argv[4]);

    //printf("%f %f %f %f", &arg1, &arg2, &arg3, &arg4);

    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (500, 500);
    glutInitWindowPosition (100, 100);
    glutCreateWindow ("Eu, Aviao");

    init ();

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    glutMainLoop();

    return 0;
}
