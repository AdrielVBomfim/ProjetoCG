/*
    Projeto "Eu, avião"

    Integrantes:    Adriel Venancio Bomfim
                    João Vitor Oliveira Cordeiro
                    Lucas Almeida Francisco


    Argumentos: 0.5 (raio da terra) 1 (posição x do sol) 1 (posicao y do sol) 1 (posicao z do sol)
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include <limits>

//ESFERA
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>
#include "Bmp.h"
#include "Sphere.h"

//ÁUDIO
#include "include/irrKlang.h"

irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

Sphere *spherePtr;
GLuint texId, texIdSun, texUniverse, texMoon;

//Renderizacao do universo ao fundo
Sphere universe(5, 100, 100, false);

//Variaveis de posição da câmera
float posCameraX, posCameraY, posCameraZ;
float pontoRefX, pontoRefY, pontoRefZ;
float raioCamera;

//Variaveis da lua
float anguloMoon = 0.0;
float velocidadeLua = 0.5;

//Variaveis de fluidez do simulador
float velocidade = 0.5;
double rot_min = M_PI/180;
double rot_min_up = (M_PI/180)*2;
float ALT_MIN;
float ALT_MAX;
float VEL_CRUZEIRO = 0.5;
float VEL_MAX = 1;

//Controles de pouso e decolagem
bool decolar, pousar;

//Argumento correspondente ao raio do planeta
GLdouble arg1;
//Argumentos correspondentes à posição x, y e z do Sol
GLfloat luz_pontual[] = {1.0, 1.0, 1.0, 1.0};


void iluminar(){
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    //cor padrão: branco
    glLightfv(GL_LIGHT0, GL_POSITION, luz_pontual);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glEnable(GL_LIGHT0);
}

//Desenha o sol na posicao da fonte de luz
void desenhar_luz(){
    glPushAttrib (GL_LIGHTING_BIT);
    GLfloat mat_diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat mat_emission[] = { 1.0, 1.0, 0.0, 1.0 };

    //atribui características ao material
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glPushMatrix();
    glTranslatef(luz_pontual[0],luz_pontual[1],luz_pontual[2]);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glRotatef(-90, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, texIdSun);
    Sphere sun(0.1f, 60, 60, false);
    sun.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_LIGHTING);
    glPopAttrib();
    glPopMatrix();
}


//Desenha eixos xyz
void desenhar_eixos(){
    glEnable(GL_LIGHTING);
    //não há efeitos de iluminação nos eixos
    glLineWidth(3);
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
    glEnd(); glDisable(GL_LIGHTING);
}

//Desenha o planeta
void desenhar_planeta(){
    //MATERIAL
    //define características para aparência do material
    //GLfloat mat_specular[] = { 1, 1, 1, 1.0 };
    GLfloat mat_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    glPushAttrib (GL_LIGHTING_BIT);
    //atribui características ao material
    //glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);
    glRotatef(-90, 1, 0, 0);
    glBindTexture(GL_TEXTURE_2D, texId);
    spherePtr->draw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_LIGHTING);
    glPopAttrib();
}

//Desenha esfera com textura de estrelas + via láctea
void desenhar_universe(){
    glPushAttrib (GL_LIGHTING_BIT);
    GLfloat mat_diffuse[] = { 1.0, 0.0, 1.0, 1.0 };
    GLfloat mat_emission[] = { 1.0, 1.0, 1.0, 1.0 };
    //atribui características ao material
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glPushMatrix();
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texUniverse);
    universe.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_LIGHTING);
    glPopAttrib();
    glPopMatrix();
}
//Desenha a lua
void desenhar_moon(){
    glPushAttrib (GL_LIGHTING_BIT);
    GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_emission[] = { 0.0, 0.0, 0.0, 1.0 };
    //atribui características ao material
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);
    glPushMatrix();
    GLdouble posMoon = arg1 + 0.08;
    glRotatef(anguloMoon, 0, 1, 0);
    glTranslatef(posMoon,0,posMoon);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texMoon);
    Sphere moon(0.04f, 60, 60, false);
    moon.draw();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_LIGHTING);
    glPopAttrib();
    glPopMatrix();
}

//Funcao que recalcula as posicoes do aviao
void spin(void){

    if(decolar){
     if(raioCamera < ALT_MAX) {
         raioCamera += 0.001;
         velocidade += 0.01;
         if(velocidade > VEL_CRUZEIRO) velocidade = VEL_CRUZEIRO;
     }
     else {
         decolar = false;
         raioCamera = ALT_MAX;
     }
    }if(pousar){
        if(raioCamera > ALT_MIN) {
            raioCamera -= 0.001;
            velocidade -= 0.01;
            if(velocidade < 0) velocidade = 0;
        }
        else {
            raioCamera = ALT_MIN;
            pousar = false;
            velocidade = 0;
        }
    }

    // Vetor na direcao do center da camera
    GLdouble versorDesl[] = {pontoRefX-posCameraX, pontoRefY-posCameraY, pontoRefZ-posCameraZ};
    // Vetor na direcao do giro - produto vetorial entre o up e o gase
    GLdouble vetDir[] = {posCameraY*versorDesl[2]-posCameraZ*versorDesl[1],
                         posCameraZ*versorDesl[0]-posCameraX*versorDesl[2],
                         posCameraX*versorDesl[1]-posCameraY*versorDesl[0]};
    GLdouble fator_norm = sqrt(pow(vetDir[0],2)+pow(vetDir[1],2)+pow(vetDir[2],2));
    vetDir[0] /= fator_norm;
    vetDir[1] /= fator_norm;
    vetDir[2] /= fator_norm;

    GLdouble x = vetDir[0], y = vetDir[1], z = vetDir[2];
    GLdouble c = cos(rot_min*velocidade), s = sin(rot_min*velocidade);
    GLdouble mat_lin1[] = {x*x*(1-c)+c,  x*y*(1-c)-z*s,  x*z*(1-c)+y*s};
    GLdouble mat_lin2[] = {y*x*(1-c)+z*s, y*y*(1-c)+c, y*z*(1-c)-x*s};
    GLdouble mat_lin3[] = {x*z*(1-c)-y*s, y*z*(1-c)+x*s, z*z*(1-c)+c};
    posCameraX = posCameraX*mat_lin1[0]+posCameraY*mat_lin1[1]+posCameraZ*mat_lin1[2];
    posCameraY = posCameraX*mat_lin2[0]+posCameraY*mat_lin2[1]+posCameraZ*mat_lin2[2];
    posCameraZ = posCameraX*mat_lin3[0]+posCameraY*mat_lin3[1]+posCameraZ*mat_lin3[2];

    pontoRefX = pontoRefX*mat_lin1[0]+pontoRefY*mat_lin1[1]+pontoRefZ*mat_lin1[2];
    pontoRefY = pontoRefX*mat_lin2[0]+pontoRefY*mat_lin2[1]+pontoRefZ*mat_lin2[2];
    pontoRefZ = pontoRefX*mat_lin3[0]+pontoRefY*mat_lin3[1]+pontoRefZ*mat_lin3[2];

    GLdouble norma = sqrt(pow(posCameraX,2)+pow(posCameraY,2)+pow(posCameraZ,2));
    GLdouble eixo_trans[] = {posCameraX/norma, posCameraY/norma, posCameraZ/norma};

    posCameraX = eixo_trans[0]*raioCamera;
    posCameraY = eixo_trans[1]*raioCamera;
    posCameraZ = eixo_trans[2]*raioCamera;

    GLdouble norma2 = sqrt(pow(pontoRefX,2)+pow(pontoRefY,2)+pow(pontoRefZ,2));
    GLdouble eixo_trans2[] = {pontoRefX/norma2, pontoRefY/norma2, pontoRefZ/norma2};

    pontoRefX = eixo_trans2[0]*raioCamera;
    pontoRefY = eixo_trans2[1]*raioCamera;
    pontoRefZ = eixo_trans2[2]*raioCamera;

    std::cout << sqrt(posCameraX*posCameraX+posCameraY*posCameraY+posCameraZ*posCameraZ) << "\n";
    std::cout << "center: " << sqrt(pontoRefX*pontoRefX+pontoRefY*pontoRefY+pontoRefZ*pontoRefZ) << "\n";
    std::cout << "vel: " << velocidade << "\n";

    anguloMoon += velocidadeLua;
    if(anguloMoon >= 360)
        anguloMoon = 0;
    glutPostRedisplay();
}

void init(void)
{
    engine->play2D("media/sound/starscape.wav", GL_TRUE);
    glClearColor (0.0, 0.0, 0.0, 0.0);

    //configura iluminação
    iluminar();

    // ativa teste de profundidade
    // o que acontece se isso não for feito?
    glEnable(GL_DEPTH_TEST);
    glShadeModel (GL_SMOOTH);

    //posCameraX = 0.3;
    //posCameraY = 0.1;
    //posCameraZ = 0;

    raioCamera = arg1 + 0.05;

    posCameraX = raioCamera;
    posCameraY = 0.0;
    posCameraZ = 0.0;

    pontoRefX = raioCamera;//raioCamera * cos(teta) * sin(fi + angulo);
    pontoRefY = 0.1;
    pontoRefZ = 0.1;// * cos(fi + angulo);

}

void specialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP){                 //Acelerar
        if(raioCamera > ALT_MIN) velocidade += 0.1;
        if(velocidade > VEL_MAX) velocidade = VEL_MAX;
    }
    else if(key == GLUT_KEY_DOWN){         //Desacelerar
        velocidade -= 0.1;
        if (velocidade < std::numeric_limits<float>::epsilon()) velocidade = 0;
        if (velocidade == 0) pousar = true;
    }
    else if(key == GLUT_KEY_PAGE_UP){           //Decolar
        if(pousar == true)
            pousar = false;

        decolar = true;
    }
    else if(key == GLUT_KEY_PAGE_DOWN){         //Aterrissar
        if(decolar == true)
            decolar = false;

        pousar = true;
    }
    else if(key == GLUT_KEY_LEFT){          // Girar para a esquerda
        GLdouble x = posCameraX, y = posCameraY, z = posCameraZ;
        GLdouble c = cos(rot_min_up*velocidade), s = sin(rot_min_up*velocidade);
        GLdouble mat_lin1[] = {x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s};
        GLdouble mat_lin2[] = {y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s};
        GLdouble mat_lin3[] = {x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c};

        pontoRefX = pontoRefX * mat_lin1[0] + pontoRefY * mat_lin1[1] + pontoRefZ * mat_lin1[2];
        pontoRefY = pontoRefX * mat_lin2[0] + pontoRefY * mat_lin2[1] + pontoRefZ * mat_lin2[2];
        pontoRefZ = pontoRefX * mat_lin3[0] + pontoRefY * mat_lin3[1] + pontoRefZ * mat_lin3[2];
    }
    else if(key == GLUT_KEY_RIGHT){             // Girar para a esquerda
        GLdouble x = posCameraX, y = posCameraY, z = posCameraZ;
        GLdouble c = cos(-rot_min_up*velocidade), s = sin(-rot_min_up*velocidade);
        GLdouble mat_lin1[] = {x * x * (1 - c) + c, x * y * (1 - c) - z * s, x * z * (1 - c) + y * s};
        GLdouble mat_lin2[] = {y * x * (1 - c) + z * s, y * y * (1 - c) + c, y * z * (1 - c) - x * s};
        GLdouble mat_lin3[] = {x * z * (1 - c) - y * s, y * z * (1 - c) + x * s, z * z * (1 - c) + c};

        pontoRefX = pontoRefX * mat_lin1[0] + pontoRefY * mat_lin1[1] + pontoRefZ * mat_lin1[2];
        pontoRefY = pontoRefX * mat_lin2[0] + pontoRefY * mat_lin2[1] + pontoRefZ * mat_lin2[2];
        pontoRefZ = pontoRefX * mat_lin3[0] + pontoRefY * mat_lin3[1] + pontoRefZ * mat_lin3[2];
    }

    glutPostRedisplay();
}

void display(void)
{
    //limpeza do zbuffer deve ser feita a cada desenho da tela
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt (posCameraX, posCameraY, posCameraZ, pontoRefX, pontoRefY, pontoRefZ, posCameraX, posCameraY, posCameraZ);
    glLightfv(GL_LIGHT0, GL_POSITION, luz_pontual);

    glutIdleFunc(spin);

    desenhar_luz();
    //desenhar_eixos();
    desenhar_planeta();
    desenhar_universe();
    desenhar_moon();

    glPopMatrix();
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    //glViewport (0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, (float)10/8, 0.001, 10);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

GLuint loadTexture(const char* fileName, bool wrap)
{
    Image::Bmp bmp;
    if(!bmp.read(fileName))
        return 0;     // exit if failed load image

    // get bmp info
    int width = bmp.getWidth();
    int height = bmp.getHeight();
    const unsigned char* data = bmp.getDataRGB();
    GLenum type = GL_UNSIGNED_BYTE;    // only allow BMP with 8-bit per channel

    // We assume the image is 8-bit, 24-bit or 32-bit BMP
    GLenum format;
    int bpp = bmp.getBitCount();
    if(bpp == 8)
        format = GL_LUMINANCE;
    else if(bpp == 24)
        format = GL_RGB;
    else if(bpp == 32)
        format = GL_RGBA;
    else
        return 0;               // NOT supported, exit

    // gen texture ID
    GLuint texture;
    glGenTextures(1, &texture);

    // set active texture and configure it
    glBindTexture(GL_TEXTURE_2D, texture);

    // select modulate to mix texture with color for shading
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

    // if wrap is true, the texture wraps over at the edges (repeat)
    //       ... false, the texture ends at the edges (clamp)
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap ? GL_REPEAT : GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap ? GL_REPEAT : GL_CLAMP);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // copy texture data
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
    //glGenerateMipmap(GL_TEXTURE_2D);

    // build our texture mipmaps
    switch(bpp)
    {
        case 8:
            gluBuild2DMipmaps(GL_TEXTURE_2D, 1, width, height, GL_LUMINANCE, type, data);
            break;
        case 24:
            gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, type, data);
            break;
        case 32:
            gluBuild2DMipmaps(GL_TEXTURE_2D, 4, width, height, GL_RGBA, type, data);
            break;
    }

    return texture;
}

int main(int argc, char** argv)
{
    std::cout << "Programa iniciado";
    if(argc < 5){
        std::cout << "Quantidade de parametros abaixo do esperado, o programa demanda 4 valores.";
        return 1;
    }
    arg1 = std::atof(argv[1]);
    luz_pontual[0] = std::atof(argv[2]);
    luz_pontual[1] = std::atof(argv[3]);
    luz_pontual[2] = std::atof(argv[4]);
    ALT_MIN = arg1+0.005;
    ALT_MAX = arg1+0.05;


    Sphere sphere1(arg1, 200, 200, false);    // radius, sectors, stacks, non-smooth (flat) shading
    spherePtr = &sphere1;

    glutInit(&argc, argv);
    //Inicialização dozbuffer
    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH  | GLUT_STENCIL);

    glutInitWindowSize (1000, 800);
    glutInitWindowPosition (100, 100);
    glutCreateWindow (argv[0]);

    init ();

    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutReshapeFunc(reshape);

    texId = loadTexture("media/textures/hi_res/8k_earth.bmp", true);
    texIdSun = loadTexture("media/textures/2k_sun.bmp", true);
    texMoon = loadTexture("media/textures/2k_moon.bmp", true);
    texUniverse = loadTexture("media/textures/hi_res/8k_stars_milky_way.bmp", true);

    glutMainLoop();
    engine->drop();

    return 0;
}
