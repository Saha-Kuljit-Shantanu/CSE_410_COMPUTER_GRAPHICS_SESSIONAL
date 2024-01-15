#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>

#include <GL/glut.h>

#define pi (2*acos(0.0))

using namespace std;


//float sectorCount = 18;
//float stackCount = 18;
//float radius = 0.4;

//float sectorStep = 2* pi/sectorCount;
//float stackStep = 2* pi/stackCount;
//float sectorAngle, stackAngle;

double tx = 0, ty = 0, tz = -15, rx = 0, ry = 0, rz = 0, cx = 0, cy = 0, dx = 0, dy = 30;

//gluLookAt(0,0,1,0,0,0,1,2,0);
double eyex = 0, eyey = 0, eyez = 1, lookx = 0, looky = 0, lookz = -1,upx = 0.6, upy =0.8, upz = 0;
double rightx = 0.8, righty = -0.6, rightz = 0;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
double toRad = pi/180;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		glColor3f(1.0, 1.0, 1.0);
		glBegin(GL_LINES);{
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
}

void drawArrow() {

    
    glColor3f(0,0,1); 
    glLineWidth(10);
    glBegin(GL_LINES);
    glVertex3f(cx, cy, 0);
    glVertex3f(cx + 2* cos(dy* toRad), cy + 2* sin(dy* toRad), 0);
    glEnd();
   

}

void drawGrid()
{
	int i;
	if(drawgrid==1)
	{
		glColor3f(0.6, 0.6, 0.6);	//grey
		glBegin(GL_LINES);{
			for(i=-8;i<=8;i++){

				if(i==0)
					continue;	//SKIP the MAIN axes

				//lines parallel to Y-axis
				glVertex3f(i*10, -90, 0);
				glVertex3f(i*10,  90, 0);

				//lines parallel to X-axis
				glVertex3f(-90, i*10, 0);
				glVertex3f( 90, i*10, 0);
			}
		}glEnd();
	}
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
	glBegin(GL_QUADS);{
		glVertex3f( a, a,2);
		glVertex3f( a,-a,2);
		glVertex3f(-a,-a,2);
		glVertex3f(-a, a,2);
	}glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0;i<segments;i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
	struct point points[100][100];
	int i,j;
	double h,r;
	//generate points
	for(i=0;i<=stacks;i++)
	{
		h=radius*sin(((double)i/(double)stacks)*(pi/2));
		r=radius*cos(((double)i/(double)stacks)*(pi/2));
		for(j=0;j<=slices;j++)
		{
			points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
			points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
			points[i][j].z=h;
		}
	}
	//draw quads using generated points
	for(i=0;i<stacks;i++)
	{
        //glColor3f((double)i/(double)stacks,(double)i/(double)stacks,(double)i/(double)stacks);
        
		for(j=0;j<slices;j++)
		{
		
		
			glBegin(GL_QUADS);{
			    //upper hemisphere
			    	glColor3f(1-j%2,j%2,0);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                		//lower hemisphere
                		glColor3f(j%2,1-j%2,0);
                		glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


void drawSS()
{
    glColor3f(1,0,0);
    drawSquare(20);

    glRotatef(angle,0,0,1);
    glTranslatef(110,0,0);
    glRotatef(2*angle,0,0,1);
    glColor3f(0,1,0);
    drawSquare(15);

    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(60,0,0);
        glRotatef(2*angle,0,0,1);
        glColor3f(0,0,1);
        drawSquare(10);
    }
    glPopMatrix();

    glRotatef(3*angle,0,0,1);
    glTranslatef(40,0,0);
    glRotatef(4*angle,0,0,1);
    glColor3f(1,1,0);
    drawSquare(5);
}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){

		case '1':
			//drawgrid=1-drawgrid;
			
			rightx = rightx* cos(0.1) + lookx* sin(0.1);
			righty = righty* cos(0.1) + looky* sin(0.1);
			rightz = rightz* cos(0.1) + lookz* sin(0.1);
			
			lookx = lookx* cos(0.1) - rightx* sin(0.1);
			looky = looky* cos(0.1) - righty* sin(0.1);
			lookz = lookz* cos(0.1) - rightz* sin(0.1);
			//cx = cx - 0.2;
			break;
			
		case '2':
			//drawgrid=1-drawgrid;
			
			//cy = cy - 0.1;
			//cx = cx + 0.2;
			
			rightx = rightx* cos(0.1) - lookx* sin(0.1);
			righty = righty* cos(0.1) - looky* sin(0.1);
			rightz = rightz* cos(0.1) - lookz* sin(0.1);
			
			lookx = lookx* cos(0.1) + rightx* sin(0.1);
			looky = looky* cos(0.1) + righty* sin(0.1);
			lookz = lookz* cos(0.1) + rightz* sin(0.1);
			
			break;
			
		case '3':
			//drawgrid=1-drawgrid;
			
			lookx = lookx* cos(0.1) + upx* sin(0.1);
			looky = looky* cos(0.1) + upy* sin(0.1);
			lookz = lookz* cos(0.1) + upz* sin(0.1);
			
			upx = upx* cos(0.1) - lookx* sin(0.1);
			upy = upy* cos(0.1) - looky* sin(0.1);
			upz = upz* cos(0.1) - lookz* sin(0.1);
			
			//e = e -1;
			//cy = cy+0.2;
			//cx = cx+0.1;

			break;
			
		case '4':
			//drawgrid=1-drawgrid;
			
			lookx = lookx* cos(0.1) - upx* sin(0.1);
			looky = looky* cos(0.1) - upy* sin(0.1);
			lookz = lookz* cos(0.1) - upz* sin(0.1);
			
			upx = upx* cos(0.1) + lookx* sin(0.1);
			upy = upy* cos(0.1) + looky* sin(0.1);
			upz = upz* cos(0.1) + lookz* sin(0.1);
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
		case '5':
			//drawgrid=1-drawgrid;
			
			//d = d -1;
			/*if(upy > -4 && upy < 4)*/ 
			//if(upy <= -4 && upy >=-4.1) {
			//	upx = -upx;
			//	upy = -7;
			//}
			
			upx = upx* cos(0.1) + rightx* sin(0.1);
			upy = upy* cos(0.1) + righty* sin(0.1);
			upz = upz* cos(0.1) + rightz* sin(0.1);
			
			rightx = rightx* cos(0.1) - upx* sin(0.1);
			righty = righty* cos(0.1) - upy* sin(0.1);
			rightz = rightz* cos(0.1) - upz* sin(0.1);
			
			//else if(upy <= -4 && upy >=-7.1){
			
			//	upy = upy + 0.1;
			//}
			
			//upy = upy - 0.2;
			//upx = upx + 0.1;
			//else 
			//cout << upx << " " << upy << endl;
			//upz = upz - 0.1;
			//cz = cz - 0.1;
			
			//eyey = eyey+0.2;
			//eyex = eyex+0.1;
			//cy = eyey;
			//cx = eyex;
			
			//eyez = eyez - 0.1;
			//cz = eyez;
			
			//eyez = eyez + 0.1;
			
			break;
			
		case '6':
			//drawgrid=1-drawgrid;
			
			upx = upx* cos(0.1) - rightx* sin(0.1);
			upy = upy* cos(0.1) - righty* sin(0.1);
			upz = upz* cos(0.1) - rightz* sin(0.1);
			
			rightx = rightx* cos(0.1) + upx* sin(0.1);
			righty = righty* cos(0.1) + upy* sin(0.1);
			rightz = rightz* cos(0.1) + upz* sin(0.1);
			
			//d = d +1;
			//upy = upy + 0.2;
			//upx = upx - 0.1;
			//cout << upy << endl;
			//cz = cz + 0.1;
			//rx = rx + 1;
			break;
			
		case 's':
			//drawgrid=1-drawgrid;
			
			eyez = eyez - 0.1;
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
		case 'w':
			//drawgrid=1-drawgrid;
			
			eyez = eyez + 0.1;
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
		case 'i':
			//drawgrid=1-drawgrid;
			
			cx = cx + 0.2* cos(dy* toRad);
			
			cy = cy + 0.2* sin(dy* toRad);
			
			
			//cout << dy;
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
		case 'j':
			//drawgrid=1-drawgrid;
			
			dy = dy - 30;
			dy = (int)dy % 360 ;
			
			//cout << dy << sin(dy *pi/180);
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
		case 'k':
			//drawgrid=1-drawgrid;
			
			cx = cx - 0.2* cos(dy* toRad);
			
			cy = cy - 0.2* sin(dy* toRad);
			
			//cout << cy << endl;;
			
			if(cx<=-2.5 && (cy>=-2.5 && cy<=2.5)){
            			dy = dy- 90;
            			
            			cx = cx - 0.4* cos(dy* toRad);
			
				cy = cy - 0.4* sin(dy* toRad);
            			//cx = cx + 0.2*sin(dy * toRad);
            
            			//cy = cy + 0.2*cos(dy * toRad);
       			 }
			
			
			//cout << dy;
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
		case 'l':
			//drawgrid=1-drawgrid;
			
			dy = dy + 30;
			
			dy = (int)dy % 360 ;
			
			//cout << dy;
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;



		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			//b = b-1;
			eyex = eyex - 0.1*lookx;
			eyey = eyey - 0.1*looky;
			eyez = eyez - 0.1*lookz;
			//upx = upx + 0.1;
			//upy = upy + 0.2;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			//b = b+1;
			eyex = eyex + 0.1*lookx;
			eyey = eyey + 0.1*looky;
			eyez = eyez + 0.1*lookz;
			//upx = upx - 0.1;
			//upy = upy - 0.2;
			break;

		case GLUT_KEY_RIGHT:
			
			
			//cameraAngle += 0.03;
			
			eyex = eyex+0.1*rightx;
			eyey = eyey+0.1*righty;
			eyez = eyez+0.1*rightz;
		
			//a = a+1;
			break;
		case GLUT_KEY_LEFT:
		
			eyex = eyex-0.1*rightx;
			eyey = eyey-0.1*righty;
			eyez = eyez-0.1*rightz;
			
			//cameraAngle -= 0.03;
			//a = a-1;
			break;

		case GLUT_KEY_PAGE_UP:
		
			eyex = eyex+0.1*upx;
			eyey = eyey+0.1*upy;
			eyez = eyez+0.1*upz;
			
			//c = c-1;
			break;
		case GLUT_KEY_PAGE_DOWN:
			//c = c+1;
			eyex = eyex-0.1*upx;
			eyey = eyey-0.1*upy;
			eyez = eyez-0.1*upz;
			
			break;

		case GLUT_KEY_INSERT:
			break;

		case GLUT_KEY_HOME:
			break;
		case GLUT_KEY_END:
			break;

		default:
			break;
	}
}


void mouseListener(int button, int state, int x, int y){	//x, y is the x-y of the screen (2D)
	switch(button){
		case GLUT_LEFT_BUTTON:
			if(state == GLUT_DOWN){		// 2 times?? in ONE click? -- solution is checking DOWN or UP
				drawaxes=1-drawaxes;
			}
			break;

		case GLUT_RIGHT_BUTTON:
			//........
			break;

		case GLUT_MIDDLE_BUTTON:
			//........
			break;

		default:
			break;
	}
}

//void drawSphere(){

//	stackAngle = pi/2 - i* stackStep;
//	xy = 

//}

void drawFloor(){

	for(int i = -60 ;i <60; i++){
	
		for(int j = -60;j <60; j++){
		
			if((i+j)%2 == 0){
			
				glColor3f(1,1,1);
			}
			
			else{
			
				glColor3f(0,0,0);
			}
			
			glBegin(GL_QUADS);
			
			glVertex3f(i,j,0);
			glVertex3f(i+1,j,0);
			glVertex3f(i+1,j+1,0);
			glVertex3f(i,j+1,0);
			
			glEnd();
			
		}
		
	}

}

void drawWall(){

	glColor3f(1,0,0);

	glBegin(GL_QUADS);
			
	glVertex3f(3,3,1);
	glVertex3f(-3,3,1);
	glVertex3f(-3,3,0);
	
	glVertex3f(3,3,0);
			
	glEnd();
	
	glBegin(GL_QUADS);
			
	glVertex3f(-3,-3,1);
	glVertex3f(3,-3,1);
	glVertex3f(3,-3,0);
	
	glVertex3f(-3,-3,0);
			
	glEnd();
	
	glBegin(GL_QUADS);
			
	glVertex3f(3,3,1);
	glVertex3f(3,-3,1);
	glVertex3f(3,-3,0);
	
	glVertex3f(3,3,0);
			
	glEnd();
	
	glBegin(GL_QUADS);
			
	glVertex3f(-3,-3,1);
	glVertex3f(-3,3,1);
	glVertex3f(-3,3,0);
	
	glVertex3f(-3,-3,0);
			
	glEnd();

}



void display(){

	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glMatrixMode(GL_MODELVIEW);

	
	glLoadIdentity();

	//gluPerspective(45,1,0.1,50);
	gluLookAt(eyex,eyey,eyez,eyex + lookx,eyey + looky, eyez+ lookz,upx,upy,upz);
	
	//look = 0 0 -1
	//up = 0.6 0.8 0
	// right = look* up = -k * 0.6i, -k * 0.8j so 0.8 i -0.6 j, 
	
	glTranslatef(tx,ty,tz);
	
	glRotatef(45,0,0,1);
	glRotatef(45,0,1,0);
	glRotatef(rx,1,0,0);
	glRotatef(ry,0,1,0);
	glRotatef(rz,0,0,1);
	
	
	drawFloor();
	drawWall();
	glTranslatef(0,0,0.4);
	drawArrow();
	glTranslatef(cx,cy,0);
	drawSphere(0.4,18,18);
	
	glutSwapBuffers();
}


void animate(){
	angle+=0.05;
	//codes for any changes in Models, Camera
	glutPostRedisplay();
}

void init(){
	//codes for initialization
	drawgrid=0;
	drawaxes=1;
	cameraHeight=150.0;
	cameraAngle=1.0;
	angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");	//Create window with title

	init();

	glEnable(GL_DEPTH_TEST);	//enable Depth Testing

	glutDisplayFunc(display);	//display callback function
	glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

	glutKeyboardFunc(keyboardListener);
	glutSpecialFunc(specialKeyListener);
	glutMouseFunc(mouseListener);

	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
