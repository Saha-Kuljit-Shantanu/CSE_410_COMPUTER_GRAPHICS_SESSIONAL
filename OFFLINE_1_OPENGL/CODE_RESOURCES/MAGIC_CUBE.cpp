#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<vector>

#include <GL/glut.h>

#define pi (2*acos(0.0))

using namespace std;


//float sectorCount = 18;
//float stackCount = 18;
//float radius = 0.4;

//float sectorStep = 2* pi/sectorCount;
//float stackStep = 2* pi/stackCount;
//float sectorAngle, stackAngle;

double tx = 0, ty = 0, tz = -15, rx = 0, ry = 0, rz = 0;

//gluLookAt(0,0,1,0,0,0,1,2,0);
double eyex = 0, eyey = 0, eyez = 1, lookx = 0, looky = 0, lookz = -1,upx = 0.6, upy =0.8, upz = 0;
double rightx = 0.8, righty = -0.6, rightz = 0;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;

struct point
{
	double x,y,z;
};


void drawAxes()
{
	if(drawaxes==1)
	{
		
		glBegin(GL_LINES);{
		
			glColor3f(1.0, 0, 0);
			glVertex3f( 100,0,0);
			glVertex3f(-100,0,0);

			glColor3f(0, 1.0, 0);
			glVertex3f(0,-100,0);
			glVertex3f(0, 100,0);

			glColor3f(0, 0, 1.0);
			glVertex3f(0,0, 100);
			glVertex3f(0,0,-100);
		}glEnd();
	}
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

double TriangleLength = 3;
double maxLength = 3;
double scale = 0.1;
double sphereRadius = 0;

std::vector<float> buildUnitPositiveX(int subdivision)
{
    const float DEG2RAD = acos(-1) / 180.0f;

    std::vector<float> vertices;
    float n1[3];        // normal of longitudinal plane rotating along Y-axis
    float n2[3];        // normal of latitudinal plane rotating along Z-axis
    float v[3];         // direction vector intersecting 2 planes, n1 x n2
    float a1;           // longitudinal angle along Y-axis
    float a2;           // latitudinal angle along Z-axis

    // compute the number of vertices per row, 2^n + 1
    int pointsPerRow = (int)pow(2, subdivision) + 1;

    // rotate latitudinal plane from 45 to -45 degrees along Z-axis (top-to-bottom)
    for(unsigned int i = 0; i < pointsPerRow; ++i)
    {
        // normal for latitudinal plane
        // if latitude angle is 0, then normal vector of latitude plane is n2=(0,1,0)
        // therefore, it is rotating (0,1,0) vector by latitude angle a2
        a2 = DEG2RAD * (45.0f - 90.0f * i / (pointsPerRow - 1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;

        // rotate longitudinal plane from -45 to 45 along Y-axis (left-to-right)
        for(unsigned int j = 0; j < pointsPerRow; ++j)
        {
            // normal for longitudinal plane
            // if longitude angle is 0, then normal vector of longitude is n1=(0,0,-1)
            // therefore, it is rotating (0,0,-1) vector by longitude angle a1
            a1 = DEG2RAD * (-45.0f + 90.0f * j / (pointsPerRow - 1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            // find direction vector of intersected line, n1 x n2
            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            // normalize direction vector
            float scale = sphereRadius / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            // add a vertex into array
            vertices.push_back(v[0]);
            vertices.push_back(v[1]);
            vertices.push_back(v[2]);
        }
    }

    return vertices;
}

int subdivision = 8;

void drawCubeSphere(){

	int i,j;

	vector<float> vertices = buildUnitPositiveX(subdivision);
	
	int QuadPerRow = (int)pow(2, subdivision);
	
	//cout << vertices.size() << endl;
	
	int PointsNext = 3* (QuadPerRow+1);
	
	
	
	for(i =0 ;i <QuadPerRow; i++){
	
		for(j =0; j<QuadPerRow;j++){
		
			
			glBegin(GL_QUADS);
			glVertex3f( vertices[i* PointsNext + 3*j], vertices[i* PointsNext + 3*j + 1], vertices[i* PointsNext + 3*j + 2] );
			glVertex3f( vertices[i* PointsNext + 3*j + 3], vertices[i* PointsNext + 3*j + 4], vertices[i* PointsNext + 3*j + 5] );
			glVertex3f( vertices[ (i+1) * PointsNext + 3*j + 3], vertices[ (i+1) * PointsNext + 3*j + 4], vertices[ (i+1) * PointsNext + 3*j + 5] );
			glVertex3f( vertices[ (i+1) * PointsNext + 3*j ] , vertices[ (i+1) * PointsNext + 3*j + 1], vertices[ (i+1) * PointsNext + 3*j + 2] );
			glEnd();
			
		}
	
	}

}

void drawAllCube(){


	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(maxLength-sphereRadius,0,0);
	drawCubeSphere();
	glPopMatrix();
	
	
	glColor3f(0,1,0);
	//glTranslatef(0,0,);
	glPushMatrix();
	
	glTranslatef(0,0,-maxLength+sphereRadius);
	glRotatef(90,0,1,0);
	//
	
	drawCubeSphere();
	glPopMatrix();
	//glTranslatef(0,0,maxLength-sphereRadius);
	
	glColor3f(0,0,1);
	glPushMatrix();
	
	glTranslatef(-maxLength+sphereRadius,0,0);
	glRotatef(180,0,1,0);
	//
	drawCubeSphere();
	glPopMatrix();
	
	glColor3f(1,0.6,0);
	glPushMatrix();
	
	glTranslatef(0,0,maxLength-sphereRadius);
	glRotatef(270,0,1,0);
	//
	drawCubeSphere();
	glPopMatrix();
	
	glColor3f(1,0.7,0.7);
	glPushMatrix();
	
	glTranslatef(0,maxLength-sphereRadius,0);
	glRotatef(90,0,0,1);
	
	//
	drawCubeSphere();
	glPopMatrix();
	
	glColor3f(1,0.7,0.7);
	glPushMatrix();
	
	glTranslatef(0,-maxLength+sphereRadius,0);
	glRotatef(270,0,0,1);
	//glTranslatef(0,0,maxLength-sphereRadius);
	drawCubeSphere();
	glPopMatrix();
		

}

void drawTriangle(){

	
	glBegin(GL_TRIANGLES);
	glVertex3f(TriangleLength,0,0);
	glVertex3f(0,TriangleLength,0);
	glVertex3f(0,0,TriangleLength);
	glEnd();
}

void drawPyramid(int j){

	int i;
	
	float difference = maxLength- TriangleLength;
	difference = difference/sqrt(3);

	
	for(i = 0; i< 4;i++){
	
		glColor3f((j+1)%2,j%2,1);
		glPushMatrix();
		glRotatef(90*i,0,1,0);
		glTranslatef(difference,difference,difference);
		drawTriangle();
		glPopMatrix();
		j++;
		
	
	}

}

void drawHedral(){

	
	drawPyramid(0);
	glRotatef(180,1,0,1);
	//glColor3f(1,0,1);
	drawPyramid(1);
	

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

void drawCylinder(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * 2.0 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            GLfloat c = (2+cos(theta))/3;
            //glColor3f(c,c,c);
            glColor3f(1,1,0);
            glVertex3f(currx, curry, height/2);
            glVertex3f(currx, curry, -height/2);

            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}

void drawFourCylinders(){

	int i,j;
	
	double TriangleHeight = TriangleLength * sqrt(2);

	glPushMatrix();
	glTranslatef(0,0,TriangleLength/2);
	glTranslatef(TriangleLength/2,0,0);
	glRotatef(-45,0,1,0);
	drawCylinder(TriangleHeight,sphereRadius,120);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0,0,-TriangleLength/2);
	glTranslatef(TriangleLength/2,0,0);
	glRotatef(45,0,1,0);
	drawCylinder(TriangleHeight,sphereRadius,120);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0,0,TriangleLength/2);
	glTranslatef(-TriangleLength/2,0,0);
	glRotatef(45,0,1,0);
	drawCylinder(TriangleHeight,sphereRadius,120);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(0,0,-TriangleLength/2);
	glTranslatef(-TriangleLength/2,0,0);
	glRotatef(-45,0,1,0);
	drawCylinder(TriangleHeight,sphereRadius,120);
	glPopMatrix();
	
	
	//glPopMatrix();
	
	//glPushMatrix();
	//glRotatef(45,0,1,0);
	//drawCylinder(TriangleLength,sphereRadius,120);
	//glPopMatrix();
	
	//glPushMatrix();
	//glRotatef(-45,0,1,0);
	//drawCylinder(TriangleLength,sphereRadius,120);
	//glPopMatrix();

}

void drawCylinders(){

	
	drawFourCylinders();
	
	glPushMatrix();
	glRotatef(90,0,0,1);
	drawFourCylinders();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(90,1,0,0);
	drawFourCylinders();
	glPopMatrix();
	


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

float zRotate = 0 ;

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
			
		case 'd':
			//drawgrid=1-drawgrid;
			
			zRotate = zRotate - 0.6;
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
		case 'a':
			//drawgrid=1-drawgrid;
			
			zRotate = zRotate + 0.6;
			
			//e = e -1;
			//cy = cy-0.2;
			//cx = cx-0.1;

			break;
			
			
		case ',':
		
			if(TriangleLength > 0) TriangleLength = TriangleLength - scale;
			if(sphereRadius < maxLength) sphereRadius = sphereRadius + scale;
		
			break;
			
		case '.':
		
			if(TriangleLength < maxLength) TriangleLength = TriangleLength + scale;
			if(sphereRadius > 0)sphereRadius = sphereRadius - scale;
		
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
	glRotatef(zRotate,0,0,1);
	
	drawAxes();
	
	//vector<float> x = buildUnitPositiveX(12);
	
	//cout << x[0] << endl;
	//cout << x[1] << endl;
	//cout << x[2] << endl;
	
	
	//drawHedral();
	
	//drawFloor();
	//drawWall();
	//glTranslatef(0,0,0.5);
	drawCylinders(); //348
	drawHedral();
	drawAllCube();
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
