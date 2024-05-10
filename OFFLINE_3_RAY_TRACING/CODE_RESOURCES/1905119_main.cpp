#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<iostream>
#include<fstream>
#include<vector>

#include <GL/glut.h>

#include "1905119_classes.h"
#include "bitmap_image.hpp"



using namespace std;


//float sectorCount = 18;
//float stackCount = 18;
//float radius = 0.4;

//float sectorStep = 2* pi/sectorCount;
//float stackStep = 2* pi/stackCount;
//float sectorAngle, stackAngle;

double tx = 0, ty = 0, tz = 0, rx = 0, ry = 0, rz = 0;

//gluLookAt(0,0,1,0,0,0,1,2,0);
//double eyex = 0, eyey = 0, eyez = 1, lookx = 0, looky = 0, lookz = -1,upx = 0.6, upy =0.8, upz = 0;
//double rightx = 0.8, righty = -0.6, rightz = 0;

double eyex = 200, eyey = 200, eyez = 40, lookx = -0.577, looky = -0.577, lookz = 0,upx = 0, upy =0, upz = 1;
double rightx = -0.577, righty = 0.577, rightz = 0;

double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double angle;
int recursion_level,num_Objects, num_pointLights, num_spotLights;
double image_size;
double window_size = 500;
bitmap_image image;
int imageCount = 0;

vector <Object*> objects;
vector <PointLight*> pointLights;
vector <SpotLight*> spotLights;


double viewAngle = 70;


float zRotate = 0 ;

void loadData(){

	ifstream fin("scene.txt");
	
	fin >> recursion_level >> image_size;
	
	fin >> num_Objects;
	
	for( int i=0; i< num_Objects; i++){
	
		string obj_name;
	
		fin >> obj_name;
		
		Object* o;
		
		if(obj_name == "sphere"){
		
			o = new Sphere();
			fin >> *((Sphere*)o);
			
		
		}
		
		if(obj_name == "triangle"){
		
			o = new Triangle();
			fin >> *((Triangle*)o);
			
		
		}
		
		if(obj_name == "general"){
		
			o = new General();
			fin >> *((General*)o);
			
		
		}
		
		objects.push_back(o);
		
	}
	
	Object* o = new Floor();
	
	o -> setCoEfficients( 0.4, 0.2, 0.2, 0.2 );
	
	o -> setShine(1);
	
	objects.push_back( o );
	
	fin >> num_pointLights;
	
	for( int i=0; i< num_pointLights; i++){
		
		PointLight* o = new PointLight();
		
		fin >> *o;
		
		pointLights.push_back(o);
		
	}
	
	fin >> num_spotLights;
	
	for( int i=0; i< num_spotLights; i++){
		
		SpotLight* o = new SpotLight();
		
		fin >> *o;
		
		spotLights.push_back(o);
		
	}
	

		
}

void capture(){


	//initialize bitmap image and set background color
	
	cout << "Capturing image" << endl;
	
	double windowHeight = window_size;
	double windowWidth = window_size;
	double imageHeight = image_size;
	double imageWidth = image_size;

	for(int i =0; i< imageWidth; i++){

		for(int j =0; j< imageHeight; j++){
		
			image.set_pixel(i,j,0,0,0);
		
		}	

	}
	
	
	
	double planeDistance = (windowHeight/2.0) /tan(pi* viewAngle/360);
	Vector3D topleft = Vector3D(	eyex + lookx*planeDistance - rightx*windowWidth/2 + upx*windowHeight/2,
					eyey + looky*planeDistance - righty*windowWidth/2 + upy*windowHeight/2,
					eyez + lookz*planeDistance - rightz*windowWidth/2 + upz*windowHeight/2);
					
	double du = windowWidth/imageWidth;
	double dv = windowHeight/imageHeight;
	
	// Choose middle of the grid cell
	topleft = topleft + Vector3D( rightx*(0.5*du) - upx*(0.5*dv) , righty*(0.5*du) - upy*(0.5*dv) , rightz*(0.5*du) - upz*(0.5*dv) );
	
	cout << du << " " << dv << endl << topleft;
	int nearest;
	double t, tmin;
	for (int i=1; i<imageWidth; i++){
	
		for (int j=1; j<imageHeight; j++){
		
			//calculate curPixel using topleft,r,u,i,j,du,dv
			
			Vector3D curPixel = topleft + Vector3D( du*i*rightx - dv*j*upx, du*i*righty - dv*j*upy, du*i*rightz - dv*j*upz ) ; 
			
			//cast ray from eye to (curPixel-eye) direction
			
			Vector3D eyeVec = Vector3D(eyex,eyey,eyez);
			
			Ray* ray = new Ray(eyeVec, curPixel-eyeVec);
			
			double *color = new double[3];
			
			color[0] = 0;
			color[1] = 0;
			color[2] = 0;
			
			
			//for each object, o in objects
			
			nearest = -1;
			tmin = -1;
			
			
			for(int k=0; k< objects.size(); k++){
			
				
				//t = o.intersect(ray, dummyColor, 0)
				
				//double* color = color[3];
				
				//color[0] = objects[k] -> color[0];
				//color[1] = objects[k] -> color[1];
				//color[2] = objects[k] -> color[2];
				
				t = objects[k] -> intersect(ray, color, 0);
				
				
				
				if(t>0 && ( nearest == -1 || t<tmin ) ) {
				
					tmin = t;
					nearest = k;
				
				}
				
				
				
				//if( tmin == -1 && t>0 ) tmin = t;
				
				
			
			}
			
			if(nearest != -1){
			
				//cout << nearest << endl;
			
				double t = objects[nearest] -> intersect(ray,color,1);
				
				//cout << color[0] << color[1] << color[2] << i <<" " << j << endl;
				
				for(int idx =0; idx<3; idx++){
				
					if(color[idx] >1) color[idx] = 1;
					if(color[idx] <0) color[idx] = 0;
				
				}
				
				//color[1] = 1;
				
				image.set_pixel(i,j,255*color[0],255*color[1],255*color[2]);
				
				
			
			}
			
			
		
		}
	
	}
	
	imageCount = imageCount + 1;


	image.save_image("Output_"+ to_string(imageCount) + ".bmp");


//update t so that it stores min +ve value
//save the nearest object, on
//tmin = on->intersect(ray, color, 1)
//update image pixel (i,j)
//save image // The 1st image you capture after running the program
//should be named Output_1.bmp, the 2nd image you capture should be
//named Output_2.bmp and so on.

}

void keyboardListener(unsigned char key, int x,int y){
	switch(key){
	
		case '0':
		
			cout << "Enter capturing image";
			capture();
			break;

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
			
			
		

		default:
			break;
	}
}


void specialKeyListener(int key, int x,int y){
	switch(key){
		case GLUT_KEY_DOWN:		//down arrow key
			//cameraHeight -= 3.0;
			//b = b-1;
			eyex = eyex - lookx;
			eyey = eyey - looky;
			eyez = eyez - lookz;
			//upx = upx + 0.1;
			//upy = upy + 0.2;
			break;
		case GLUT_KEY_UP:		// up arrow key
			//cameraHeight += 3.0;
			//b = b+1;
			eyex = eyex + lookx;
			eyey = eyey + looky;
			eyez = eyez + lookz;
			//upx = upx - 0.1;
			//upy = upy - 0.2;
			break;

		case GLUT_KEY_RIGHT:
			
			
			//cameraAngle += 0.03;
			
			eyex = eyex+rightx;
			eyey = eyey+righty;
			eyez = eyez+rightz;
		
			//a = a+1;
			break;
		case GLUT_KEY_LEFT:
		
			eyex = eyex-rightx;
			eyey = eyey-righty;
			eyez = eyez-rightz;
			
			//cameraAngle -= 0.03;
			//a = a-1;
			break;

		case GLUT_KEY_PAGE_UP:
		
			eyex = eyex+upx;
			eyey = eyey+upy;
			eyez = eyez+upz;
			
			//c = c-1;
			break;
		case GLUT_KEY_PAGE_DOWN:
			//c = c+1;
			eyex = eyex-upx;
			eyey = eyey-upy;
			eyez = eyez-upz;
			
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
	
	//drawFloor();
	
	//loadData();
	
	for( int i = 0;i<objects.size(); i++){
	
		//cout << objects.size();
		objects[i] -> draw();
	
	}
	
	for( int i = 0;i<num_pointLights; i++){
	
		pointLights[i] -> draw();
	
	}
	
	for( int i = 0;i<num_spotLights; i++){
	
		spotLights[i] -> draw();
	
	}
	
	
	
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
	
	loadData();
	
	image = bitmap_image(image_size, image_size);

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
