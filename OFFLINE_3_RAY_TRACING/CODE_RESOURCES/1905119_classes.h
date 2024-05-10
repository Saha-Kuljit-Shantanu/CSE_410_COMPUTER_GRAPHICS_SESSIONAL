#define pi (2*acos(0.0))

#include <cmath>

using namespace std;

class Vector3D{

	private: double x,y,z;
	
	public:
	
		Vector3D(){}
		
		Vector3D( double x, double y, double z){
		
			this -> x = x;
			this -> y = y;
			this -> z = z;
			
		}
		
		double getX(){
		
			return this -> x;
			
		}
		
		double getY(){
		
			return this -> y;
			
		}
		
		double getZ(){
		
			return this -> z;
			
		}
		
		friend Vector3D operator* (double n, Vector3D vct) ;

		Vector3D operator+ (Vector3D  vct){

		    return Vector3D( x+vct.getX(), y+vct.getY(), z+vct.getZ() );
		}

		Vector3D operator- (Vector3D  vct){

		    return Vector3D( x-vct.getX(), y-vct.getY(), z-vct.getZ() );
		}

		Vector3D operator/ (double n){

		    return Vector3D(x/n,y/n,z/n);
		}

		friend ostream& operator<< (ostream& out, const Vector3D& vct) ;
		

};

ostream& operator<< (ostream& out, const Vector3D& vct){
 
    out << vct.x << " " << vct.y << " " << vct.z << endl;

    return out;

}

Vector3D operator * ( double n,Vector3D vct) {

    return Vector3D(n*vct.x,n*vct.y,n*vct.z);
}

double dot(Vector3D vct1, Vector3D vct2){

    return vct1.getX()*vct2.getX() + vct1.getY()*vct2.getY() + vct1.getZ()*vct2.getZ();
    
}



double length(Vector3D vct){

    double val = dot(vct,vct);
    val = sqrt(val);
    return val;
    
}

Vector3D normalize(Vector3D vct){

    return vct/length(vct);
    
}

Vector3D cross(Vector3D vct1, Vector3D vct2){

    double x = vct1.getY() * vct2.getZ()- vct1.getZ() * vct2.getY();
    double y = vct1.getZ() * vct2.getX()- vct1.getX() * vct2.getZ();
    double z = vct1.getX() * vct2.getY()- vct1.getY() * vct2.getX();
    return Vector3D(x,y,z);
    
}

struct point
{
	double x,y,z;
};

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
			    	//glColor3f(1-j%2,j%2,0);
				glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                		//lower hemisphere
                		//glColor3f(j%2,1-j%2,0);
                		glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
				glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
				glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
				glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
			}glEnd();
		}
	}
}


class Object;
class PointLight;
class SpotLight;
class Ray;


extern vector <Object*> objects;
extern vector <PointLight*> pointLights;
extern vector <SpotLight*> spotLights;
extern int recursion_level;

/* ------------------------ RAY ----------------------- */

class Ray{

	public:
	
	Vector3D start;
	Vector3D dir;
	
	Ray(){
	
		
	
	}
	
	Ray( Vector3D start, Vector3D dir ){
	
		this -> start = start;
		dir = normalize(dir);
		this -> dir = dir;
	
	}

};


/* ------------------------ POINTLIGHT ----------------------- */


class PointLight{

	private:
	
	double size;

	public:

	Vector3D light_pos;
	double color[3];
	
	
	void draw(){
	
		glColor3f(color[0],color[1],color[2]);
		
		glPointSize(size);
		
		glBegin(GL_POINTS);
		
		glVertex3f( light_pos.getX(), light_pos.getY(), light_pos.getZ());
		
		glEnd();
	
	}
	void setColor(double r, double g, double b){
	
		color[0] = r;
		color[1] = g;
		color[2] = b;
	
	}
	
	PointLight(){
	
		size = 8;
	
	}
	
	PointLight( Vector3D v ){
	
		light_pos = v;
		size = 8;
	
	}
	
	PointLight( Vector3D v, double r, double g, double b ){
	
		light_pos = v;
		setColor(r,g,b);
		size = 8;
	
	}
	
	void setSize(double size){
		
		this -> size = size;	
		
	}
	
	double getSize(){
	
		return size;
	
	}
	
	friend istream& operator >> (istream &in, PointLight &p){
	
		double x,y,z;
	
		in >> x >> y >> z;
		
		Vector3D v = Vector3D(x,y,z);
		
	
		
		
		p = PointLight(v);
		
		in >> p.color[0] >> p.color[1] >> p.color[2];
		
		
		return in;
		
	
	}
	
};

/* ------------------------ SPOTLIGHT ----------------------- */

class SpotLight{

	public:

	PointLight point_light;
	Vector3D light_direction;
	double cutoff_angle;
	
	void draw(){
	
		point_light.draw();
	
	}
	
	SpotLight(){
	
		
	
	}
	
	friend istream& operator >> (istream &in, SpotLight &sp){
	
	
		in >> sp.point_light;
	
		double x,y,z;
	
		in >> x >> y >> z;
		
		sp.light_direction = Vector3D(x,y,z);
		
		sp.point_light.setSize(20);
		
		
		
		
		in >> sp.cutoff_angle;
		
		
		return in;
		
	
	}
	
};







/* ------------------------ OBJECT ----------------------- */

class Object{

	public:

	Vector3D reference_point;
	
	double color[3];
	double coEfficients[4] ;
	
	int shine ;
	
	Object( double x, double y, double z ){
	
		reference_point = Vector3D(x,y,z);
	
	}
	
	Object(){
	
	}
	
	Object( Vector3D v ){
	
		reference_point = v;
	
	}
	
	virtual void draw(){}
	
	
	void setColor(double r, double g, double b){
	
		color[0] = r;
		color[1] = g;
		color[2] = b;
	
	}
	void setShine( int shine ){
	
		this -> shine = shine;
	
	}
	void setCoEfficients(double amb, double dif, double spe, double ref){
	
		coEfficients[0] = amb;
		coEfficients[1] = dif;
		coEfficients[2] = spe;
		coEfficients[3] = ref;
	
	}
	
	virtual double getTmin(Ray *r, double *color, int level){
	
		return -1;
	
	}
	
	virtual Ray getNormal(Vector3D intercept, Ray* incident){ return Ray(); }
	
	virtual double intersect(Ray *r, double *color, int level){
	
		double tmin = getTmin(r, color, level);
		
		//cout << tmin << endl;
	
		if (level == 0) return tmin;
		Vector3D intersectionPoint = r->start + tmin*r->dir;
		
		//color = new double[3];
		//double intersectionPointColor = getColorAt(intersectionPoint)
		color[0] = this -> color[0]*coEfficients[0] ;
		color[1] = this -> color[1]*coEfficients[0] ;
		color[2] = this -> color[2]*coEfficients[0] ;
		
		
		
		//cast ray from pl.light_pos to intersectionPoint
		
		for(int i =0 ; i< pointLights.size(); i++){
		
			Vector3D start = pointLights[i] -> light_pos;
			Vector3D dir = intersectionPoint - start;
		
			Ray* lightRay = new Ray( start,dir );
			
			//calculate normal at intersectionPoint
			
			Ray normal = getNormal( intersectionPoint, lightRay);
			
			
			
			// if intersectionPoint is in shadow, the diffuse
			// and specular components need not be calculated
			
			double t_shadow = length(intersectionPoint - start) ;
			
			if( floor(t_shadow) <=0 ) continue;
			
			//if ray is not obscured by any object
			
			bool obscure = false;
			
			for(int j=0; j<objects.size(); j++){
			
				double t_obscure = objects[i] -> getTmin(lightRay,color,0);
				if(t_obscure > 0 && ceil(t_obscure) <= t_shadow){
				
					obscure = true;
					break;
				
				}
			
			}
			
			if( obscure == false){
			
				//calculate lambertValue using normal, ray
				
				double lambertValue = max(0.0, dot(normal.dir, dir) );
				//find reflected ray, rayr for ray
				
				Vector3D reflectVec = 2*dot(dir,normal.dir)*normal.dir - dir;
			
				Ray reflection = Ray( intersectionPoint, reflectVec);
			
			
				//calculate phongValue using r, rayr
				
				double phongValue = max(0.0, dot(reflection.dir, r -> dir) );
				
				color[0] = color[0] + pointLights[i] -> color[0]*coEfficients[1] * lambertValue* this -> color[0] +  pointLights[i] -> color[0] * coEfficients[2]* pow(phongValue, shine) * this -> color[0];
				
				color[1] = color[1] + pointLights[i] -> color[1]*coEfficients[1] * lambertValue* this -> color[1] + 		pointLights[i] -> color[1] * coEfficients[2]* pow(phongValue, shine) * this -> color[1];
				
				color[2] = color[2] + pointLights[i] -> color[2]*coEfficients[1] * lambertValue* this -> color[2] + 		pointLights[i] -> color[2] * coEfficients[2]* pow(phongValue, shine) * this -> color[2];
				
				

			}
		
		}
		
		
		// Do the same calculation for each spot light unless
		// the ray cast from light_pos to intersectionPoint
		// exceeds cutoff-angle for the light source
		
		for(int i =0 ; i< spotLights.size(); i++){
		
			Vector3D start = spotLights[i] -> point_light.light_pos;
			Vector3D pldir = intersectionPoint - start;
			Vector3D spldir = spotLights[i] -> light_direction;
			
			double pldotspl = dot(pldir, spldir);
			double pl = length(pldir);
			double spl = length(spldir);
			
			double cosVal = pldotspl/(pl*spl);
			
			double cutoff = 180*acos(cosVal)/pi;
			
			if( cutoff < spotLights[i] -> cutoff_angle ){
			
				Ray* lightRay = new Ray( start,pldir );
			
				//calculate normal at intersectionPoint
				
				Ray normal = getNormal( intersectionPoint, lightRay);
				
				
				
				// if intersectionPoint is in shadow, the diffuse
				// and specular components need not be calculated
				
				double t_shadow = length(intersectionPoint - start) ;
				
				if( floor(t_shadow) <=0 ) continue;
				
				//if ray is not obscured by any object
				
				bool obscure = false;
				
				for(int j=0; j<objects.size(); j++){
				
					double t_obscure = objects[i] -> getTmin(lightRay,color,0);
					if(t_obscure > 0 && ceil(t_obscure) <= t_shadow){
					
						obscure = true;
						break;
					
					}
				
				}
				
				if( obscure == false){
				
					//calculate lambertValue using normal, ray
					
					double lambertValue = max(0.0, dot(normal.dir, pldir) );
					//find reflected ray, rayr for ray
					
					Vector3D reflectVec = 2*dot(pldir,normal.dir)*normal.dir - pldir;
				
					Ray reflection = Ray( intersectionPoint, reflectVec);
				
				
					//calculate phongValue using r, rayr
					
					double phongValue = max(0.0, dot(reflection.dir, r -> dir) );
					
					color[0] = color[0] + pointLights[i] -> color[0]*coEfficients[1] * lambertValue* this -> color[0] +  pointLights[i] -> color[0] * coEfficients[2]* pow(phongValue, shine) * this -> color[0];
					
					color[1] = color[1] + pointLights[i] -> color[1]*coEfficients[1] * lambertValue* this -> color[1] + 		pointLights[i] -> color[1] * coEfficients[2]* pow(phongValue, shine) * this -> color[1];
					
					color[2] = color[2] + pointLights[i] -> color[2]*coEfficients[1] * lambertValue* this -> color[2] + 		pointLights[i] -> color[2] * coEfficients[2]* pow(phongValue, shine) * this -> color[2];
					
					

				}
			
				
			
			}
			
			
			
			
		
		}
		
		
		
		if( level >= recursion_level) return tmin;
		
		//construct reflected ray from intersection point
		
		Ray normal = getNormal( intersectionPoint, r);
		
		Vector3D reflectVec = 2*dot(r -> dir,normal.dir)*normal.dir - r -> dir;
		
		// actually slightly forward from the point (by moving the start a little bit towards the reflection direction) to avoid self intersection
			
		Ray* reflection = new Ray( intersectionPoint + 0.0002* reflectVec, reflectVec);
		
		int nearest = -1;
		double t = -1, tm = -1;
		
		for(int k=0; k< objects.size(); k++){
				
			t = objects[k] -> intersect(reflection, color, 0);
				
				
				
			if(t>0 && ( nearest == -1 || t<tm ) ) {
				
				tm = t;
				nearest = k;
				
			}
				
				
				
				
				
				
			
		}
			
		if(nearest != -1){
		
			double *tempcolor = new double[3];
			
			tempcolor[0] = 0;
			tempcolor[1] = 0;
			tempcolor[2] = 0;
			
				
			
			double t = objects[nearest] -> intersect(reflection,tempcolor,level+1);
				
			color[0] = color[0] + tempcolor[0]*coEfficients[3];
			color[1] = color[1] + tempcolor[1]*coEfficients[3];
			color[2] = color[2] + tempcolor[2]*coEfficients[3];
				
				
				
				
			
		}
	
	}
	

};

class Sphere : public Object{

	public:

	double radius;
	
	Sphere( double radius, Vector3D v) : Object(v){
	
		this -> radius = radius;
	
	}
	
	Sphere( ) : Object(){
	
	
	}

	void draw(){
	
		
		glColor3f(color[0],color[1],color[2]);
		glTranslatef( reference_point.getX(), reference_point.getY(), reference_point.getZ() );
		drawSphere(radius,60,60);
		glTranslatef( -reference_point.getX(), -reference_point.getY(), -reference_point.getZ() );
		
	
	}
	
	friend istream& operator >> (istream &in, Sphere &s){
	
		double x,y,z, r;
	
		in >> x >> y >> z;
		
		Vector3D v = Vector3D(x,y,z);
	
		in >> r;
		
		s = Sphere(r,v);
		
		in >> s.color[0] >> s.color[1] >> s.color[2];
		
		in >> s.coEfficients[0] >> s.coEfficients[1] >> s.coEfficients[2] >> s.coEfficients[3];
		
		in >> s.shine;
		
		return in;
		
	
	}
	
	Ray getNormal(Vector3D intercept, Ray* incident){
	
		return Ray(intercept, reference_point - intercept);
	
	}
	
	double getTmin(Ray *r, double *color, int level){
	
		/*double start_pos = dot( r -> start, r-> start);
		
		double tp = -dot(r -> start, r -> dir);
		
		if( start_pos > radius*radius && tp < 0) return -1;
		
		double d_square = start_pos - tp*tp;
		
		double dist_square = radius * radius - d_square;
		
		if( dist_square < 0 ) return -1;
		
		else{
		
			double dist = sqrt( dist_square );
			
			double tmin;
			
			if( start_pos < 0 ) tmin = tp - dist;
			
			else if( start_pos > 0 ) tmin = tp + dist;
			
			else tmin = tp;
			
			if(tmin < 0) return -1;
			
			return tmin;
		
		}*/
		
		//cout << "In sphere" << endl;
		
		Vector3D start = r -> start - reference_point;
		
		Vector3D dir = r-> dir;
		
		double start_pos = dot( start, start);
		
		double r_square = radius* radius;
		
		double b = 2*dot(start, dir), c = start_pos - r_square;
		
		double discriminant =  b*b - 4*c;
		
		//cout << dir;
		
		if( discriminant < 0) return -1;
		
		double t1 = ( -b + sqrt(discriminant) )/ 2 ;
		
		double t2 = ( -b - sqrt(discriminant) )/ 2 ;
		
		if(t2 > 0) return t2;
		
		else if(t1 > 0) return t1;
		
		else return -1;
		
		
	
		
	
	}

};

class Triangle : public Object{

	private:
	
	double determinant(double ara[3][3]){
	
		double v1 = ara[0][0] * ( ara[1][1] * ara[2][2] - ara[1][2] * ara[2][1] );
		double v2 = ara[1][0] * ( ara[0][1] * ara[2][2] - ara[2][1] * ara[0][2] );
		double v3 = ara[2][0] * ( ara[0][1] * ara[1][2] - ara[1][1] * ara[0][2] );
		
		return v1 -v2 +v3;
	
	}

	public:

	Vector3D ref2,ref3 ;
	
	Triangle( Vector3D v1, Vector3D v2, Vector3D v3 ) : Object(v1){
	
		this -> ref2 = v2;
		this -> ref3 = v3;
	
	}
	
	Triangle() : Object(){
	
	}

	void draw(){
	
		
		glColor3f(color[0],color[1],color[2]);
		//glTranslatef( reference_point.getX(), reference_point.getY(), reference_point.getZ() );
		glBegin(GL_TRIANGLES);{
		
			glVertex3f( reference_point.getX(), reference_point.getY(), reference_point.getZ());
			glVertex3f( ref2.getX(), ref2.getY(), ref2.getZ());
			glVertex3f( ref3.getX(), ref3.getY(), ref3.getZ());
			
		}glEnd();
		//glTranslatef( -reference_point.getX(), -reference_point.getY(), -reference_point.getZ() );
		
	
	}
	
	friend istream& operator >> (istream &in, Triangle &t){
	
		double x,y,z;
	
		in >> x >> y >> z;
		
		Vector3D v1 = Vector3D(x,y,z);
		
		in >> x >> y >> z;
		
		Vector3D v2 = Vector3D(x,y,z);
		
		in >> x >> y >> z;
		
		Vector3D v3 = Vector3D(x,y,z);
	
		
		
		t = Triangle(v1,v2,v3);
		
		in >> t.color[0] >> t.color[1] >> t.color[2];
		
		in >> t.coEfficients[0] >> t.coEfficients[1] >> t.coEfficients[2] >> t.coEfficients[3];
		
		in >> t.shine;
		
		return in;
		
	
	}
	
	Ray getNormal(Vector3D intercept, Ray* incident){
	
		Vector3D dir = cross(ref2 - reference_point, ref3 - reference_point);
	
		if( dot(incident -> dir , dir ) < 0 ) return Ray(intercept, dir );
		return Ray(intercept, -1*dir );
	
	}
	
	
	double getTmin(Ray *r, double *color, int level){
	
		//cout << "In Triangle" << endl;
	
		Vector3D a_b = reference_point - ref2;
		
		Vector3D a_c = reference_point - ref3;
		
		Vector3D a_R0 = reference_point - r -> start;
		
		Vector3D dir = r -> dir;
	
		double betaMat[3][3] =  { { a_R0.getX(), a_c.getX(), dir.getX() }, 
						{ a_R0.getY(), a_c.getY(), dir.getY() },
						{ a_R0.getZ(), a_c.getZ(), dir.getZ() } } ;
						
		double gammaMat[3][3] =  { { a_b.getX(), a_R0.getX(), dir.getX() }, 
						{ a_b.getY(), a_R0.getY(), dir.getY() },
						{ a_b.getZ(), a_R0.getZ(), dir.getZ() } } ;
						
		double tMat[3][3] =  { { a_b.getX(), a_c.getX(), a_R0.getX() }, 
						{ a_b.getY(), a_c.getY(), a_R0.getY() },
						{ a_b.getZ(), a_c.getZ(), a_R0.getZ() } } ;
						
		double AMat[3][3] =  { { a_b.getX(), a_c.getX(), dir.getX() }, 
						{ a_b.getY(), a_c.getY(), dir.getY() },
						{ a_b.getZ(), a_c.getZ(), dir.getZ() } } ;
						
						
		double betaDet = determinant(betaMat);
		double gammaDet = determinant(gammaMat);
		double tDet = determinant(tMat);
		double ADet = determinant(AMat);
						
		
		double beta = betaDet/ADet;
		double gamma = gammaDet/ADet;
		double t = tDet/ADet;
		
		if( beta+ gamma <1 && beta>0 && gamma>0 && t>0 ) return t;
		
		else return -1;
						
						
		
						
		
						
						
	
	}

};

class General : public Object{

	public:
	
	double A, B, C, D, E, F, G, H, I, J;

	double length, width, height ;
	
	General( Vector3D v1 ) : Object(v1){
	
		
	
	}
	
	General() : Object(){
	
	}

	void draw(){}
	
	bool ok( Vector3D intercept){
	
		if( floor(length) > 0 ){
		
			if( intercept.getX() < reference_point.getX() ) return false;
			if( intercept.getX() > reference_point.getX() + length ) return false;
		
		}
		
		if( floor(width) > 0 ){
		
			if( intercept.getY() < reference_point.getY() ) return false;
			if( intercept.getY() > reference_point.getY() + width ) return false;
		
		}
		
		if( floor(height) > 0 ){
		
			if( intercept.getZ() < reference_point.getZ() ) return false;
			if( intercept.getZ() > reference_point.getZ() + height ) return false;
		
		}
		
		return true;
	
	}
	
	double getTmin(Ray *r, double *color, int level){
	
		double startx = r -> start.getX();
		double starty = r -> start.getY();
		double startz = r -> start.getZ();
		
		double dirx = r -> dir.getX();
		double diry = r -> dir.getY();
		double dirz = r -> dir.getZ();
		
		double a = A*dirx*dirx + B*diry*diry + C*dirz*dirz + D*dirx*diry + E*dirx*dirz + F*diry*dirz;
		
		double b = 2* A* startx * dirx + 2* B* starty * diry + 2* C* startz* dirz 
			+ D* (startx* diry + starty* dirx) + E* (startx* dirz + startz* dirx) + F * (starty* dirz + startz* diry)
			+ G* dirx + H* diry + I* dirz;
			
		double c = A*startx*startx + B*starty*starty + C*startz*startz + D*startx*starty + E*startx*startz + F*starty*startz
			+ G* startx + H* starty + I* startz+ J;
			
		double discriminant = b*b - 4*a *c;
		
		if( discriminant < 0) return -1;
		
		if( a < 0.0002 && a > -0.0002){
		
			return -c/b;
		
		}
		
		double t1 = ( -b + sqrt(discriminant) )/ (2*a) ;
		
		double t2 = ( -b - sqrt(discriminant) )/ (2*a) ;
		
		if(t2 > 0) {
		
			Vector3D intersectionPoint = r -> start + t2* r -> dir;
			
			if( ok(intersectionPoint) == true ) 
			
				return t2;
		
		}
		
		if(t1 > 0) {
		
			Vector3D intersectionPoint = r -> start + t1* r -> dir;
			
			if( ok(intersectionPoint) == true ) 
			
				return t1;
		
		}
		
		return -1;
	
	}



	friend istream& operator >> (istream &in, General &g){
	
		in >> g.A >> g.B >> g.C >> g.D >> g.E >> g.F >> g.G >> g.H >> g.I >> g.J ;
		
		double x,y,z;
		
		in >> x >> y >> z;
		
		Vector3D vref = Vector3D(x,y,z);
		
		g.reference_point = vref;
		
		in >> g.length >> g.width >> g.height;
		
		

		
		in >> g.color[0] >> g.color[1] >> g.color[2];
		
		in >> g.coEfficients[0] >> g.coEfficients[1] >> g.coEfficients[2] >> g.coEfficients[3];
		
		in >> g.shine;
		
		return in;
		
	
	}
	
	
	Ray getNormal(Vector3D intercept, Ray* incident){
	
		double ddx = 2*A*intercept.getX() + D* intercept.getY() +E* intercept.getZ() +G;
		
		double ddy = 2*B*intercept.getY() + D* intercept.getX() +F* intercept.getZ() +H;
		
		double ddz = 2*C*intercept.getZ() + E* intercept.getX() +F* intercept.getY() +I;
		
		return Ray( intercept, Vector3D(ddx,ddy,ddz) );
	
	}

};

class Floor: public Object{

	public:
	
	double floorWidth, tileWidth;

	Floor( double floorWidth=1000, double tileWidth=20) : Object(){
	
		this -> floorWidth = floorWidth;
		this -> tileWidth = tileWidth;
	
	}

	void draw(){

		for(int i = -floorWidth/2 ;i <floorWidth/2 ; i+=tileWidth){
		
			for(int j = -floorWidth/2 ;j <floorWidth/2 ; j+=tileWidth){
			
				int m= abs(i)/tileWidth;
				int n = abs(j)/tileWidth;
			
				if((m+n)%2 == 0){
				
					glColor3f(1,1,1);
				}
				
				else{
				
					glColor3f(0,0,0);
				}
				
				glBegin(GL_QUADS);
				
				glVertex3f(i,j,0);
				glVertex3f(i+tileWidth,j,0);
				glVertex3f(i+tileWidth,j+tileWidth,0);
				glVertex3f(i,j+tileWidth,0);
				
				glEnd();
				
			}
			
		}

	}
	
	Ray getNormal(Vector3D intercept, Ray* incident){
	
		Vector3D dir = Vector3D(0,0,1);
	
		return Ray(intercept, dir );
	
	}
	
	
	double getTmin(Ray *r, double *color, int level){
	
		
		//Ray plane intersection
		
		Vector3D normal = Vector3D(0,0,1);
		
		Vector3D dir = r -> dir;
		
		Vector3D start = r -> start;
		
		double ndir = dot(normal,dir);
		double nstart = dot(normal,start);
		
		double t = -nstart/ndir;
		
		//Additional checks
		
		Vector3D plane = start + t*dir;
		
		double w = floorWidth/2;
		
		int m= floor(plane.getX()/tileWidth);
		int n = floor(plane.getY()/tileWidth);
		
		int col = ( abs(m+n) +1 )%2;
		
		//if ( ( m+n )%2 == 0 ){ 
		
		this -> color[0] = col;
		this -> color[1] = col;
		this -> color[2] = col;
		
		//}
		
		if( plane.getX() > -w && plane.getX() < w && plane.getY() > -w && plane.getY() < w ) return t;
		
		return -1;
		
	
	}
	

};



