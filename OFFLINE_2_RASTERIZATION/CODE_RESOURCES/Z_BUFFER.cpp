#include "MVP_TRANSFORMATION.cpp"
#include "bitmap_image.hpp"


class color{

    public:

        int r,g,b;

        color(){

            r = 0;
            g = 0;
            b = 0;

        }

        color(int r,int g,int b){

            this -> r = r;
            this -> g = g;
            this -> b = b;
        
        }

};

static unsigned long int g_seed = 1; 
inline int random()
{
    g_seed = (214013 * g_seed + 2531011); 
    return (g_seed >> 16) & 0x7FFF;
}

color drawColor(){

    int r = random()%255;
    int g = random()%255;
    int b = random()%255;

    return color(r,g,b);

}



class z_buf{

    public:

        double** zbuffer;
        double screen_width;
        double screen_height;

        int left = -1, right = 1, top = 1, bottom = -1;

        int ht = right - left, vt = top - bottom;

        double dx,dy, Top_Y, Left_X;

        int z_min,z_max;

        bitmap_image image;

        z_buf(){}

        z_buf(double screen_width, double screen_height){

            this -> screen_height = screen_height;
            this -> screen_width = screen_width;

            
            

            dx = ht/screen_width,dy = vt/screen_height;

            Top_Y = top - dy/2,Left_X = left + dx/2;

            z_min = -1, z_max = 1;

            zbuffer = new double* [(int)screen_width]; 

            for(int i = 0; i < screen_width; i++){

                zbuffer[i] = new double[(int)screen_height];

                for(int j = 0; j < screen_height; j++){

                    zbuffer[i][j] = z_max;

                }

            }

            image.setwidth_height(screen_width, screen_height);

        }

        ~z_buf(){

            for(int i = 0; i < screen_width; i++){

                delete[] zbuffer[i];

            }

            delete[] zbuffer;

        
        }

        /*--------------------- The z-Buffer Algorithm --------------------------*/

        void procedure(){

            for(vector<Triangle>::iterator t = TriangleSet.begin(); t != TriangleSet.end(); t++){

                color c = drawColor();

                double y_max = max3( t -> vtx[0].getY() , t -> vtx[1].getY(), t -> vtx[2].getY() ); 

                int top_scanline = max( 0, (int)round( ( Top_Y - y_max ) / dy ) ); 

                double y_min = min3( t -> vtx[0].getY() , t -> vtx[1].getY(), t -> vtx[2].getY() );

                int bottom_scanline = min( (int)( screen_height - 1 ), (int)round( ( Top_Y - y_min ) / dy ) ); 

                for( int row_no = top_scanline; row_no <= bottom_scanline; row_no++ ){

                    double scanline_y = Top_Y - row_no*dy; 

                    double Right_X = right - dx/2;

                    double x_max = INT_MIN , x_min = INT_MAX, z_left, z_right; 

                    for(int i =0 ; i< 3; i++){

                        vector3 a = t -> vtx[i];
                        vector3 b = t -> vtx[(i+1)%3];

                        if(a.getY() == b.getY() && a.getY() == scanline_y){

                            if( a.getX() < b.getX() ){

                                x_max = max( x_max, b.getX() );
                                if( x_max == b.getX() ) z_right = b.getZ();
                                x_min = min( x_min, a.getX() );
                                if( x_min == a.getX() ) z_left  = a.getZ(); 

                            }

                            else{

                                x_max = max( x_max, a.getX() );
                                if( x_max == a.getX() ) z_right = a.getZ();
                                x_min = min( x_min, b.getX() );
                                if( x_min == b.getX() ) z_left  = b.getZ(); 

                            }

                        }

                        

                        else if( a.getY() != b.getY() ){

                            double x ;

                            if( a.getX() == b.getX() ) {

                                x = a.getX();
                                
                            }
                            else x = a.getX() + ( scanline_y - a.getY() ) / slopeXY(a,b); 

                            if( x <= max( a.getX(), b.getX() ) && x >= min( a.getX(), b.getX() ) ){

                                x_min = min( x, x_min ); 
                                x_max = max( x, x_max ); 
                                if( x == x_min ) z_left  = a.getZ() + ( scanline_y - a.getY() ) * slopeYZ(a,b);
                                if( x == x_max ) z_right = a.getZ() + ( scanline_y - a.getY() ) * slopeYZ(a,b);

                            }



                        }

                        

                    }

                    if( x_min == INT_MAX || x_max == INT_MIN ) continue; 

                    int left_intersecting_column = (int)round( ( max( Left_X, x_min ) - Left_X ) / dx );
                    int right_intersecting_column = screen_width - (int)round( (Right_X - min(Right_X, x_max)) / dx ); 

                    for ( int col_no = left_intersecting_column; col_no < right_intersecting_column; col_no++ ){

                        double scanline_x = Left_X + col_no* dx; 

                        double z = z_left + (scanline_x - x_min)* (z_right - z_left) / (x_max - x_min); 

                        if( z >= z_min && z <= z_max && z < zbuffer[row_no][col_no] ){ 

                            zbuffer[row_no][col_no] = z; 
                             
                            image.set_pixel(col_no, row_no, c.r, c.g, c.b);

                        }

                    }

                }

            }

            image.save_image("out.bmp");
        

        }

        void save_zbuffer(){


            ofstream fout("z-buffer.txt");
            fout << fixed << setprecision(6);

            for (int i = 0; i < screen_height; i++) {

                for (int j = 0; j < screen_width; j++) {

                    if (zbuffer[i][j] < z_max) fout << zbuffer[i][j] << "\t";

                }

                fout << endl;
            }

            fout.close();

        }



};



void Z_buffer(){


    ifstream config("config.txt");
    

    double screen_width,screen_height;

    config >> screen_width >> screen_height;
      

    z_buf zbuf = z_buf(screen_width,screen_height);

    zbuf.procedure(); 

    zbuf.save_zbuffer();

    config.close();
    


}
