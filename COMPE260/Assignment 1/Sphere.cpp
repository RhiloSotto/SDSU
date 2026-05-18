#include <iostream>

#include "Sphere.h"
#define PI 3.1415926535897932384 
// Pi up to 20 digits
using namespace std;


// default constructor with radius 1.0
Sphere::Sphere() {
    Radius = 1.0;
}

// explicit value constructor
// returns error message for negative radii
Sphere::Sphere(double newRadius) {
    if (newRadius >= 0)
        Radius = newRadius;
    else {
        cerr << "Radius must be non-negative, radius set to 1.0" << endl;
        Radius = 1.0;
    }
}

// returns radius
double Sphere::getRadius() const{
    return Radius;
}

// returns diameter
double Sphere::getDiameter() const{
    return 2*Radius;
}

// returns circumference
double Sphere::getCircumference() const{
    return 2*PI*Radius;
}

// returns area of a circle with sphere's radius
double Sphere::getArea() const{
    return PI*(Radius*Radius);
}

// returns sphere volume   
double Sphere::getVolume() const{
    return (4/3.0)*PI*(Radius*Radius*Radius);
}

// returns sphere surface area
double Sphere::getSurfaceArea() const{
    return 4*PI*Radius*Radius;
}

// changes radius, only non-negative radii
Sphere& Sphere::setRadius(double newRadius) {
    if (newRadius >= 0)
        Radius = newRadius;
    else
        cerr << "Radius must be non-negative, radius still " << Radius << endl;
    return *this;
}

// helper function for <<
void Sphere::display(ostream& out) const{
    out << "Radius: " << getRadius() << endl
    << "Diameter: " << getDiameter() << endl
    << "Circumference: " << getCircumference() << endl
    << "Area: " << getArea() << endl
    << "Volume: " << getVolume() << endl
    << "Surface Area: " << getSurfaceArea() << endl;
}

// overloaded << operator
ostream& operator<<(ostream& j, const Sphere& s) {
  s.display(j);
  return j;
}

// overloaded + operator
Sphere& operator+(Sphere& s, double rhs) {
    if (s.getRadius() + rhs >= 0)
        s.setRadius(s.getRadius() + rhs);
    else 
        cerr << "Radius must be non-negative, radius still " << s.getRadius() << endl;
    return s;
}

// overloaded - operator
Sphere& operator-(Sphere& s, double rhs) {
    if (s.getRadius() - rhs >= 0) 
       s.setRadius(s.getRadius() - rhs);
    else 
       cerr << "Radius must be non-negative, radius still " << s.getRadius() << endl;
    return s;  
}

// overloaded * operator
Sphere& operator*(Sphere& s, double rhs) {
    if (rhs > 0)
        s.setRadius(s.getRadius() * rhs);
    else 
        cerr << "Radius must be non-negative, radius still " << s.getRadius() << endl;
    return s;
}