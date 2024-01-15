#include <iostream>
#include <cmath>
#include <vector>


using namespace std;

class vector3{

    private :

        double x,y,z,t;

    public :

        vector3(){


        }

        vector3(double x, double y, double z, double t=1){

            this -> x = x;
            this -> y = y;
            this -> z = z;
            this -> t = t;

        }

        void setX(double x){

            this -> x = x;

        }

        double getX(){

            return x;
        }

        void setY(double y){

            this -> y = y;

        }

        double getY(){

            return y;
        }

        void setZ(double z){

            this -> z = z;

        }

        double getZ(){

            return z;
        }

        void setT(double t){

            this -> t = t;

        }

        double getT(){

            return t;
        }

        friend vector3 operator* (double n, vector3 vct) ;

        vector3 operator+ (vector3  vct){

            return vector3( x+vct.getX(), y+vct.getY(), z+vct.getZ() );
        }

        vector3 operator- (vector3  vct){

            return vector3( x-vct.getX(), y-vct.getY(), z-vct.getZ() );
        }

        vector3 operator/ (double n){

            return vector3(x/n,y/n,z/n,t);
        }

        friend ostream& operator<< (ostream& out, const vector3& vct) ;

        
            
            


    
};

ostream& operator<< (ostream& out, const vector3& vct){
 
    out << vct.x << " " << vct.y << " " << vct.z << endl;

    return out;

}

vector3 operator * ( double n,vector3 vct) {

    return vector3(n*vct.x,n*vct.y,n*vct.z,vct.t);
}

double dot(vector3 vct1, vector3 vct2){

    return vct1.getX()*vct2.getX() + vct1.getY()*vct2.getY() + vct1.getZ()*vct2.getZ();
}

double dot4(vector3 vct1, vector3 vct2){

    return vct1.getX()*vct2.getX() + vct1.getY()*vct2.getY() + vct1.getZ()*vct2.getZ() + vct1.getT() * vct2.getT();
}

vector3 cross(vector3 vct1, vector3 vct2){

    double x = vct1.getY() * vct2.getZ()- vct1.getZ() * vct2.getY();
    double y = vct1.getZ() * vct2.getX()- vct1.getX() * vct2.getZ();
    double z = vct1.getX() * vct2.getY()- vct1.getY() * vct2.getX();
    return vector3(x,y,z);
}

double max3(double a, double b, double c){

    return max(a, max(b,c));
}

double min3(double a, double b, double c){

    return min(a, min(b,c));
}

double slopeXY(vector3 a,vector3 b){

    double dy = a.getY() - b.getY();
    double dx = a.getX() - b.getX();

    return dy/dx;

}

double slopeYZ(vector3 a,vector3 b){

    double dz = a.getZ() - b.getZ();
    double dy = a.getY() - b.getY();
    
    return dz/dy;
    
}

double slopeZX(vector3 a,vector3 b){

    double dx = a.getX() - b.getX();
    double dz = a.getZ() - b.getZ();

    return dx/dz;
    
}

class matrix{

    public:

        vector3 vct[4];

        matrix(){

            vct[0] = vector3(1,0,0,0);
            vct[1] = vector3(0,1,0,0);
            vct[2] = vector3(0,0,1,0);
            vct[3] = vector3(0,0,0,1);
        }

        matrix(vector3 vct0,vector3 vct1,vector3 vct2,vector3 vct3){

            vct[0] = vct0;
            vct[1] = vct1;
            vct[2] = vct2;
            vct[3] = vct3;

        }

        matrix operator * ( matrix M) {

            vector3 vct2[4];

            vct2[0] = vector3(vct[0].getX(),vct[1].getX(),vct[2].getX(),vct[3].getX());
            vct2[1] = vector3(vct[0].getY(),vct[1].getY(),vct[2].getY(),vct[3].getY());
            vct2[2] = vector3(vct[0].getZ(),vct[1].getZ(),vct[2].getZ(),vct[3].getZ());
            vct2[3] = vector3(vct[0].getT(),vct[1].getT(),vct[2].getT(),vct[3].getT());

            vector3 vct3[4];

            for(int i=0 ;i<4;i++){

                vct3[i].setX( dot4( vct2[0],M.vct[i] ) );
                vct3[i].setY( dot4( vct2[1],M.vct[i] ) );
                vct3[i].setZ( dot4( vct2[2],M.vct[i] ) );
                vct3[i].setT( dot4( vct2[3],M.vct[i] ) );


            }

            return matrix(vct3[0],vct3[1],vct3[2],vct3[3]);
            
        }

        void scale(){

            for(int i=0;i<3;i++) {

                vct[i] = vct[i]/vct[i].getT();
                vct[i].setT(1);

            }

        }

        friend ostream& operator<< (ostream& out, const matrix& mat) ;


};

ostream& operator<< (ostream& out, const matrix& mat){

    for(int i=0; i<3; i++){

        out << mat.vct[i];
    }

    out << endl;

    return out;
}

vector3 normalize(vector3 vct){

    double val = dot(vct,vct);
    val = sqrt(val);
    return vct/val;
}

vector3 rodriguez(vector3 vct1, vector3 vct2, double theta){

    // vct1 -------> The vector to be rotated
    // vct2 -------> The axis of rotation( ax ay az )

    theta = M_PI* theta/180;

    double a = ( 1 - cos(theta) )* dot(vct1,vct2) ;

    vector3 temp_vct1 = cos(theta) * vct1;
    vector3 temp_vct2 = a * vct2;
    vector3 temp_vct3 = sin(theta) * cross(vct2,vct1);
    return temp_vct1 + temp_vct2 + temp_vct3 ;

}

class Triangle{

    public:

        vector3 vtx[3];

        Triangle(){


        }

        Triangle(vector3 a, vector3 b, vector3 c){

            vtx[0] = a;
            vtx[1] = b;
            vtx[2] = c;
            
        }

        Triangle(matrix M){

            for(int i = 0; i<3; i++){

                vtx[i] = M.vct[i];
            }


        }
};

vector<Triangle> TriangleSet;



// int main(){

//     vector3 va(2,0,0,0);
//     vector3 vb(0,2,0,0);
//     vector3 vc(0,0,2,0);
//     vector3 vd(20,0,0);

//     matrix ma(va,vb,vc,vd);

//     vector3 ve(0,1,0,0);
//     vector3 vf(-1,0,0,0);
//     vector3 vg(0,0,1,0);
//     vector3 vh(0,2,0);
//     vh.setY(0);

//     matrix mb(ve,vf,vg,vh);

//     ma = ma * mb;

//     for(int i=0; i<4;i++){

//         cout << ma.vct[i].getX() << " " << ma.vct[i].getY() << " " << ma.vct[i].getZ() << " " << ma.vct[i].getT() << endl;
//     }

//     vector3 vun(3,4,0);

//     vector3 vn = normalize(vun);

//     cout << vn.getX() << " " << vn.getY() << " " << vn.getZ() << " " << vn.getT() << endl;

//     vn = rodriguez(va,vc,90);

//     cout << fixed << setprecision(7) << vn.getX() << " " << vn.getY() << " " << vn.getZ() << " " << vn.getT() << endl;

//     vn = rodriguez(vb,vc,90);

//     cout << vn.getX() << " " << vn.getY() << " " << vn.getZ() << " " << vn.getT() << endl;

//     vn = rodriguez(vc,vc,90);

//     cout << vn.getX() << " " << vn.getY() << " " << vn.getZ() << " " << vn.getT() << endl;

// }

