#include <iostream>
using namespace std;

#ifndef SPHERE
#define SPHERE

class Sphere
{
public:
    /* CONSTRUCTORS */
    // default constructor
    Sphere(); 
    // explicit value constructor
    Sphere(double newRadius); 

    /* ACCESSOR FUNCTIONS */
    // returns radius
    double getRadius() const; 
    // returns diameter
    double getDiameter() const; 
    // returns circumference
    double getCircumference() const;
    // returns area of a circle with sphere's radius 
    double getArea() const; 
    // returns sphere volume
    double getVolume() const; 
    // returns sphere surface area
    double getSurfaceArea() const; 
    

    /* MUTATOR FUNCTIONS */
    // changes radius, only non-negative radii
    Sphere& setRadius(double newRadius);

    /* DISPLAY FUNCTIONS*/
    // helper function for <<
    void display(ostream& out) const; 

private:
    /* DATA MEMBERS */
    double Radius;
};

// returns ostream&, can chain
ostream& operator<<(ostream& j, const Sphere& s);
// adds to sphere radius, won't allow result to be negative
Sphere& operator+(Sphere& s, double rhs);
// subtracts from sphere radius, won't allow result to be negative
Sphere& operator-(Sphere& s, double rhs);
// multiplies sphere radius, won't allow result to be negative
Sphere& operator*(Sphere& s, double rhs);

#endif