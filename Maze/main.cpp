#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>
#include <math.h>
#include <iostream>
#define LIMITE  25
#define CXZ 0.7,0.7,0.7
#define CR 1.,0.,0.
#define CV 0.,1.,0.
#define CB 0.,0.,1.
#define CJ 1.,1.,0.
#define WIDTH 1000
#define HEIGHT 500
#define PASXZ 1
#define PAS_ANGLE (M_PI/50)
#include <string>
#include <fstream>
#define	RAND_MAX	0x7FFF

using namespace std;
GLfloat pas= 1;
GLint signe = 3;
//GLfloat pas= 10;
//GLint signe = 5;
//transaltion de la scene
GLfloat tx,tz,angle=0.4;

float epsilon = 1e-40;
string lastkey = "";
string lastpgkey = "";
GLdouble camera_pos[3];

float margex = 1;

//la taille du tableau des blocs
int indLs = 0;
int indlb = 0;

//la taille du tableau des objets
int indObj = 0;




int window_menu;


typedef struct {
       float x,y,z;/* Coordonnées cartésiennes du vertex */
} sommet;

typedef struct {
       sommet a, b, c,d;  /* Indices des sommets de la face dans le tableau des sommets */
       GLuint texture;
} face;
typedef struct {
       face f1, f2, f3,f4; /* f1(Av) f2(G) f3(D) f4(Ar) Indices des faces du bloc dans le tableau des faces */
       float couleur[3];
} bloc;


typedef struct {
       sommet position;
       int type;
       float couleur[3];
} object;

sommet Ls[100];
bloc Lb[100];
object obj[100];

sommet v[2];
int iteratorV = 0;

int depth = 6;

string modeDessin = "2D";
int cmpmode = 0;









float ECHELLE_X = 8;
float MARGE_X = 7.8;
float ECHELLE_Y = 9;
float MARGE_Y = 9.8;
//float ECHELLE_Y = 6;
//float MARGE_Y = 6.5;

//char* textureName = "D://textures/wall.bmp";

boolean modetexture = false;
boolean modeobjet = false;
boolean modelumiere = false;
boolean modefog = false;
boolean modemiroir = false;
boolean modeombre = false;
boolean shadowcolor = true;
boolean miroircolor = true;
boolean modefloor = false;
boolean vuehaut = false;
int texture = 0;
int selectedBlock = 0;
//int selectedFace = 1;

char* textureName = "C://textures/1.bmp";


float fogDensity = 0.03;


int floorTexture = 0;
//float thefloor[3] = {0.7,0.7,0.7};

void redim(int w,int h);
GLuint chargerImageBMP(const char * f);

void dessinerAxes(){
     glLineWidth(5);
     glColor3d(1,0,0);
     glBegin(GL_LINES);
        //axe x
        glVertex3f(-LIMITE,.5,-1);
        glVertex3f(LIMITE,.5,-1);

        //axe Y
        glColor3d(0,1,0);
        glVertex3f(.5,-LIMITE,-1);
        glVertex3f(.5,LIMITE,-1);

        //axe z
        glColor3d(0,0,1);
        glVertex3f(.5,.5,-LIMITE);
        glVertex3f(.5,.5,LIMITE);

     glEnd();
}



void dessinerSommet(int i){
    //printf("nbr sommet %d \n" , indLs );
    glPointSize(10);
    glColor3f(1,1,1);
    glBegin(GL_POINTS);
        glVertex3f(Ls[i].x,Ls[i].y,Ls[i].z);
        //printf("sommet %d  %d %d \n" ,Ls[i].x,Ls[i].y,Ls[i].z);
    glEnd();
}

void dessinerQuads3D(int ind){
    //printf("nbr sommet %d \n" , indLs );





//
    if(modemiroir && miroircolor){
        //cout<<"color " << Lb[ind].couleur[0]<<" "<<Lb[ind].couleur[1]<<" "<<Lb[ind].couleur[2]<<endl;
        glColor4d(Lb[ind].couleur[0],Lb[ind].couleur[1],Lb[ind].couleur[2],0.5);
    }else if(modemiroir && !miroircolor){
        glColor3f(Lb[ind].couleur[0],Lb[ind].couleur[1],Lb[ind].couleur[2]);
    }else if(modeombre && shadowcolor){
        //cout<<"color shadow"<<endl;
        glColor4f(0.4,0.4,0.4,1); //color the shadow black
    }else if(modeombre && !shadowcolor){
        glColor3f(Lb[ind].couleur[0],Lb[ind].couleur[1],Lb[ind].couleur[2]);
    }else{
        glColor3f(Lb[ind].couleur[0],Lb[ind].couleur[1],Lb[ind].couleur[2]);
    }




    glPointSize(30);
    glBegin(GL_QUADS);
        //faceavant

        glVertex3f(Lb[ind].f1.a.x,Lb[ind].f1.a.y,Lb[ind].f1.a.z);
        glVertex3f(Lb[ind].f1.b.x,Lb[ind].f1.b.y,Lb[ind].f1.b.z);
        glVertex3f(Lb[ind].f1.c.x,Lb[ind].f1.c.y,Lb[ind].f1.c.z);
        glVertex3f(Lb[ind].f1.d.x,Lb[ind].f1.d.y,Lb[ind].f1.d.z);



        //faceguache
        glVertex3f(Lb[ind].f2.a.x,Lb[ind].f2.a.y,Lb[ind].f2.a.z);
        glVertex3f(Lb[ind].f2.b.x,Lb[ind].f2.b.y,Lb[ind].f2.b.z);
        glVertex3f(Lb[ind].f2.c.x,Lb[ind].f2.c.y,Lb[ind].f2.c.z);
        glVertex3f(Lb[ind].f2.d.x,Lb[ind].f2.d.y,Lb[ind].f2.d.z);

//        //facedroit
        glVertex3f(Lb[ind].f3.a.x,Lb[ind].f3.a.y,Lb[ind].f3.a.z);
        glVertex3f(Lb[ind].f3.b.x,Lb[ind].f3.b.y,Lb[ind].f3.b.z);
        glVertex3f(Lb[ind].f3.c.x,Lb[ind].f3.c.y,Lb[ind].f3.c.z);
        glVertex3f(Lb[ind].f3.d.x,Lb[ind].f3.d.y,Lb[ind].f3.d.z);
//
//        //facearriere
        glVertex3f(Lb[ind].f4.a.x,Lb[ind].f4.a.y,Lb[ind].f4.a.z);
        glVertex3f(Lb[ind].f4.b.x,Lb[ind].f4.b.y,Lb[ind].f4.b.z);
        glVertex3f(Lb[ind].f4.c.x,Lb[ind].f4.c.y,Lb[ind].f4.c.z);
        glVertex3f(Lb[ind].f4.d.x,Lb[ind].f4.d.y,Lb[ind].f4.d.z);
    glEnd();
}


void dessinerQuads2D(int ind){
    //printf("nbr sommet %d \n" , indLs );
    glPointSize(30);
    glBegin(GL_QUADS);
        //faceavant
        glColor3f(0,1,1);
        glVertex3f(Lb[ind].f1.a.x,Lb[ind].f1.a.y,Lb[ind].f1.a.z);
        glVertex3f(Lb[ind].f1.b.x,Lb[ind].f1.b.y,Lb[ind].f1.b.z);
        glVertex3f(Lb[ind].f1.c.x,Lb[ind].f1.c.y,Lb[ind].f1.c.z);
        glVertex3f(Lb[ind].f1.d.x,Lb[ind].f1.d.y,Lb[ind].f1.d.z);

    glEnd();
}

void appliquerTexture(int ind){


   // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glClear(GL_DEPTH_BUFFER_BIT);

    if(Lb[ind].f1.texture != 0){
       // glMatrixMode(GL_MODELVIEW);
       // glLoadIdentity();
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D,Lb[ind].f1.texture);
        glBegin(GL_QUADS);
        // Face Avant
            glTexCoord2f(0.0f, 0.0f); glVertex3f(Lb[ind].f1.a.x,Lb[ind].f1.a.y,Lb[ind].f1.a.z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(Lb[ind].f1.b.x,Lb[ind].f1.b.y,Lb[ind].f1.b.z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(Lb[ind].f1.c.x,Lb[ind].f1.c.y,Lb[ind].f1.c.z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(Lb[ind].f1.d.x,Lb[ind].f1.d.y,Lb[ind].f1.d.z);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }



    if(Lb[ind].f2.texture != 0){
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D,Lb[ind].f2.texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(Lb[ind].f2.a.x,Lb[ind].f2.a.y,Lb[ind].f2.a.z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(Lb[ind].f2.b.x,Lb[ind].f2.b.y,Lb[ind].f2.b.z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(Lb[ind].f2.c.x,Lb[ind].f2.c.y,Lb[ind].f2.c.z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(Lb[ind].f2.d.x,Lb[ind].f2.d.y,Lb[ind].f2.d.z);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }


    if(Lb[ind].f3.texture != 0){
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D,Lb[ind].f3.texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(Lb[ind].f3.a.x,Lb[ind].f3.a.y,Lb[ind].f3.a.z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(Lb[ind].f3.b.x,Lb[ind].f3.b.y,Lb[ind].f3.b.z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(Lb[ind].f3.c.x,Lb[ind].f3.c.y,Lb[ind].f3.c.z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(Lb[ind].f3.d.x,Lb[ind].f3.d.y,Lb[ind].f3.d.z);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }


    if(Lb[ind].f4.texture != 0){

        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D,Lb[ind].f4.texture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(Lb[ind].f4.a.x,Lb[ind].f4.a.y,Lb[ind].f4.a.z);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(Lb[ind].f4.b.x,Lb[ind].f4.b.y,Lb[ind].f4.b.z);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(Lb[ind].f4.c.x,Lb[ind].f4.c.y,Lb[ind].f4.c.z);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(Lb[ind].f4.d.x,Lb[ind].f4.d.y,Lb[ind].f4.d.z);
        glEnd();
        glDisable(GL_TEXTURE_2D);
    }


}



void dessiner3dCube(float x,float y){

    float marge = 0.5;
    y = 1;
    float z = 0.2;


     glPointSize(10);
     glBegin(GL_QUADS);
      // avant
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x,y,z);
      glVertex3f(x+marge, y, z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge, 0, z);
      glVertex3f(x,0, z);

      // arriere

      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x,0,-z);
      glVertex3f(x+marge,0,-z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge,y,-z);
      glVertex3f(x,y,-z);

//
//      // gauche
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x,y,-z);
      glVertex3f(x,y,z);
      glVertex3f(x,0,z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x,0,-z);

//      //  droit
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge,y,-z);
      glVertex3f(x+marge,y,z);
      glVertex3f(x+marge,0,z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge,0,-z);
   glEnd();

}


void dessiner3dTriangle(float x,float y){
    float marge = 0.3;
    y = 1;
    float z = 0.2;
    glBegin(GL_TRIANGLE_FAN);
      // avant
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x,y, 0.0f);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x-marge, 0,z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge, 0, z);

      // droit
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x,y, 0.0f);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge, 0, z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge, 0, -z);

      // arriere
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x, y, 0.0f);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x+marge, 0, -z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x-marge, 0, -z);

      // gauche
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f( x, y, 0.0f);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x-marge,0,-z);
      glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      glVertex3f(x-marge,0, z);
   glEnd();

}




void dessiner3dCylinder(float x,float y){
   // cout<<"object at position " << x << " "<< y << endl;

    GLfloat angle          = 0.0;
    GLfloat angle_stepsize = 0.1;
    y = 0;
    GLfloat radius = 0.2;
    GLfloat height = 0.5;

    /** dessin du tube */
    glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
    glBegin(GL_QUAD_STRIP);
    angle = 0.0;
        while( angle < 2*M_PI ) {
            x = x + radius * cos(angle)/6;
            y = y + radius * sin(angle)/6;
            glVertex3f(x, y , height);
            glVertex3f(x, y , -height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius, y, height);
       // glVertex3f(radius, y, -height);
    glEnd();

    /** dessin du circle */
    glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
    glBegin(GL_POLYGON);
    angle = 0.0;
        while( angle < 2*M_PI ) {
            x = x + radius * cos(angle)/6;
            y = y + radius * sin(angle)/6;
            glVertex3f(x, y, height);
            angle = angle + angle_stepsize;
        }
        glVertex3f(radius,y, height);
    glEnd();
}




void dessinerObjet(int ind){

    glColor3f(obj[ind].couleur[0],obj[ind].couleur[1],obj[ind].couleur[2]);
    float x = obj[ind].position.x;
    float y = obj[ind].position.y;


//    glPointSize(30);
//    glBegin(GL_POINTS);
//        glVertex2f(x,0);
//    glEnd();



   // cout<<"object at position " << x << " "<< y << endl;

    switch(obj[ind].type){

        case 1 :
                 //glTranslatef(x,1,0);
                 //glutSolidSphere(0.3,10,10);
                 dessiner3dTriangle(x,y);
                 break;
        case 2 :
                 //glTranslatef(x,1,0);
                 //glutSolidTeapot(0.3);
                 dessiner3dCube(x,y);
                 break;

        case 3 :
                 //glTranslatef(x,1,0);
                 //glutSolidTorus(0.1,0.2,20,20);
                 dessiner3dCylinder(x,y);
                 break;
    }


}



void dessinerScene(void){
    int i;


   if(vuehaut){
        glRotatef(60, 1.0f, 0.0f, 0.0f);
   }else{
        glRotatef(0, 1.0f, 0.0f, 0.0f);
   }


    // Tracer le plan xz
   // if(modeDessin == "3D"){

   if(modefloor){
        glColor3f(CXZ);
        //glPointSize(40);
        glEnable(GL_TEXTURE_2D);
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_REPLACE);
        glBindTexture(GL_TEXTURE_2D,floorTexture);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-LIMITE,	0.,	-LIMITE	);;
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-LIMITE,	0.,	LIMITE	);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(LIMITE,	0.,	LIMITE	);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(LIMITE,	0.,	-LIMITE	);
        glEnd();
          glDisable(GL_TEXTURE_2D);
   }else{
        glBegin(GL_POLYGON);
            glColor3f(CXZ);
            glVertex3f(-LIMITE,	0.,	-LIMITE	);
            glVertex3f(-LIMITE,	0.,	LIMITE	);
            glVertex3f(LIMITE,	0.,	LIMITE	);
            glVertex3f(LIMITE,	0.,	-LIMITE	);
        glEnd();
   }







    for(i=0;i<indLs;i++){
        //glColor3f(Lb[i].couleur[0],Lb[i].couleur[1],Lb[i].couleur[2]);
        dessinerSommet(i);
    }


   // cout<<"mode de dessin " << modeDessin << endl;
    for(i=0;i<indlb;i++){

        //glColor3f(Lb[i].couleur[0],Lb[i].couleur[1],Lb[i].couleur[2]);
        if(modeDessin == "3D"){
            dessinerQuads3D(i);
        }else{
            dessinerQuads2D(i);
        }

        if(modetexture){
            appliquerTexture(i);
        }else{
            glDisable(GL_TEXTURE_2D);
        }


    }


   // cout<<"mode objet"<<modeobjet<<endl;

   // (modeobjet)? cout<<" yes"<<endl:cout<<" no"<<endl;
    if(modeobjet){
            for(int i =0; i< indObj;i++){
                dessinerObjet(i);
            }
    }

//     cout<<"mode lumiere";
//     (modelumiere)? cout<<" yes"<<endl:cout<<" no"<<endl;

    if(modelumiere){

          //cout<<"tx ="<<tx<<endl;
          //cout<<"tz ="<<tz<<endl;

          glEnable(GL_COLOR_MATERIAL);
          glEnable(GL_LIGHTING); //activer la lumiere
          glEnable(GL_LIGHT0); //activer la lumiere No. 0
          glEnable(GL_NORMALIZE); //Automatically "normalize" normals
          glShadeModel(GL_SMOOTH); //activier l'Ombrage doux (bon effet) modèle de Gouraud

          glClear(GL_DEPTH_BUFFER_BIT);
          //GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f};
          GLfloat lightColor0[] = {1.5f,1.5f,1.5f, 1.0f}; //l'intensité du couleur
          GLfloat lightPos0[] = {-tx,0,tz, 1.0f}; //position
          glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0); //définir la couleur
          glLightfv(GL_LIGHT0, GL_POSITION, lightPos0); //définir la position
          glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 200);
    }else{
          glDisable(GL_COLOR_MATERIAL);
          glDisable(GL_LIGHTING); //activer la lumiere
          glDisable(GL_LIGHT0); //activer la lumiere No. 0
          glDisable(GL_NORMALIZE);
    }


    if(modefog){

        GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0}; //la couleur gris

        glEnable (GL_FOG); //activier le brouillard

        glFogi (GL_FOG_MODE, GL_EXP2); //brouillard mode GL_EXP2

        glFogfv (GL_FOG_COLOR, fogColor); //définier la couleur du brouillard

        glFogf (GL_FOG_DENSITY, fogDensity); //définer la densité

        glHint (GL_FOG_HINT, GL_NICEST); // définir le mode plus agréable

    }else{
        glDisable(GL_FOG);
    }


   // (modemiroir)? cout<<" yes"<<endl:cout<<" no"<<endl;
    if(modemiroir){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glShadeModel (GL_SMOOTH);//modèle de Gouraud
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            glClearStencil(0); //nettoyer le stencil buffer
            glClearDepth(1.0f);
            glClear (GL_STENCIL_BUFFER_BIT);
            //glLoadIdentity();
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //desactiver le masque de couleur
            glDepthMask(GL_FALSE); //desactiver le masque de profondeur

            glEnable(GL_STENCIL_TEST); //activier les tests du sentcil

            glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
            glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); //Définissez le tampon stencil pour remplacer notre prochain lot de données

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Activer le masque de couleur
            glDepthMask(GL_TRUE); //activer le masque de profendeur

            glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Définissez le tampon stencil pour garder notre prochain lot de données


            glDisable(GL_DEPTH_TEST); //Désactiver le test de profondeur de l'ombre
            glPushMatrix();
            glScalef(1.0f, -1.0f, 1.0f); //Basculer l'ombre verticalement

            glTranslatef(0,2,0); //(translation) l'ombre sur notre plan de dessin

            //dessin de la réflexion
            miroircolor = true;
            for(i=0;i<indlb;i++){
                if(modeDessin == "3D"){
                    dessinerQuads3D(i);
                }
            }

            glPopMatrix();

            glEnable(GL_DEPTH_TEST); //activier le testing de la profendeur
            glDisable(GL_STENCIL_TEST); //desactiver le sentcil testing


            glEnable(GL_BLEND); //activer alpha blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //definissez alpha blending


            miroircolor = false;
//            for(i=0;i<indlb;i++){
//                if(modeDessin == "3D"){
//                    dessinerQuads3D(i);
//                }
//            }

        }



    if(modeombre){
            glEnable(GL_DEPTH_TEST);
            glDepthFunc(GL_LEQUAL);
            glShadeModel (GL_SMOOTH);
            glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
            glClearStencil(0); //nettoyer le stencil buffer
            glClearDepth(1.0f);
            glClear (GL_STENCIL_BUFFER_BIT);
            //glLoadIdentity();
            glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); //desactiver le masque de couleur
            glDepthMask(GL_FALSE); //desactiver le masque de profondeur

            glEnable(GL_STENCIL_TEST); //activier les tests du sentcil

            glStencilFunc(GL_ALWAYS, 1, 0xFFFFFFFF);
            glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE); //Définissez le tampon stencil pour remplacer notre prochain lot de données

            glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE); //Activer le masque de couleur
            glDepthMask(GL_TRUE); //activer le masque de profendeur

            glStencilFunc(GL_EQUAL, 1, 0xFFFFFFFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP); //Définissez le tampon stencil pour garder notre prochain lot de données


            glDisable(GL_DEPTH_TEST); //Désactiver le test de profondeur de l'ombre
            glPushMatrix();
            glScalef(1.0f, -1.0f, 1.0f); //Basculer l'ombre verticalement

            glTranslatef(0,2,0); //(translation) l'ombre sur notre plan de dessin

            //dessin de l'ombre
            shadowcolor = true;
            for(i=0;i<indlb;i++){
                if(modeDessin == "3D"){
                    dessinerQuads3D(i);
                }
            }

            glPopMatrix();

            glEnable(GL_DEPTH_TEST); //activier le testing de la profendeur
            glDisable(GL_STENCIL_TEST); //desactiver le sentcil testing

            glEnable(GL_BLEND); //activer alpha blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); //definissez alpha blending


            shadowcolor = false;
//            for(i=0;i<indlb;i++){
//                if(modeDessin == "3D"){
//                    dessinerQuads3D(i);
//                }
//            }

        }


    glutPostRedisplay();
}





void affiche(void){

    // Effacer l'écran avec la couleur du fond courant
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3f(1.0, 1.0, 1.0);

    int w=glutGet(GLUT_WINDOW_WIDTH);
	int h=glutGet(GLUT_WINDOW_HEIGHT);
    redim(w,h);
    glLoadIdentity();


    //gluLookAt(0.0,0.0,1.0,0.0,0.0,-1.0,0.0,1.0,0.0);




    glTranslatef(tx,0,tz);
    glRotatef(angle,0,1,0);


    dessinerScene();

//    glColor3f(1,0,1);
//    glPointSize(10);
//    glBegin(GL_POINTS);
////        glVertex2d(0,9.4);
////        glVertex2d(0,0);
////        glVertex2f(8,0);
////        glVertex2f(-7.8,0);
////        glVertex3d(0,0,6.5);
////        glVertex3d(0,0,0);
////        glVertex3f(8,0,0);
////        glVertex3f(-7.8,0,0);
//    glEnd();





    // Afficher le buffer (ce qui est dessiné)( cas de Single buffering).
    glutSwapBuffers ();
}
//pour afficher du texte
void afficherTexte(int x,int y,char *string,void *font) {
      int len,i;
      glRasterPos2f(x,y);
      len = (int) strlen(string);
      for (i = 0; i < len; i++) {
        glutBitmapCharacter(font,string[i]);
      }
}

void menu(){
      glColor3f(0.5,0.5,1);
      afficherTexte(130,20,"** MENU **",GLUT_BITMAP_TIMES_ROMAN_24);
      glColor3f(0,0,0);
     // glColor3f(rand()/(double)RAND_MAX,rand()/(double)RAND_MAX,rand()/(double)RAND_MAX);
      afficherTexte(10,65,"T : pour transformer le dessin 2D en 3D",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,95,"V : pour vider l'ecran",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,125,"S : pour sauvegarde le labyrinth",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,155,"C : pour charger le labyrinth",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,185,"E : pour activer/desactiver les textures",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,215,"O : pour activer/desactiver l'ajout des objets",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,245,"Z : pour supprimer le dernier objet",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,275,"H : pour changer les textures",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,305,"L : pour activer/desactiver la lumiere/ombre ",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,335,"F : pour activer/desactiver l'effet brouillard",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,365,"d/D : pour augmenter/diminuer la densite du brouillard ",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,395,"M : pour activer/desactiver l'effet miroir ",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,425,"W : pour activer/desactiver l'effet ombre ",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,455,"R : pour activer/desactiver la texture du sol",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,485,"U : pour activer/desactiver la vue en haut",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,515,"B : pour supprimer le dernier bloc",GLUT_BITMAP_HELVETICA_18);
      afficherTexte(10,545,"Q : pour quittez",GLUT_BITMAP_HELVETICA_18);

}

void affiche2(void) {
      glClear(GL_COLOR_BUFFER_BIT);
      menu();
      glutSwapBuffers();
}



void viderTabBlocs(){

    indLs = 0;
    for(int i = 0;i<indlb;i++){

        Lb[i].f1.a.x = 0;
        Lb[i].f1.b.x = 0;
        Lb[i].f1.c.x = 0;
        Lb[i].f1.d.x = 0;
        Lb[i].f1.a.y = 0;
        Lb[i].f1.b.y = 0;
        Lb[i].f1.c.y = 0;
        Lb[i].f1.d.y = 0;
        Lb[i].f1.a.z = 0;
        Lb[i].f1.b.z = 0;
        Lb[i].f1.c.z = 0;
        Lb[i].f1.d.z = 0;


        Lb[i].f2.a.x = 0;
        Lb[i].f2.b.x = 0;
        Lb[i].f2.c.x = 0;
        Lb[i].f2.d.x = 0;
        Lb[i].f2.a.y = 0;
        Lb[i].f2.b.y = 0;
        Lb[i].f2.c.y = 0;
        Lb[i].f2.d.y = 0;
        Lb[i].f2.a.z = 0;
        Lb[i].f2.b.z = 0;
        Lb[i].f2.c.z = 0;
        Lb[i].f2.d.z = 0;




        Lb[i].f3.a.x = 0;
        Lb[i].f3.b.x = 0;
        Lb[i].f3.c.x = 0;
        Lb[i].f3.d.x = 0;
        Lb[i].f3.a.y = 0;
        Lb[i].f3.b.y = 0;
        Lb[i].f3.c.y = 0;
        Lb[i].f3.d.y = 0;
        Lb[i].f3.a.z = 0;
        Lb[i].f3.b.z = 0;
        Lb[i].f3.c.z = 0;
        Lb[i].f3.d.z = 0;



        Lb[i].f4.a.x = 0;
        Lb[i].f4.b.x = 0;
        Lb[i].f4.c.x = 0;
        Lb[i].f4.d.x = 0;
        Lb[i].f4.a.y = 0;
        Lb[i].f4.b.y = 0;
        Lb[i].f4.c.y = 0;
        Lb[i].f4.d.y = 0;
        Lb[i].f4.a.z = 0;
        Lb[i].f4.b.z = 0;
        Lb[i].f4.c.z = 0;
        Lb[i].f4.d.z = 0;

        glDeleteTextures( 1, &Lb[i].f1.texture );
        glDeleteTextures( 1, &Lb[i].f2.texture );
        glDeleteTextures( 1, &Lb[i].f3.texture );
        glDeleteTextures( 1, &Lb[i].f4.texture );

        Lb[i].f1.texture = 0;
        Lb[i].f2.texture = 0;
        Lb[i].f3.texture = 0;
        Lb[i].f4.texture = 0;

    }

    indlb = 0;
    indObj = 0;
}


void supprimerDernierObjet(){
    indObj--;
    if(indObj <0) indObj = 0;
}

void supprimerDernierBloc(){
    indlb--;
    if(indlb <0) indlb = 0;
}

void suavegarder(){


          ofstream MAZEFICHIER;

          MAZEFICHIER.open("./maze.txt");

          if(MAZEFICHIER.is_open())
          {
                  MAZEFICHIER<<indlb<<endl;
                  for(int i=0;i<indlb;i++)
                  {
                    MAZEFICHIER<<Lb[i].f1.a.x<<" "<<Lb[i].f1.a.y<<" "<<Lb[i].f1.a.z<<endl;
                    MAZEFICHIER<<Lb[i].f1.b.x<<" "<<Lb[i].f1.b.y<<" "<<Lb[i].f1.b.z<<endl;
                    MAZEFICHIER<<Lb[i].f1.c.x<<" "<<Lb[i].f1.c.y<<" "<<Lb[i].f1.c.z<<endl;
                    MAZEFICHIER<<Lb[i].f1.d.x<<" "<<Lb[i].f1.d.y<<" "<<Lb[i].f1.d.z<<endl;

                    MAZEFICHIER<<Lb[i].f2.a.x<<" "<<Lb[i].f2.a.y<<" "<<Lb[i].f2.a.z<<endl;
                    MAZEFICHIER<<Lb[i].f2.b.x<<" "<<Lb[i].f2.b.y<<" "<<Lb[i].f2.b.z<<endl;
                    MAZEFICHIER<<Lb[i].f2.c.x<<" "<<Lb[i].f2.c.y<<" "<<Lb[i].f2.c.z<<endl;
                    MAZEFICHIER<<Lb[i].f2.d.x<<" "<<Lb[i].f2.d.y<<" "<<Lb[i].f2.d.z<<endl;


                    MAZEFICHIER<<Lb[i].f3.a.x<<" "<<Lb[i].f3.a.y<<" "<<Lb[i].f3.a.z<<endl;
                    MAZEFICHIER<<Lb[i].f3.b.x<<" "<<Lb[i].f3.b.y<<" "<<Lb[i].f3.b.z<<endl;
                    MAZEFICHIER<<Lb[i].f3.c.x<<" "<<Lb[i].f3.c.y<<" "<<Lb[i].f3.c.z<<endl;
                    MAZEFICHIER<<Lb[i].f3.d.x<<" "<<Lb[i].f3.d.y<<" "<<Lb[i].f3.d.z<<endl;


                    MAZEFICHIER<<Lb[i].f4.a.x<<" "<<Lb[i].f4.a.y<<" "<<Lb[i].f4.a.z<<endl;
                    MAZEFICHIER<<Lb[i].f4.b.x<<" "<<Lb[i].f4.b.y<<" "<<Lb[i].f4.b.z<<endl;
                    MAZEFICHIER<<Lb[i].f4.c.x<<" "<<Lb[i].f4.c.y<<" "<<Lb[i].f4.c.z<<endl;
                    MAZEFICHIER<<Lb[i].f4.d.x<<" "<<Lb[i].f4.d.y<<" "<<Lb[i].f4.d.z<<endl;

                    MAZEFICHIER<<Lb[i].couleur[0]<<" "<<Lb[i].couleur[1]<<" "<<Lb[i].couleur[2]<<endl;
                  }


                  MAZEFICHIER.close();
                  cout<<"maze a ete suavegardé avec succes"<<endl;
            }else{
                 cout<<"erreur n'est pas sauvegardé"<<endl;
            }


}








void charger(){

    ifstream MAZEFICHIER("./maze.txt");
	if(MAZEFICHIER)  //On teste si tout est OK
	{
          MAZEFICHIER>>indlb;
          for(int i=0;i<indlb;i++)
          {
                MAZEFICHIER>>Lb[i].f1.a.x;
                MAZEFICHIER>>Lb[i].f1.a.y;
                MAZEFICHIER>>Lb[i].f1.a.z;
                MAZEFICHIER>>Lb[i].f1.b.x;
                MAZEFICHIER>>Lb[i].f1.b.y;
                MAZEFICHIER>>Lb[i].f1.b.z;
                MAZEFICHIER>>Lb[i].f1.c.x;
                MAZEFICHIER>>Lb[i].f1.c.y;
                MAZEFICHIER>>Lb[i].f1.c.z;
                MAZEFICHIER>>Lb[i].f1.d.x;
                MAZEFICHIER>>Lb[i].f1.d.y;
                MAZEFICHIER>>Lb[i].f1.d.z;

                MAZEFICHIER>>Lb[i].f2.a.x;
                MAZEFICHIER>>Lb[i].f2.a.y;
                MAZEFICHIER>>Lb[i].f2.a.z;
                MAZEFICHIER>>Lb[i].f2.b.x;
                MAZEFICHIER>>Lb[i].f2.b.y;
                MAZEFICHIER>>Lb[i].f2.b.z;
                MAZEFICHIER>>Lb[i].f2.c.x;
                MAZEFICHIER>>Lb[i].f2.c.y;
                MAZEFICHIER>>Lb[i].f2.c.z;
                MAZEFICHIER>>Lb[i].f2.d.x;
                MAZEFICHIER>>Lb[i].f2.d.y;
                MAZEFICHIER>>Lb[i].f2.d.z;


                MAZEFICHIER>>Lb[i].f3.a.x;
                MAZEFICHIER>>Lb[i].f3.a.y;
                MAZEFICHIER>>Lb[i].f3.a.z;
                MAZEFICHIER>>Lb[i].f3.b.x;
                MAZEFICHIER>>Lb[i].f3.b.y;
                MAZEFICHIER>>Lb[i].f3.b.z;
                MAZEFICHIER>>Lb[i].f3.c.x;
                MAZEFICHIER>>Lb[i].f3.c.y;
                MAZEFICHIER>>Lb[i].f3.c.z;
                MAZEFICHIER>>Lb[i].f3.d.x;
                MAZEFICHIER>>Lb[i].f3.d.y;
                MAZEFICHIER>>Lb[i].f3.d.z;



                MAZEFICHIER>>Lb[i].f4.a.x;
                MAZEFICHIER>>Lb[i].f4.a.y;
                MAZEFICHIER>>Lb[i].f4.a.z;
                MAZEFICHIER>>Lb[i].f4.b.x;
                MAZEFICHIER>>Lb[i].f4.b.y;
                MAZEFICHIER>>Lb[i].f4.b.z;
                MAZEFICHIER>>Lb[i].f4.c.x;
                MAZEFICHIER>>Lb[i].f4.c.y;
                MAZEFICHIER>>Lb[i].f4.c.z;
                MAZEFICHIER>>Lb[i].f4.d.x;
                MAZEFICHIER>>Lb[i].f4.d.y;
                MAZEFICHIER>>Lb[i].f4.d.z;

                MAZEFICHIER>>Lb[i].couleur[0];
                MAZEFICHIER>>Lb[i].couleur[1];
                MAZEFICHIER>>Lb[i].couleur[2];



          }



    }
	else
    {
		cout << "ERREUR : Impossible d'ouvrir le fichier. lire" << endl;
	}

}







void redim (int w, int h){



//    ECHELLE_W = w/(2*LIMITE);
//    ECHELLE_H = h/(2*LIMITE);


    glViewport(0,0,w,h);

   // calculVolume();

//    // Fixer le volume à observer
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-1, 1., -1., 1.,1.,LIMITE);
    gluLookAt(0.,1.5,8.,0.,1.5,0.,0.,1.,0);
    // glFrustum(-1, 1., -1., 1.,1.,LIMITE);
   // gluLookAt(0.,1.5,8.,0.,1.5,0.,0.,1.,.0);

   // gluPerspective(50, ((GLfloat) w) / ((GLfloat) h),1.0,LIMITE);
    //gluLookAt(-tx,tx,-tz,tz,0.,0.,0.,0.,-1.);


	// gluLookAt(0,tx,LIMITE,-LIMITE,tz,-tz,0.,1.,.0);
	// gluLookAt(0,tx,PC[2],PC[0]+U[0],PC[1]+U[1],PC[2]+U[2],0.,1.,0.);
	//gluPerspective(45.0, ((GLfloat) w) / ((GLfloat) h),1.0,2*LIMITE);
	//glOrtho(0.0f, w, h, 0.0f, - LIMITE, LIMITE);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void redim2(int w,int h) {
      glViewport(0,0,w,h);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(0,w,0,h);
      glScalef(1,-1,1);
      glTranslatef(0,-h,0);
      glMatrixMode(GL_MODELVIEW);
}





int collision2(float x,float z){



     //printf("camera Position x = %d y = %d z = %d \n",x,y,z);
    for(int ind=0;ind<indlb;ind++){


       // printf("camera pos x = %f z = %f \n",x,z);
//        printf("face avant point a  x = %f z = %f \n",Lb[ind].f1.a.x,Lb[ind].f1.a.z);
//        printf("face avant point c  x = %f z = %f \n",Lb[ind].f1.c.x,Lb[ind].f1.c.z);



      // printf("camera Position x = %d z = %d \n",tx,tz);
        if(x >= Lb[ind].f1.a.x - margex && x <= Lb[ind].f1.c.x + margex){
            if(abs(z - Lb[ind].f1.c.z) < epsilon){
                return 1;
            }
        }


//        printf("face gauche point a  x = %f z = %f \n",Lb[ind].f2.a.x,Lb[ind].f2.a.z);
//        printf("face gauche point c  x = %f z = %f \n",Lb[ind].f2.c.x,Lb[ind].f2.c.z);

        if(z >= Lb[ind].f2.a.z && z <= Lb[ind].f2.c.z){
            if(abs(x - Lb[ind].f2.c.x) < epsilon){

                return 1;
            }
        }


//        printf("face droit point a  x = %f z = %f \n",Lb[ind].f3.a.x,Lb[ind].f3.a.z);
//        printf("face droit point c  x = %f z = %f \n",Lb[ind].f3.c.x,Lb[ind].f3.c.z);
        if(z >= Lb[ind].f3.a.z && z <= Lb[ind].f3.c.z){
            if(abs(x  - Lb[ind].f3.c.x) < epsilon){

                return 1;
            }
        }



//        printf("face arriere point a  x = %f z = %f \n",Lb[ind].f4.a.x,Lb[ind].f4.a.z);
//        printf("face arriere point c  x = %f z = %f \n",Lb[ind].f4.c.x,Lb[ind].f4.c.z);
        if(x >= Lb[ind].f4.a.x - margex && x <= Lb[ind].f4.c.x + margex){
            if(abs(z - Lb[ind].f4.c.z) < epsilon){

                return 1;
            }
        }






    }


    return 0;


}


void specialClavier(int key, int x, int y){

    //position de camera

    int viewport[4];
    GLdouble matModelView[16];
    GLdouble matProjection[16];
    // get matrixs and viewport:
    glGetDoublev( GL_MODELVIEW_MATRIX, matModelView );
    glGetDoublev( GL_PROJECTION_MATRIX, matProjection );
    glGetIntegerv( GL_VIEWPORT, viewport );
    gluUnProject( (viewport[2]-viewport[0])/2 , (viewport[3]-viewport[1])/2,
    0.0, matModelView, matProjection, viewport,
    &camera_pos[0],&camera_pos[1],&camera_pos[2]);

   // printf("camera Position x = %f y = %f z = %f \n",camera_pos[0],camera_pos[1],camera_pos[2]);

   // int collis = collision(camera_pos[0],camera_pos[1],camera_pos[2]);
    //float xx = (float)-1*tx;

    int collis = collision2(camera_pos[0],camera_pos[2]);


    printf("collis = %d \n",collis);


    cout<<lastkey<<endl;



   // printf("angle rotation = %f \n",angle);

   // cout<< "translation x = "<< tx<<endl;

  //  cout<< "translation z = "<< tz<<endl;


//    if(angle > 62 && angle < 142){
//        colisf3 = 1;
//        colisf1 = 2;
//        colisf4 = 3;
//        colisf2 = 4;
//    }else if(angle < -62){
//        colisf2 = 1;
//        colisf1 = 3;
//        colisf4 = 2;
//        colisf3 = 4;
//
//    }else{
//        colisf1 = 1;
//        colisf2 = 2;
//        colisf3 = 3;
//        colisf4 = 4;
//    }

       // float goback = 1;

        if(collis == 1 && lastkey == "left"){
              tx+=signe*-1*pas*.1;
        }else if(collis == 1 && lastkey == "up"){
              tz+=signe*-1*pas*.1 ;
        }else if(collis == 1 && lastkey == "right"){
              tx+=signe*pas*.1 ;
        }else if(collis == 1 && lastkey == "down"){
              tz+= signe*pas*.1 ;
        }





        switch (key) {

//            case GLUT_KEY_LEFT :    if(collis != colisf3){
//                                         tx+=signe*pas*.1 ;
//                                    }
//
//                                    break;
//
//            case GLUT_KEY_UP :
//                                    if(collis != colisf1 && collis != colisf4){
//                                        tz+= signe*pas*.1 ;
//                                    }
//                                    break;
//            case GLUT_KEY_RIGHT :   if(collis != colisf2){
//                                        tx+=signe*-1*pas*.1 ;
//                                    }
//                                    break;
//            case GLUT_KEY_DOWN :    if(collis != colisf4){
//                                        tz+=signe*-1*pas*.1 ;
//                                    }
//                                    break;
//

            case GLUT_KEY_LEFT :    if(collis == 0){
                                         tx+=signe*pas*.1 ;
                                         lastkey = "left";
                                    }
                                    break;

            case GLUT_KEY_UP :      if(collis == 0){
                                         tz+= signe*pas*.1 ;
                                         lastkey = "up";

                                    }
                                    break;
            case GLUT_KEY_RIGHT :   if(collis == 0){
                                        tx+=signe*-1*pas*.1;
                                        lastkey = "right";
                                    }
                                    break;

            case GLUT_KEY_DOWN :    if(collis == 0){
                                        tz+=signe*-1*pas*.1 ;
                                        lastkey = "down";
                                    }

                                    break;



            //rotation
            case GLUT_KEY_PAGE_UP :   if(collis == 0){
                                        angle++;
                                       // lastpgkey="page_up";

                                      }else{
                                        angle--;
                                      }
                                      break;


            case GLUT_KEY_PAGE_DOWN:  if(collis == 0){
                                        angle--;
                                       // lastpgkey="page_down";

                                      }else{
                                        angle++;
                                      }

                                        break;

       }





   // calculVolume();
    glutPostRedisplay();



}

//int id = 0;
void ajouterObjet(float x,float y){

   obj[indObj].position.x = x;
   obj[indObj].position.y = y;
   obj[indObj].position.z = 0;


   int id  = rand()%3 + 1;

   //id = 3;
   obj[indObj].type = id;
   obj[indObj].couleur[0] = rand()/(double)RAND_MAX;
   obj[indObj].couleur[1] = rand()/(double)RAND_MAX;
   obj[indObj].couleur[2] = rand()/(double)RAND_MAX;

   indObj++;
   glutPostRedisplay();
}


GLuint chargerImageBMP(const char *fileName){
        FILE *file;
        unsigned char header[54];
        unsigned int dataPos;
        unsigned int size;
        unsigned int width, height;
        unsigned char *data;


        file = fopen(fileName, "rb");

        if (file == NULL)
        {
            cout<<fileName<<" Erreur: chemin du fichier est invalid !" <<endl;
            return false;
        }

        if (fread(header, 1, 54, file) != 54)
        {
            cout<<"Erreur: fichier invalid!"<<endl;
            return false;
        }

        if (header[0] != 'B' || header[1] != 'M')
        {
            cout<<"Erreur: fichier invalid!"<<endl;
            return false;
        }

        dataPos     = *(int*)&(header[0x0A]);
        size        = *(int*)&(header[0x22]);
        width       = *(int*)&(header[0x12]);
        height      = *(int*)&(header[0x16]);

        if (size == NULL)
            size = width * height * 3;
        if (dataPos == NULL)
            dataPos = 54;

        data = new unsigned char[size];

        fread(data, 1, size, file);

        fclose(file);
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//appliquer l'algorime le plus proche de l'interpolation
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        cout<<fileName<<" bien chargée "<<endl;

    return texture;
}



void ajouterTextureAutreFace(char key){
    switch(key){
        case '1' : Lb[selectedBlock].f1.texture = chargerImageBMP(textureName);break;
        case '2' : Lb[selectedBlock].f2.texture = chargerImageBMP(textureName);break;
        case '3' : Lb[selectedBlock].f3.texture = chargerImageBMP(textureName);break;
        case '4' : Lb[selectedBlock].f4.texture = chargerImageBMP(textureName);break;

    }
    glutPostRedisplay();
}




void changerTexture(int ind,int f){

    selectedBlock = ind;

    if(modetexture){
         if(f == 1){
            Lb[ind].f1.texture = chargerImageBMP(textureName);
        }
        else if(f== 2){
            Lb[ind].f2.texture = chargerImageBMP(textureName);
        }else if(f==3){
            Lb[ind].f3.texture = chargerImageBMP(textureName);
        }else if(f==4){
            Lb[ind].f4.texture = chargerImageBMP(textureName);
        }
    }

    glutPostRedisplay();

}

void ajouterTexture(float x,float y){

    cout<<"right mouse clicked " << x << " "<< y << endl;

    if(y < 0)
    {
        y*= -1;
    }

     //printf("camera Position x = %d y = %d z = %d \n",x,y,z);
    for(int ind=0;ind<indlb;ind++){


     //   printf("camera pos x = %f z = %f \n",x,z);


      //  printf("face avant point a  x = %f y = %f \n",Lb[ind].f1.a.x,Lb[ind].f1.a.y);
      //  printf("face avant point c  x = %f y = %f \n",Lb[ind].f1.c.x,Lb[ind].f1.c.y);



      // printf("camera Position x = %d z = %d \n",tx,tz);
      //    printf("face avant point a  x = %f y = %f \n",Lb[ind].f2.a.x,Lb[ind].f2.a.y);
       //   printf("face avant point c  x = %f y = %f \n",Lb[ind].f2.c.x,Lb[ind].f2.c.y);
        if(x >= Lb[ind].f1.a.x && x <= Lb[ind].f1.c.x){
            if(y >= Lb[ind].f1.c.y && y <= Lb[ind].f1.a.y ){
                cout<<"face 1 "<<endl;
                changerTexture(ind,1);
                return;
            }
        }


      //  printf("face avant point a  x = %f y = %f \n",Lb[ind].f2.a.x,Lb[ind].f2.a.y);
      //  printf("face avant point c  x = %f y = %f \n",Lb[ind].f2.c.x,Lb[ind].f2.c.y);
        if(abs(x  - Lb[ind].f2.c.x) < epsilon){
            if(y >= Lb[ind].f2.c.y && y <= Lb[ind].f2.a.y ){
                cout<<"face 2 "<<endl;
                changerTexture(ind,2);
                return;
            }
        }
//
//
//        printf("face droit point a  x = %f y = %f \n",Lb[ind].f3.a.x,Lb[ind].f3.a.y);
//        printf("face droit point c  x = %f y = %f \n",Lb[ind].f3.c.x,Lb[ind].f3.c.y);
        if(y >= Lb[ind].f3.c.y && y <= Lb[ind].f3.a.y){
            if(abs(x  - Lb[ind].f3.c.x) < epsilon){
                cout<<"face 3 "<<endl;
                changerTexture(ind,3);
                return;
            }
        }
//
//
//
//        printf("face arriere point a  x = %f y = %f \n",Lb[ind].f4.a.x,Lb[ind].f4.a.y);
//        printf("face arriere point c  x = %f y = %f \n",Lb[ind].f4.c.x,Lb[ind].f4.c.y);
         if(x >= Lb[ind].f4.a.x && x <= Lb[ind].f4.c.x){
            if(y >= Lb[ind].f4.c.y  && y <= Lb[ind].f4.a.y ){

                cout<<"face 4 "<<endl;
                changerTexture(ind,4);
                return;
            }
        }






    }


}



void switchTextures(){
     srand(time(0));
     int indice  = rand()%12 + 1;
     char* ss = new char[100];
     sprintf(ss,"C://textures/%d.bmp",indice);
   //  cout<<" here !!"<<ss<<endl;

     for(int i=0;i<indlb;i++){
        if(Lb[i].f1.texture != 0){
            glDeleteTextures( 1, &Lb[i].f1.texture );
            Lb[i].f1.texture = chargerImageBMP(ss);
        }
        if(Lb[i].f2.texture != 0){
            glDeleteTextures( 1, &Lb[i].f2.texture );
            Lb[i].f2.texture = chargerImageBMP(ss);
        }
        if(Lb[i].f3.texture != 0){
            glDeleteTextures( 1, &Lb[i].f3.texture );
            Lb[i].f3.texture = chargerImageBMP(ss);
        }
        if(Lb[i].f4.texture != 0){
            glDeleteTextures( 1, &Lb[i].f4.texture );
            Lb[i].f4.texture = chargerImageBMP(ss);
        }
     }
     textureName = ss;
    // cout<<" here !!"<<textureName<<endl;
}


void clavier(unsigned char key, int x, int y){
        switch(key){

            case 't' :
                      if(cmpmode % 2 == 1){
                            modetexture = false;
                            modeDessin = "2D";
                      }else if(cmpmode %2 == 0){
                            modeDessin = "3D";

                      }
                      cmpmode++;
                      break;

             case 'v' :
                      viderTabBlocs();
                      break;

             case 's' :
                      suavegarder();
                      break;

             case 'c' :
                      charger();
                      break;

             case 'q' :
                      exit(0);
                      break;

             case 'e' :
                       modetexture = !modetexture;
                       break;
             case 'h' :
                      switchTextures();
                      break;

             case 'l' :
                        modelumiere = !modelumiere;
                        break;

             case 'o' : modeobjet = !modeobjet;
                        break;
             case 'z' : if(modeobjet) supprimerDernierObjet();
                        break;

             case 'f' : modefog = !modefog;
                        break;

            case 'm'  : modemiroir = !modemiroir;
                        modeombre = false;
                        break;

            case 'w'  : modeombre = !modeombre;
                        modemiroir = false;
                        break;
            case 'r' : modefloor = !modefloor;


             case 'd': if(modefog){
                            fogDensity *= 1.10;
                            glFogf(GL_FOG_DENSITY, fogDensity);
                        }
                        break;
             case 'D':  if(modefog){
                            fogDensity /= 1.10;
                            glFogf(GL_FOG_DENSITY, fogDensity);
                        }
                        break;

            case 'u' : vuehaut = !vuehaut;
                       break;

            case 'b' : supprimerDernierBloc();
                       break;


            case '1' : ajouterTextureAutreFace('1');break;

            case '2' : ajouterTextureAutreFace('2');break;

            case '3' : ajouterTextureAutreFace('3');break;

            case '4' : ajouterTextureAutreFace('4');break;


        }

        glutPostRedisplay();


}

void mouseMotion(int button,int state,int x,int y){



//    float xx= (((x- (WIDTH/2)) * 5)/220)+0.2;
//  float yy= -1*(((y - (HEIGHT/2)) * 5 )/125 - 0.6);

//


//
//    float xx = (x/(double)ECHELLE_W)-LIMITE ;
//    float yy = (y/(double)ECHELLE_H)-LIMITE ;
//
//    yy *= -1;

//    float xx = (((x - (WIDTH/2)) * 10)/(WIDTH/2));
//    float yy = -1*(((y - (HEIGHT/2)) * 10)/(HEIGHT/2));

//    float xx = (float)(x/500);
//    float yy = (float)(y/294);

    int w=glutGet(GLUT_WINDOW_WIDTH);
    int h=glutGet(GLUT_WINDOW_HEIGHT);

    float xx= ((float)(x * ECHELLE_X)/(w/2)) - MARGE_X ;
    float yy= -1*(((float)(y * ECHELLE_Y)/(h/2))- MARGE_Y);
    float cofx= ((30 + tz)/30);
    float cofy= ((30 + tz)/30);

    xx /= cofx;
    yy /= cofy;


  //cout<<"before translation "<< xx << " " << yy << endl;

    if(tx>0 || tx < 0)
    {
        xx -= tx;
    }

  //cout<<"after translation "<< xx << " " << yy << endl;



//
//    float xx = (((x- (WIDTH/2)) * 9)/(WIDTH/2));
//    float yy = -1*(((y - (HEIGHT/2)) * 9)/(HEIGHT/2));

      if ( state == GLUT_DOWN )
      {
        switch ( button )
        {
          case GLUT_LEFT_BUTTON  :

                                   Ls[indLs].x = xx;
                                   Ls[indLs].z = (float) rand()/RAND_MAX * (0 - (-4))+0;
                                   Ls[indLs].y = yy;

                                   printf("Sommet ajoute %.f %.f %.f \n" , Ls[indLs].x,Ls[indLs].y,Ls[indLs].z);

                                   indLs++;


                                  // printf("Sommet (%d) V ajoute  \n" , iteratorV);

                                  if(iteratorV < 2 ){
                                        v[iteratorV].x = xx;
                                        v[iteratorV].y = yy;
                                        v[iteratorV].z = Ls[indLs-1].z;

                                        printf("Sommet (%d) V ajoute %.2f %.2f %.2f \n" , iteratorV, v[iteratorV].x, v[iteratorV].y, v[iteratorV].z);
                                        iteratorV++;
                                        if(iteratorV == 2){
                                               //depth = rand()/RAND_MAX * (6 - 1) + 1;
                                               srand((time(0)));
                                               depth = rand()%4+1;
                                               //depth++;
                                               cout<<"depth == " <<depth<<endl;
                                                v[1].z = v[0].z;
                                                v[1].y = 0;


                                               iteratorV = 0;
                                               indLs = 0;
                                               //printf(" nbr quads %d \n",indlb);
                                               Lb[indlb].couleur[0] = rand()/(double)RAND_MAX;
                                               Lb[indlb].couleur[1] = rand()/(double)RAND_MAX;
                                               Lb[indlb].couleur[2] = rand()/(double)RAND_MAX;
                                               //faceavant
                                               Lb[indlb].f1.a.x = v[0].x;
                                               Lb[indlb].f1.b.x = v[1].x;
                                               Lb[indlb].f1.c.x = v[1].x;
                                               Lb[indlb].f1.d.x = v[0].x;


                                               Lb[indlb].f1.a.y = v[0].y;
                                               Lb[indlb].f1.b.y = v[0].y;
                                               Lb[indlb].f1.c.y = v[1].y;
                                               Lb[indlb].f1.d.y = v[1].y;

                                               Lb[indlb].f1.a.z = v[0].z;
                                               Lb[indlb].f1.b.z = v[1].z;
                                               Lb[indlb].f1.c.z = v[1].z;
                                               Lb[indlb].f1.d.z = v[0].z;


                                               //face guache
                                               Lb[indlb].f2.a.x = v[0].x;
                                               Lb[indlb].f2.b.x = v[0].x;
                                               Lb[indlb].f2.c.x = v[0].x;
                                               Lb[indlb].f2.d.x = v[0].x;


                                               Lb[indlb].f2.a.y = v[0].y;
                                               Lb[indlb].f2.b.y = v[0].y;
                                               Lb[indlb].f2.c.y = v[1].y;
                                               Lb[indlb].f2.d.y = v[1].y;

                                               Lb[indlb].f2.a.z = -depth;
                                               Lb[indlb].f2.b.z = v[0].z;
                                               Lb[indlb].f2.c.z = v[0].z;
                                               Lb[indlb].f2.d.z = -depth;


                                               //facedroit
                                               Lb[indlb].f3.a.x = v[1].x;
                                               Lb[indlb].f3.b.x = v[1].x;
                                               Lb[indlb].f3.c.x = v[1].x;
                                               Lb[indlb].f3.d.x = v[1].x;


                                               Lb[indlb].f3.a.y = v[0].y;
                                               Lb[indlb].f3.b.y = v[0].y;
                                               Lb[indlb].f3.c.y = v[1].y;
                                               Lb[indlb].f3.d.y = v[1].y;

                                               Lb[indlb].f3.a.z = -depth;
                                               Lb[indlb].f3.b.z = v[1].z;
                                               Lb[indlb].f3.c.z = v[1].z;
                                               Lb[indlb].f3.d.z = -depth;


                                               //facearriere
                                               Lb[indlb].f4.a.x = v[0].x;
                                               Lb[indlb].f4.b.x = v[1].x;
                                               Lb[indlb].f4.c.x = v[1].x;
                                               Lb[indlb].f4.d.x = v[0].x;


                                               Lb[indlb].f4.a.y = v[0].y;
                                               Lb[indlb].f4.b.y = v[0].y;
                                               Lb[indlb].f4.c.y = v[1].y;
                                               Lb[indlb].f4.d.y = v[1].y;

                                               Lb[indlb].f4.a.z = -depth;
                                               Lb[indlb].f4.b.z = -depth;
                                               Lb[indlb].f4.c.z = -depth;
                                               Lb[indlb].f4.d.z = -depth;

                                               indlb++;
                                        }
                                   }


                                   break ;
                case GLUT_RIGHT_BUTTON :
                                       // cout<<"right mouse clicked"<<endl;
                                        if(modetexture) ajouterTexture(xx,yy);
                                        if(modeobjet) ajouterObjet(xx,yy);
                                        break;

        }

      }

     // calculVolume();
      glutPostRedisplay();



}




void loadFloorTexture(){
    floorTexture = chargerImageBMP("C://textures/floor.bmp");
    cout<<"floorTexture "<<floorTexture<<endl;
}



int initx = 0;
int fois = 0;
void passivemouse(int x,int y){
    cout<<"passive motion init x = " << x <<endl;
    cout<<"passive motion " << x << y <<endl;
    if(fois == 0){
        initx = x;
    }
    fois++;
    if(x>initx){
        angle--;
    }else {

        angle++;
    }

    glutPostRedisplay();
}



int main(int argc, char** argv){

        glutInit(&argc, argv);

        // Single buffering et couleurs en RGB
        glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB |GLUT_DEPTH);
        // Fixer la taille et la position  de la fenêtre
        glutInitWindowSize (WIDTH,HEIGHT);
        glutInitWindowPosition (200, 100);

        // Créer une fenêtre avec comme titre celui passé en paramètre
        glutCreateWindow ("Maze ");

        // Choix de la couleur du fond
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glEnable(GL_DEPTH_TEST);

        // Les Callback
        glutDisplayFunc(affiche);
        glutKeyboardFunc(clavier);
        glutSpecialFunc(specialClavier);
        glutMouseFunc(mouseMotion);
        loadFloorTexture();
        //glutPassiveMotionFunc(passivemouse);



        glutReshapeFunc(redim);

        //la fenetre qui contient le menu:
        glutInitWindowSize(450,560);
        glutInitWindowPosition (10, 0);
        window_menu = glutCreateWindow("Menu");
        glClearColor(1.0,1.0,1.0,1.0);
        glutDisplayFunc(affiche2);
        glutReshapeFunc(redim2);

        // Entrer dans la boucle principale et traiter les événements.
        // On conserver l'image produite à l'écran.
        // Le programme est mis en attente d'événements
        glutMainLoop();

        return 0;
}


