#include "Sphere.h"
#include <iostream>
using namespace std;

int main() {
    Sphere unitSphere; // default radius of 1.0
    cout << "This is a default sphere" << endl 
    << unitSphere << endl;

    Sphere mySphere(3.5); // creates a sphere with radius 3.5
    cout << "This is demonstrating available accessor functions" << endl
    << "This is getRadius " << mySphere.getRadius() << endl
    << "This is getDiameter " << mySphere.getDiameter() << endl
    << "This is getCircumference " << mySphere.getCircumference() << endl
    << "This is getArea " << mySphere.getArea() << endl
    << "This is getVolume " << mySphere.getVolume() << endl
    << "This is getSurfaceArea " << mySphere.getSurfaceArea() << endl << endl;

    cout << "This is another way to print out a sphere " << endl;
    mySphere.setRadius(2.0).display(cout);
    cout << endl;    

    cout << "This is demonstrating addition " << endl; 
    mySphere + 1.0;
    cout << mySphere.getRadius() << endl;

    cout << "This is demonstrating subtraction " << endl;
    mySphere - 0.5;
    cout << mySphere.getRadius() << endl;


    cout << "This is demonstrating multiplication " << endl;
    mySphere * 1.5;
    cout << mySphere.getRadius() << endl;

    
    cout << "This is demonstrating multiple operations " << endl; 
    (unitSphere * 0.25) + .25;
    cout << mySphere.getRadius() << endl;

    cout << "all of these operators are applied to the sphere's radius" << endl << endl;

    cout << "This is demonstrating possible errors " << endl;
    cout << "this expression would result in a negative radius" << endl;
    mySphere - 10;
    cout << "^Error Message above, radius will remain unchanged " << endl;
    
    Sphere testSphere(-1.0);
    cout << "This sphere had a negative input parameter," 
    << " automatically set radius to 1.0" << endl;
    cout << testSphere << endl;

    return 0;
}