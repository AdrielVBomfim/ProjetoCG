/*
    Projeto "Eu, avião"

    Integrantes:    Adriel Venancio Bomfim
                    João Vitor Oliveira Cordeiro
                    Lucas Almeida Francisco


    Argumentos: 0.5 (raio da terra) 2.0 (posição x do sol) 0.3 (posicao y do sol) 0.7 (posicao z do sol)
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

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
Sphere universe(5, 60, 60, false);

//Variáveis de posição da câmera
float posCameraX, posCameraY, posCameraZ;
float pontoRefX, pontoRefY, pontoRefZ;
float teta, fi;
float raioCamera;
float anguloMoon = 0.0;
float velocidade = 0.001;
float velocidadeLua = 0.5;
bool decolar, pousar, turn_left, turn_right;

//Argumento correspondente ao raio do planeta
GLdouble arg1;
//Argumentos correspondentes à posição x, y e z do Sol
GLfloat luz_pontual[] = {1.0, 1.0, 1.0, 1.0};

float angulo = 2*M_PI/360;

float ALT_MIN;
float ALT_MAX;
float VEL_CRUZEIRO = 0.005;
float VEL_MAX = 0.008;
float COEF_GIRO = 3;

void iluminar(){
    //LU1.0
    // no mínimo 8 fontes podem ser utilizadas
    //(iniciadas com cor preta)
    // número de fontes de luz afeta performance

    //LUZ 0

    //define características para a fonte de luz 0
    //fonte de luz direcional (por que w==0?)
    GLfloat light0_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat light0_ambient[] = { 0.8, 0.8, 0.8, 1.0 };
    //atribui características para a fonte de luz 0
    //cor padrão: branco
    glLightfv(GL_LIGHT0, GL_POSITION, luz_pontual);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);

    //"acende" cada uma das luzes configuradas
    glEnable(GL_LIGHT0);
}

//desenha esfera na posição da fonte de luz
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


//desenha eixos xyz
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

//desenha objeto
void desenhar_planeta(){
    //MATERIAL
    //define características para aparência do material
    //exercício: testar exemplos da seção
    //Changing Material Properties, do Red Book
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

//desenha esfera com textura de estrelas + via láctea
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
//desenha a lua
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

void spin(void){

    if(decolar){
     if(raioCamera < ALT_MAX) {
         raioCamera += 0.001;
         velocidade += 0.0001;
         if(velocidade > VEL_CRUZEIRO) velocidade = VEL_CRUZEIRO;
     }
     else {
         decolar = false;
         raioCamera = ALT_MAX;
     }
    }if(pousar){
        if(raioCamera > ALT_MIN) {
            raioCamera -= 0.001;
            velocidade -= 0.0001;
            if(velocidade < 0) velocidade = 0;
        }
        else {
            raioCamera = ALT_MIN;
            pousar = false;
            velocidade = 0;
        }
    }

    float versorDesl[] = {pontoRefX-posCameraX, pontoRefY-posCameraY, pontoRefZ-posCameraZ};
    float vetorDirecao[] = {posCameraY*versorDesl[2]-posCameraZ*versorDesl[1],
                            posCameraZ*versorDesl[0]-posCameraX*versorDesl[2],
                            posCameraX*versorDesl[1]-posCameraY*versorDesl[0]};

    float giro = COEF_GIRO*(1-(velocidade/VEL_MAX)*0.6);
    if(turn_left){
        versorDesl[0] += vetorDirecao[0]*giro;
        versorDesl[1] += vetorDirecao[1]*giro;
        versorDesl[2] += vetorDirecao[2]*giro;
        turn_left = false;
    }if(turn_right){
        versorDesl[0] -= vetorDirecao[0]*giro;
        versorDesl[1] -= vetorDirecao[1]*giro;
        versorDesl[2] -= vetorDirecao[2]*giro;
        turn_right = false;
    }


    float fator = sqrt(versorDesl[0]*versorDesl[0]+versorDesl[1]*versorDesl[1]+versorDesl[2]*versorDesl[2]);
    versorDesl[0] = versorDesl[0]/fator;
    versorDesl[1] = versorDesl[1]/fator;
    versorDesl[2] = versorDesl[2]/fator;

    posCameraX += versorDesl[0]*velocidade;
    posCameraY += versorDesl[1]*velocidade;
    posCameraZ += versorDesl[2]*velocidade;

    pontoRefX += versorDesl[0]*velocidade;
    pontoRefY += versorDesl[1]*velocidade;
    pontoRefZ += versorDesl[2]*velocidade;

    if(posCameraX != 0) teta = atan(posCameraY/posCameraX);
    else teta = 0;
    float numerador = sqrt(pow(posCameraX,2)+pow(posCameraY,2));
    if(posCameraZ != 0) fi = atan(numerador/posCameraZ);
    else fi = 0;

    if(posCameraX > 0 && posCameraZ < 0) fi+=M_PI;
    if(posCameraX < 0 && posCameraZ < 0) fi+=M_PI;
    if(posCameraX < 0 && posCameraZ > 0) fi+=2*M_PI;

    if(posCameraX < 0 && posCameraY > 0) teta+=M_PI;
    if(posCameraX < 0 && posCameraY < 0) teta+=M_PI;
    if(posCameraX > 0 && posCameraY < 0) teta+=2*M_PI;

    //std::cout << "Versor: "<< versorDesl[0]<<" "<<versorDesl[1]<<" "<<versorDesl[2] << "\n";
//    std::cout << "-------------------\n";
//    std::cout << "Pos XYZ transl: " << posCameraX << " " << posCameraY << " " << posCameraZ<< "\n";
//    std::cout << "Center XYZ transl: " << pontoRefX << " " << pontoRefY << " " << pontoRefZ<< "\n";
//    std::cout << "Teta: " << teta << " Phi: " << fi<< "\n";

    posCameraX = raioCamera * sin(fi) * cos(teta);
    posCameraY = raioCamera * sin(fi) * sin(teta);
    posCameraZ = raioCamera * cos(fi);

    if(pontoRefX != 0) teta = atan(pontoRefY/pontoRefX);
    else teta = 0;
    numerador = sqrt(pow(pontoRefX,2)+pow(pontoRefY,2));
    if(pontoRefZ != 0) fi = atan(numerador/pontoRefZ);
    else fi = 0;

    if(pontoRefX > 0 && pontoRefZ < 0) fi+=M_PI;
    if(pontoRefX < 0 && pontoRefZ < 0) fi+=M_PI;
    if(pontoRefX < 0 && pontoRefZ > 0) fi+=2*M_PI;

    if(pontoRefX < 0 && pontoRefY > 0) teta+=M_PI;
    if(pontoRefX < 0 && pontoRefY < 0) teta+=M_PI;
    if(pontoRefX > 0 && pontoRefY < 0) teta+=2*M_PI;

//    std::cout << "Center Teta: " << teta << " Phi: " << fi<< "\n";

    pontoRefX = raioCamera * sin(fi) * cos(teta);
    pontoRefY = raioCamera * sin(fi) * sin(teta);
    pontoRefZ = raioCamera * cos(fi);

    anguloMoon += velocidadeLua;
    if(anguloMoon >= 360)
        anguloMoon = 0;
    glutPostRedisplay();
}

void init(void)
{
    engine->play2D("starscape.wav", GL_TRUE);
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

    teta = 0.0;
    fi = M_PI/2;

    pontoRefX = raioCamera;//raioCamera * cos(teta) * sin(fi + angulo);
    pontoRefY = 0.1;
    pontoRefZ = 0.1;// * cos(fi + angulo);

}

void specialKeys(int key, int x, int y)
{
    if(key == GLUT_KEY_UP){                 //Acelerar
        if(raioCamera > ALT_MIN) velocidade += 0.001;
        if(velocidade > VEL_MAX) velocidade = VEL_MAX;
    }
    else if(key == GLUT_KEY_DOWN){         //Desacelerar
        velocidade -= 0.001;
        if (velocidade < 0) velocidade = 0;
        if (velocidade == 0) pousar = true;
    }
    else if(key == GLUT_KEY_PAGE_UP){           //Decolar
        decolar = true;
    }
    else if(key == GLUT_KEY_PAGE_DOWN){         //Aterrissar
        pousar = true;
    }
    else if(key == GLUT_KEY_LEFT){          // Girar para a esquerda
        turn_left = true;
    }
    else if(key == GLUT_KEY_RIGHT){             // Girar para a esquerda
        turn_right = true;
    }

    glutPostRedisplay();
}

// Por que a luz fica parada durante a renderização, já que quem se
// move na cena é a câmera?
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
    //glOrtho(-3.0 * arg1, 3.0 * arg1, -3.0 * arg1, 3.0 * arg1, -3.0 * arg1, 3.0 * arg1);
    gluPerspective(70.0, 10/8, 0.001, 10);

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

    texId = loadTexture("8k_earth.bmp", true);
    texIdSun = loadTexture("2k_sun.bmp", true);
    texMoon = loadTexture("2k_moon.bmp", true);
    texUniverse = loadTexture("2k_stars_milky_way.bmp", true);

    glutMainLoop();
    engine->drop();

    return 0;
}