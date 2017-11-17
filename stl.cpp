#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
using namespace std;

const long double pi = 3.14159265359;
int count = 0;
typedef struct {
  double x;
  double y;
  double z;
} point_t;
typedef struct {
  point_t one;
  point_t two;
  point_t three;
} triangle_t;

point_t getNorm(triangle_t tri, double size){
  point_t norm;
  point_t U; // needs to be points, not triangles;
  point_t V;
  U.x = (tri.two.x - tri.one.x);
  U.y = (tri.two.y - tri.one.y);
  U.z = (tri.two.z - tri.one.z); // needs to be points, not triangles;
  V.x = (tri.three.x - tri.one.x);
  V.y = (tri.three.y - tri.one.y);
  V.z = (tri.three.z - tri.one.z);

  norm.x = ((U.y*V.z)-(U.z*V.y));
  norm.y = ((U.z*V.x)-(U.x*V.z));
  norm.z = ((U.x*V.y)-(U.y*V.x));
  norm.x = norm.x/size;
  norm.y = norm.y/size;
  norm.z = norm.z/size;


  return norm;
}
triangle_t makeTri(point_t a, point_t b, point_t c){
  triangle_t triangle;
  triangle.one = a;
  triangle.two = b;
  triangle.three = c;
  return triangle;
}

void movePoint(point_t& b, double x, double y, double r, double theta, double& thetaCount){

  b.x = x + r*(cos(thetaCount));
  b.y = y + r*(sin(thetaCount));
  thetaCount += theta;
}
  class Shape {
    private:
      double x,y,z;

    public:
      Shape(double x, double y, double z) : x(x), y(y), z(z) {count++;}
      virtual void write(string myfile) const = 0;


    };



  class Cylinder : public Shape{
    private:
    	double r, h;
    	int facets;
      std::vector<triangle_t> triangle;
    public:
    	Cylinder(double x, double y, double z, double r, double h, int facets) : Shape(x, y, z), r(r), h(h)  {
        double theta = (double(2*pi))/(double(facets));
        double thetaCount = (double(2*pi))/(double(facets));
        point_t origin1 = {x,y,0};
        point_t origin2 = {x,y,h};
        point_t start = {x+r,0,0};
        point_t start2 = {x+r,0,h};
        point_t temp;
        point_t temp2;
        temp.x = start.x;
        temp.y = start.y;
        temp.z = start.z;
        temp2.x = start2.x;
        temp2.y = start2.y;
        temp2.z = start2.z;


        for(int i = 0; i < facets; i++){
          movePoint(temp, x, y, r, theta, thetaCount);
          triangle.push_back(makeTri(origin1,temp,start));
          triangle.push_back(makeTri(start,start2,temp));

          start.x = temp.x;
          start.y = temp.y;
          temp2.x = temp.x;
          temp2.y = temp.y;

          triangle.push_back(makeTri(origin2,temp2,start2));
          triangle.push_back(makeTri(start2,temp,temp2));
          start2.x = temp2.x;
          start2.y = temp2.y;
        }
      }
      void write(string myfile) const {
        ofstream f(myfile, ios::app);
        f << "solid cylinder" << count << endl << "\t";
        for(int i = 0; i<triangle.size(); i++){
          f << "facet normal " << getNorm(triangle[i], pow(r,2.0)).x << " " << getNorm(triangle[i],pow(r,2.0)).y << " " << getNorm(triangle[i],pow(r,2.0)).z << endl << "\t" << "outer loop" << endl;
          f << "vertex " << triangle[i].one.x << " " << triangle[i].one.y << " " << triangle[i].one.z << endl;
          f << "vertex " << triangle[i].two.x << " " << triangle[i].two.y << " " << triangle[i].two.z << endl;
          f << "vertex " << triangle[i].three.x << " " << triangle[i].three.y << " " << triangle[i].three.z << endl;
          f << "endloop" << endl;
          f << "end facet" << endl;
          }
        f << "end solid" << endl;

      }
    };

  class Cube: public Shape {
    private:
    	double size;
      std::vector<triangle_t> triangle;

    public:
    	Cube(double x, double y, double z, double size) : Shape(x,y,z), size(size) {

        point_t c0 = {x, y, z};
        point_t c1 = {x + size, y, z};
        point_t c2 = {x, y + size, z};
        point_t c3 = {x, y, z + size};
        point_t c4 = {x + size, y + size, z};
        point_t c5 = {x + size,y , z + size};
        point_t c6 = {x, y + size, z + size};
        point_t c7 = {x + size, y + size, z + size};

        triangle.push_back(makeTri(c6,c5,c7));
        triangle.push_back(makeTri(c5,c6,c3));
        triangle.push_back(makeTri(c0,c4,c1));
        triangle.push_back(makeTri(c4,c0,c2));
        triangle.push_back(makeTri(c0,c5,c3));
        triangle.push_back(makeTri(c5,c0,c1));
        triangle.push_back(makeTri(c5,c4,c7));
        triangle.push_back(makeTri(c4,c5,c1));
        triangle.push_back(makeTri(c4,c6,c7));
        triangle.push_back(makeTri(c6,c4,c2));
        triangle.push_back(makeTri(c0,c6,c2));
        triangle.push_back(makeTri(c6,c0,c3));


      }

      void write(string myfile) const {
        ofstream f(myfile, ios::app);
        f << "solid cube" << count << endl << "\t";
        for(int i = 0; i<12; i++){
          f << "facet normal " << getNorm(triangle[i], pow(size,2.0)).x << " " << getNorm(triangle[i],pow(size,2.0)).y << " " << getNorm(triangle[i],pow(size,2.0)).z << endl << "\t" << "outer loop" << endl;
          f << "vertex " << triangle[i].one.x << " " << triangle[i].one.y << " " << triangle[i].one.z << endl;
          f << "vertex " << triangle[i].two.x << " " << triangle[i].two.y << " " << triangle[i].two.z << endl;
          f << "vertex " << triangle[i].three.x << " " << triangle[i].three.y << " " << triangle[i].three.z << endl;
          f << "endloop" << endl;
          f << "end facet" << endl;
          }
        f << "end solid" << endl;

      }



    };


  class CAD {
    private:
    	vector<Shape*> shapes;

    public:
      CAD(){}
      ~CAD(){
        for(int i = 0; i < shapes.size(); i++){
          delete shapes[i];
        }
      }
      void add(Shape* shape) {

        shapes.push_back(shape);
      }
      void write(string file){
        for(int i = 0; i < shapes.size(); i++){
          shapes[i]->write(file);

        }
      }

};

  //https://www.stratasysdirect.com/resources/how-to-prepare-stl-files/
  //https://www.viewstl.com/
  int main() {
  	CAD c;
  	c.add(new Cube(0,0,0,5));
  	c.add(new Cylinder(100,0,0,3, 10, 10));
    c.write("test.stl");

  }
