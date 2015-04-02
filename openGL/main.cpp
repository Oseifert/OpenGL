//
// opengl odds & ends
//

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glut/glut.h>
#include <math.h>


using namespace std;

void menu_func(int value);
void help();

// window width/height
int windowWidth=500;
int windowHeight=500;
double transX =0.0;
double transY = 0.0;
double transZ = 0.0;
double zoom = 0;
float headRot = 0;
float headShake = 0;
float robotX = 0;
float robotY = 0;
float trunk1Rot = 0;
float trunk2Rot = 0;
float trunk3Rot = 0;


bool lamp = false;
bool roboView = false;
bool ambient = true;
bool point = true;
void drawCube();

// camera rotation parameters
float phi=0;
float theta=.5;

enum{
    OPTIONS_AMBIENT,
    OPTIONS_POINT,
    OPTIONS_HELP
};



void menu_func(int value){
    GLfloat white[] = {1,1,1,1};
    GLfloat black[] = {0,0,0,1};
    GLfloat lightAmbient[] = {.2,.2,.2,1};
    GLfloat lightAmbientOff[] = {0,0,0,1};
    switch (value) {
        case OPTIONS_AMBIENT:
            if (ambient) {
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbientOff);
            }
            else{
                glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);
            }
            
            ambient = !ambient;
            break;
        
        case OPTIONS_POINT:

            if (point) {
                glLightfv(GL_LIGHT0, GL_DIFFUSE, black);
                glLightfv(GL_LIGHT0, GL_SPECULAR, black);
            }
            else{
                glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
                glLightfv(GL_LIGHT0, GL_SPECULAR, white);
            }
            point = !point;
            break;
            
        case OPTIONS_HELP:
            help();
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
}

int make_menu(){
    
    int main = glutCreateMenu(menu_func);
    glutAddMenuEntry("Toggle Ambient Ligting", OPTIONS_AMBIENT);
    glutAddMenuEntry("Toggle Point Lighting", OPTIONS_POINT);
    glutAddMenuEntry("Help", OPTIONS_HELP);
    
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    return main;
}

void help()
{

    cerr << "OpenGL\n\
    \n\
    action                result\n\
    ------                ------\n\
    left mouse            rotate view left-right and up-down\n\
    w                     move camera in positive y-direction\n\
    s                     move camera in negative y-direction\n\
    d                     move camera in positive x-direction\n\
    a                     move camera in negative x-direction\n\
    e                     move camera in positive z-direction\n\
    q                     move camera in negative z-direction\n\
    j                     zoom in\n\
    k                     zoom out\n\
    y/u                   nod head\n\
    o/p                   shake head\n\
    i/k                   move robot in +/- x direction\n\
    j/l                   move robot in +/- y direction\n\
    b/B                   move trunk at the shoulder\n\
    n/N                   move trunk at elbow\n\
    m/M                   move trunk at wrist\n\
    v                     toggle on/off the robo-lamp\n\
    c                     switch to 1st/3rd person view\n\
    \n\
    "
        << endl;
}

void init(void)
{
    // set up the view volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,1, 1, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glEnable(GL_LIGHT0);
    if (lamp) {
        glEnable(GL_LIGHT2);
    }
    else{
        glDisable(GL_LIGHT2);
    }
    glEnable(GL_LIGHTING);
    
    GLfloat lightAmbient[] = {.2,.2,.2,1};
    
    GLfloat lightPosition[] = {0,8,0,1};
    GLfloat white[] {1,1,1,0};
    GLfloat yellow[] {1,1,0,0};
    
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);
    //set Point light info

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, .03);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

//    //set spot light info
//    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, .03);
//    glLightfv(GL_LIGHT2, GL_DIFFUSE, yellow);
//    glLightfv(GL_LIGHT2, GL_SPECULAR, yellow);


    // clear colors
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // enabling/disabling opengl features
    glEnable(GL_DEPTH_TEST);
}

void motion(int x, int y) {
    static int lastX=-1;
    static int lastY=-1;
    
    if (lastX==-1 || x==-1) {
        lastX = x;
    }
    else {
        phi -=(float)(x-lastX)/ (float) windowWidth;
        lastX=x;
    }
    
    if (lastY==-1 || y==-1 ) {
        lastY = y;
    }
    else {
        if ((y>lastY && theta<3.14159/2.2) || (y<lastY && theta>-3.14159/2.2))
            theta +=(float)(y-lastY)/ (float) windowHeight;
        lastY=y;
    }
    
    glutPostRedisplay();
}


void mouse(int button, int state, int x, int y) {
    if (button==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
        motion(-1,-1);
    }
}

void arrow_keys ( int a_keys, int x, int y )
{
    
    switch ( a_keys ) {
        case GLUT_KEY_UP:
            glutFullScreen ( );
            break;
        case GLUT_KEY_DOWN: {
            windowWidth=500;
            windowHeight=500;
            glutReshapeWindow ( windowWidth, windowHeight );
            break;
        }
            
        default:
            break;
    }
    glutPostRedisplay();
}


void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case 'w':
            ++transY;
            break;
        case 's':
            --transY;
            break;
        case 'd':
            ++transX;
            break;
        case 'a':
            --transX;
            break;
        case 'e':
            ++transZ;
            break;
        case 'q':
            --transZ;
            break;
        case 'z':
            ++zoom;
            break;
        case 'x':
            --zoom;
            break;
        case 'y':
            --headRot;
            break;
        case 'u':
            ++headRot;
            break;
        case 'p':
            --headShake;
            break;
        case 'o':
            ++headShake;
            break;
        case 'i':
            ++robotY;
            break;
        case 'k':
            --robotY;
            break;
        case 'j':
            ++robotX;
            break;
        case 'l':
            --robotX;
            break;
        case 'b':
            --trunk1Rot;
            break;
        case 'B':
            ++trunk1Rot;
            break;
        case 'n':
            --trunk2Rot;
            break;
        case 'N':
            ++trunk2Rot;
            break;
        case 'm':
            --trunk3Rot;
            break;
        case 'M':
            ++trunk3Rot;
            break;
        case 'v':
            if (lamp) {
                glDisable(GL_LIGHT2);
            }
            else{
                glEnable(GL_LIGHT2);
            }
            
            lamp = !lamp;
            break;
        case 'c':
            roboView = !roboView;
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void reshape(int width, int height)
{
    int size = min(width,height);
    windowWidth=width;
    windowHeight=width;
    glViewport(0,0,size,size);
}

void display( void )
{
    GLenum glErr = glGetError();
    if (glErr != GL_NO_ERROR){
        cout << "glError: " << gluErrorString(glErr) << endl;
    }
    
    
    // clear buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // reset modelview matrix for viewpoint (0,0,5) and save
    glLoadIdentity();
    double x = 15.0*sin(phi);
    double y = 15*sin(theta);
    double z = 15.0*cos(phi);
    double zoomX = zoom * transX+x;
    double zoomY = zoom * transY+y;
    double zoomZ = zoom * transZ+z;
    
    
    double scale =3.0;
    
    if (!roboView) {
        gluLookAt(transX,10+transY,15+transZ,transX+x,transY+y,transZ-z,0,1,0);
    }
//    positions:0,18,3
//    lookat0,18,6
//    up0,21,3
    if (roboView) {
        double theta = headShake*M_PI/180;
        double phi = (90 -(headRot-25))*M_PI/180;
        GLdouble camr = 1*scale;
        GLdouble viewr = 2*scale;
        
        
//        GLdouble camx = (robotX*scale) + camr*(sin(theta)*sin(phi));
//        GLdouble camy = 5*scale - (camr*sin(phi));
//        GLdouble camz = (robotY*scale) + (camr*cos(theta)*cos(phi));
//        GLdouble viewx = (robotX*scale) + viewr*sin(theta)*cos(phi);
//        GLdouble viewy = 5*scale-(viewr*sin(phi));
//        GLdouble viewz = (robotY*scale) + (viewr*cos(theta)*cos(phi));

        GLdouble camx = (robotX*scale) + camr*(sin(phi)*sin(theta));
        GLdouble camy = 5*scale - (camr*cos(phi));
        GLdouble camz = (robotY*scale) + (camr*cos(theta)*sin(phi));
        GLdouble viewx = (robotX*scale) + viewr*sin(theta)*sin(phi);
        GLdouble viewy = 5*scale-(viewr*cos(phi));
        GLdouble viewz = (robotY*scale) + (viewr*cos(theta)*sin(phi));
        
                cerr<<"positions:"<<camx<<','<<camy<<','<<camz<<endl;
                cerr<<"lookat"<<viewx<<','<<viewy<<','<<viewz<<endl;
                //cerr<<"up"<<up_x<<','<<up_y<<','<<up_z<<endl;
        
        gluLookAt(camx, camy, camz,
                  viewx, viewy, viewz,
                  0, 1, 0);
    }
    
    glPushMatrix();
    
    float size=20;
    GLfloat white[] = {1,1,1,0};
    GLfloat black[] = {0,0,0,0};
    GLfloat red[] = {1,0,0,0};              // red
    GLfloat green[] = {0,1,0,0};             // green
    GLfloat purple[] = {1,0,1,0};	    // purple
    
    int numCheckers = 10;
    double checkerWidth = 30;
    double checkerHeight = 30;
    double offsetX = (numCheckers*checkerWidth)/2;
    double offsetY = (numCheckers*checkerHeight)/2;
    
    glNormal3f(0,1,0);
    for(int i = 0; i< numCheckers; i++){
        for(int j = 0; j <numCheckers; j++){
            
            if((i+j)%2 == 0){
                glMaterialfv(GL_FRONT, GL_AMBIENT, black);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
                glMaterialfv(GL_FRONT, GL_SPECULAR, black);
                glMateriali(GL_FRONT,GL_SHININESS,0);
//                glColor3f(0,0,0);
                
            }
            else{
                glMaterialfv(GL_FRONT, GL_AMBIENT, white);
                glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                glMaterialfv(GL_FRONT, GL_SPECULAR, white);
                glMateriali(GL_FRONT,GL_SHININESS,0);
//                glColor3f(1,1,1);
            }
            
            glBegin(GL_QUADS);
            glVertex3f ((i * checkerWidth) - offsetX, 0, (j * checkerHeight) - offsetY);
            glVertex3f ((i * checkerWidth) - offsetX, 0, ((j+1) * checkerHeight) - offsetY);
            glVertex3f (((i+1) * checkerWidth) - offsetX, 0, ((j+1) * checkerHeight) - offsetY);
            glVertex3f (((i+1) * checkerWidth) - offsetX, 0, (j * checkerHeight) - offsetY);
            glEnd();
            
        }
        
    }
    
    glPushMatrix();
    glScalef(scale, scale, scale);
    glTranslatef(robotX, 0, robotY);
    
    //body
    glPushMatrix();
    glTranslatef(0, 3, 0);
    drawCube();
    glPopMatrix();
    
    //left leg
    glPushMatrix();
    glTranslatef(-.5, 1, 0);
    glScalef(.3, 1, .3);
    drawCube();
    glPopMatrix();
    
    // Right leg
    glPushMatrix();
    glTranslatef(.5, 1, 0);
    glScalef(.3, 1, .3);
    drawCube();
    glPopMatrix();
    
    // Head Assembly
    
    glPushMatrix();
    glTranslatef(0, 5, 0);
    glRotatef(headRot, 1, 0, 0);
    glRotatef(headShake, 0, 1, 0);
    
    // draw head
    glMaterialfv(GL_FRONT, GL_AMBIENT, purple);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
    glMaterialfv(GL_FRONT, GL_SPECULAR, purple);
    glMateriali(GL_FRONT,GL_SHININESS,50);
    glPushMatrix();
    glutSolidSphere(1,100,100);
    glPopMatrix();
    
    //draw eyes
    glMaterialfv(GL_FRONT, GL_AMBIENT, black);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
    glMaterialfv(GL_FRONT, GL_SPECULAR, black);
    glMateriali(GL_FRONT,GL_SHININESS,0);
    
    //left eye
    glPushMatrix();
    glTranslatef(-.5, .2, .8);
    glutSolidSphere(.1, 20, 30);
    glPopMatrix();
    
    //right eye
    glPushMatrix();
    glTranslatef(.5, .2, .8);
    glutSolidSphere(.1, 20, 30);
    glPopMatrix();
    
    //Robot viewpoint

    
    
    
    //Create Light
    if (lamp) {
        glPushMatrix();
        GLfloat yellow[] {1,1,0,0};
        //set spot light info
        glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, .1);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, green);
        glLightfv(GL_LIGHT2, GL_SPECULAR, green);
        GLfloat spot_direction[] = {0,0,1};
        GLfloat spot_position[] = {0, 0, 0,1};
        glTranslatef(0, .8, .7);
        glLightfv(GL_LIGHT2, GL_POSITION, spot_position);
        glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction);
        glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
        glColor3f(0, 1, 0);
        glPushAttrib(GL_LIGHTING_BIT);
        glDisable(GL_LIGHTING);
        glutSolidSphere(.1, 20, 20);
        glEnable(GL_LIGHTING);
        glPopAttrib();

        glPopMatrix();
    }
    if (!lamp) {
        glPushMatrix();
        glTranslatef(0, .8, .7);
        glMaterialfv(GL_FRONT, GL_AMBIENT, black);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
        glMaterialfv(GL_FRONT, GL_SPECULAR, black);
        glMateriali(GL_FRONT,GL_SHININESS,0);
        glutSolidSphere(.1, 20, 20);
        glPopMatrix();
    }
    
    
    //draw trunk Shoulder
    glPushMatrix();
    
    //1.5
    glTranslatef(0, 0, 1);
    glRotatef(trunk1Rot, 1, 0, 0);
    glTranslatef(0, 0, .5);
    glScalef(.2, .2, .8);
    drawCube();
    

    //draw trunk forearm
    glPushMatrix();
    
    //1
    glTranslatef(0, 0, 1);
    glRotatef(trunk2Rot, 1, 0, 0);

    glPushMatrix();
    glScalef(.8, .8, 1);
    drawCube();
    glPopMatrix();

    
    
    //draw trunk hand
    glPushMatrix();
    glTranslatef(0, -1.2, 1.1);
    glPushMatrix();
    glScalef(.8, 2.5, .1);
    drawCube();
    glPopMatrix();
    glPopMatrix();
    
    glPopMatrix();
    //end trunk
    glPopMatrix();
    
    
    

    //End Head assembly
    glPopMatrix();
    
    
    
    
    glPopMatrix();
    
    
//    // draw a red triangle
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, red);
//    glMateriali(GL_FRONT,GL_SHININESS,0);
//    
//    
//    drawCube();
//    
////    glColor3f(0, 0, 1);
//    
//    glBegin(GL_TRIANGLES);
//    glVertex3f(-3,1,-8);
//    glVertex3f(3,1,-10);
//    glVertex3f(0,4,-9);
//    glEnd();
//    
//    glNormal3f(0,1,0);
//    
//    
//    // draw a green triangle
//    glMaterialfv(GL_FRONT, GL_AMBIENT, green);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, green);
//    glMateriali(GL_FRONT,GL_SHININESS,0);
//    
////    glColor3f(0.1, 0.3, 0.2);
//    
//    glBegin(GL_TRIANGLES);
//    glVertex3f(-10,1,-3);
//    glVertex3f(-6,1,-4);
//    glVertex3f(-2,4,-2);
//    glEnd();
//    
//    // draw Sphere
//    glMaterialfv(GL_FRONT, GL_AMBIENT, purple);
//    glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
//    glMaterialfv(GL_FRONT, GL_SPECULAR, purple);
//    glMateriali(GL_FRONT,GL_SHININESS,50);
////    glColor3f(0.6, 0.2, 0.8);
    
//    glPushMatrix();
//    glTranslatef(0, 5, 0);
//    glutSolidSphere(2,100,100);
//    glPopMatrix();
    
    glPopMatrix();
    glutSwapBuffers();
}

void drawCube () {
    GLfloat green[] = {0,1,0,0};
    glMaterialfv(GL_FRONT, GL_AMBIENT, green);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, green);
    glMaterialfv(GL_FRONT, GL_SPECULAR, green);
    glMateriali(GL_FRONT,GL_SHININESS,15);
    
    glPushMatrix();
    
    glBegin(GL_QUADS);
    // front face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    // Back Face
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    // Top Face
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    // Bottom Face
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    // Right face
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);
    // Left Face
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();
    glPopMatrix();
    
}



int main ( int argc, char** argv )
{
    
    glutInit(&argc, argv );
    glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL ); // Display Mode
    glutInitWindowPosition(0,0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("OpenGL Project");
    make_menu();
    init ();
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(arrow_keys);
    glutMainLoop( );
    
    return 1;
}