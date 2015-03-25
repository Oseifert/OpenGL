//
// opengl odds & ends
//

#include <iostream>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <glut/glut.h>
#include <math.h>


using namespace std;

// window width/height
int windowWidth=500;
int windowHeight=500;

// camera rotation parameters
float phi=0;
float theta=.5;



void init(void)
{
    // set up the view volume
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(80,1, 1, 200);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
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
    gluLookAt(x,y,z,0,0,0,0,1,0);
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
    double offsetX = windowWidth/2;
    double offsetY = windowHeight/2;
    
    
    for(int i = 0; i< numCheckers; i++){
        for(int j = 0; j <numCheckers; j++){
            
            if((i+j)%2 == 0){
                //                glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
                //                glMaterialfv(GL_FRONT, GL_SPECULAR, black);
                //                glMateriali(GL_FRONT,GL_SHININESS,0);
                glColor3f(0,0,0);
                
            }
            else{
                //                glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
                //                glMaterialfv(GL_FRONT, GL_SPECULAR, white);
                //                glMateriali(GL_FRONT,GL_SHININESS,0);
                glColor3f(1,1,1);
            }
            
            glBegin(GL_QUADS);
            glVertex3f ((i * checkerWidth) - offsetX, 0, (j * checkerHeight) - offsetY);
            glVertex3f ((i * checkerWidth) - offsetX, 0, ((j+1) * checkerHeight) - offsetY);
            glVertex3f (((i+1) * checkerWidth) - offsetX, 0, ((j+1) * checkerHeight) - offsetY);
            glVertex3f (((i+1) * checkerWidth) - offsetX, 0, (j * checkerHeight) - offsetY);
            glEnd();
            
        }
        
    }
    
    
    
    // draw a red triangle
    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    //    glMaterialfv(GL_FRONT, GL_SPECULAR, red);
    //    glMateriali(GL_FRONT,GL_SHININESS,0);
    
    glColor3f(0, 0, 1);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(-3,1,-8);
    glVertex3f(3,1,-10);
    glVertex3f(0,4,-9);
    glEnd();
    
    glNormal3f(0,1,0);
    
    
    // draw a red triangle
    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, red);
    //    glMaterialfv(GL_FRONT, GL_SPECULAR, red);
    //    glMateriali(GL_FRONT,GL_SHININESS,0);
    
    glColor3f(0.1, 0.3, 0.2);
    
    glBegin(GL_TRIANGLES);
    glVertex3f(-10,1,-3);
    glVertex3f(-6,1,-4);
    glVertex3f(-2,4,-2);
    glEnd();
    
    // draw Sphere
    //    glMaterialfv(GL_FRONT, GL_DIFFUSE, purple);
    //    glMaterialfv(GL_FRONT, GL_SPECULAR, purple);
    //    glMateriali(GL_FRONT,GL_SHININESS,50);
    glColor3f(0.6, 0.2, 0.8);
    
    glutSolidSphere(2,100,100);
    
    
    glPopMatrix();
    glutSwapBuffers();
}


int main ( int argc, char** argv )
{
    
    glutInit(&argc, argv );
    glutInitDisplayMode ( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_STENCIL ); // Display Mode
    glutInitWindowPosition(0,0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Odds & ends tutorial");
    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutSpecialFunc(arrow_keys);
    glutMainLoop( );
    
    return 1;
}