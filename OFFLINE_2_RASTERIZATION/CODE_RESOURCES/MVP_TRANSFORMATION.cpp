#include "BASICS.cpp"
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <stack>

stack <matrix> S;
matrix M, Tt, Ts, Tr, I, Vt, Vr, V, Proj;
vector3 eye, look, up, l, r, u;
double fovY, aspectRatio, near, far;

vector3 getVector3(string line){

    double a, b, c;

    stringstream stream(line);

    stream >> a >> b >> c;

    return vector3(a,b,c);


}

void generateStage(){

    vector3 dump_vct = I.vct[3];

    matrix P ;

    vector3 vct[3];

    ifstream fin ("scene.txt");

    ofstream out1("stage1.txt");

    ofstream out2("stage2.txt");

    ofstream out3("stage3.txt");

    out1 << fixed << setprecision(7);

    out2 << fixed << setprecision(7);

    out3 << fixed << setprecision(7);

    string line;

    getline(fin,line);

    eye = getVector3(line);

    getline(fin,line);

    look = getVector3(line);

    getline(fin,line);

    up = getVector3(line);

    getline(fin,line);

    stringstream stream(line);

    stream >> fovY >> aspectRatio >> near >> far;

    /*--------------------- For View Transformation --------------------------*/

    l = look - eye;

    l = normalize(l);

    r = cross(l,up);

    r = normalize(r);

    u = cross(r,l);

    Vt.vct[3].setX(-eye.getX());
    Vt.vct[3].setY(-eye.getY());
    Vt.vct[3].setZ(-eye.getZ());

    Vr.vct[0] = vector3( r.getX(), u.getX(), -l.getX(),0 );
    Vr.vct[1] = vector3( r.getY(), u.getY(), -l.getY(),0 );
    Vr.vct[2] = vector3( r.getZ(), u.getZ(), -l.getZ(),0 );

    V = Vr* Vt;

    /*--------------------- For Projection Transformation --------------------------*/

    double fovX = fovY * aspectRatio ;

    fovX = M_PI* fovX/180;
    fovY = M_PI* fovY/180;

    double pt = near * tan(fovY/2) ;
    double pr = near * tan(fovX/2) ;

    Proj.vct[0].setX(near/pr);
    Proj.vct[1].setY(near/pt);

    double temp = near - far;
    
    Proj.vct[2].setZ( (far + near)/temp );
    Proj.vct[3].setZ( (2 * far * near)/temp );

    Proj.vct[2].setT( -1 );
    Proj.vct[3].setT( 0 );

    /*--------------------- For Command Reading and Execution (Generation of Stages)--------------------------*/

    while(true){

        
        getline(fin,line);

        if(line == "triangle"){

            for(int i=0; i<3 ;i++){

                getline(fin,line);
                
                vct[i] = getVector3(line);
            }

            P = matrix(vct[0],vct[1],vct[2],dump_vct);

            P = M*P;

            P.scale();

            out1 << P;

            P = V*P;

            P.scale();

            out2 << P;

            P = Proj*P;

            P.scale();

            out3 << P;

            TriangleSet.push_back(Triangle(P));
            

        }

        if(line == "translate"){

            double tx,ty,tz;

            getline(fin,line);

            stringstream stream(line);

            stream >> tx >> ty >> tz ;

            Tt.vct[3].setX(tx);
            Tt.vct[3].setY(ty);
            Tt.vct[3].setZ(tz);

            M = M * Tt;

            
        }

        if(line == "scale"){

            double sx,sy,sz;

            getline(fin,line);

            stringstream stream(line);

            stream >> sx >> sy >> sz ;

            Ts.vct[0].setX(sx);
            Ts.vct[1].setY(sy);
            Ts.vct[2].setZ(sz);

            M = M * Ts;

            
        }

        if(line == "rotate"){

            double angle,ax,ay,az;

            getline(fin,line);

            stringstream stream(line);

            stream >> angle >> ax >> ay >> az ;

            vector3 a = vector3(ax,ay,az) ;

            a = normalize(a);

            for(int i=0; i<3; i++){

                I.vct[i];
                vct[i] = rodriguez(I.vct[i],a,angle);
                vct[i].setT(0);

            }

            Tr = matrix(vct[0],vct[1],vct[2],dump_vct);

            M = M * Tr;
            
        }

        if(line == "push"){

            S.push(M);

            
        }

        if(line == "pop"){

            M = S.top();
            S.pop();

            
        }

        if(line == "end"){

            break;

        }

    }

    
    fin.close();
    out1.close();
    out2.close();
    out3.close();
    
}

