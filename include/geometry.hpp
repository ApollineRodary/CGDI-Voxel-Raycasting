/**
 * @file geometry.hpp
 */
#ifndef __RAYCAST_GEOMETRY__
#define __RAYCAST_GEOMETRY__

#include <array>
#include <vector>
#include <cmath>
#include <iostream>

using std::numbers::pi;

using Vertex = std::array<double,3>;
using Face = std::vector<size_t>;
using Matrix4d = std::array<std::array<double,4>,4>;


/**
 * Struct used to store information about a 3D point in a scene.
 * It contains lots of methods useful for 3D operations in geometry.
 */
struct Point {
private:
    // Attributes
    /**
     * Array of 3 double to store the 3D coordinates.
     * @note Storing it as an array is very useful for polyscope since we only have to make vectors of Point objects and it is directly compatible with polyscope's functions.
     */
    std::array<double,3> xyz;

public:
    // Constructors
    /**
     * Default constructor.
     */
    Point() : xyz({0., 0., 0.}) {}
    /**
     * Basic constructor from 3 coordinates.
     */
    Point(const double& x, const double& y, const double& z) : xyz({x, y, z}) {}
    /**
     * Basic constructor from an array of 3 coordinates directly.
     */
    Point(const std::array<double,3>& arr) : xyz(arr) {}
    /**
     * Constructor from two points to make a "3D vector"
     */
    Point(const Point& a, const Point& b)
        : xyz({b.xyz[0]-a.xyz[0], b.xyz[1]-a.xyz[1], b.xyz[2]-a.xyz[2]}) {}

    // Operators
    /**
     * Sum operation between 2 points.
     */
    inline Point operator+(const Point& p) const {
        return Point(xyz[0]+p.xyz[0], xyz[1]+p.xyz[1], xyz[2]+p.xyz[2]);
    }
    /**
     * Difference operation between 2 points.
     */
    inline Point operator-(const Point& p) const {
        return Point(xyz[0]-p.xyz[0], xyz[1]-p.xyz[1], xyz[2]-p.xyz[2]);
    }
    /**
     * Scaling operation for a Point.
     * @param   d   Scaling factor.
     */
    inline Point operator*(const double& d) const {
        return Point(xyz[0]*d, xyz[1]*d, xyz[2]*d);
    }
    /**
     * Inverse scaling operation for a Point.
     * @param   d   Scaling factor.
     */
    inline Point operator/(const double& d) const {
        return Point(xyz[0]/d, xyz[1]/d, xyz[2]/d);
    }
    /**
     * Sum operation between this point and another one.
     */
    inline Point& operator+=(const Point& p) {
        xyz[0] += p.xyz[0], xyz[1] += p.xyz[1], xyz[2] += p.xyz[2];
        return *this;
    }
    /**
     * Difference operation between this point and another one.
     */
    inline Point& operator-=(const Point& p) {
        xyz[0] -= p.xyz[0], xyz[1] -= p.xyz[1], xyz[2] -= p.xyz[2];
        return *this;
    }
    /**
     * Self scaling operation.
     * @param   d   Scaling factor.
     */
    inline Point& operator*=(const double& d) {
        xyz[0] *= d, xyz[1] *= d, xyz[2] *= d;
        return *this;
    }
    /**
     * Self inverse scaling operation.
     * @param   d   Scaling factor.
     */
    inline Point& operator/=(const double& d) {
        xyz[0] /= d, xyz[1] /= d, xyz[2] /= d;
        return *this;
    }

    // Methods
    /**
     * Getter for the x coordinate of the Point.
     * @return  A copy of that coordinate.
     */
    inline double x() const {
        return xyz[0];
    }
    /**
     * Getter for the x coordinate of the Point.
     * @note    This version returns a reference!
     * @return  A reference to that coordinate.
     */
    inline double& x() {
        return xyz[0];
    }
    /**
     * Getter for the y coordinate of the Point.
     * @return  A copy of that coordinate.
     */
    inline double y() const {
        return xyz[1];
    }
    /**
     * Getter for the y coordinate of the Point.
     * @note    This version returns a reference!
     * @return  A reference to that coordinate.
     */
    inline double& y() {
        return xyz[1];
    }
    /**
     * Getter for the z coordinate of the Point.
     * @return  A copy of that coordinate.
     */
    inline double z() const {
        return xyz[2];
    }
    /**
     * Getter for the z coordinate of the Point.
     * @note    This version returns a reference!
     * @return  A reference to that coordinate.
     */
    inline double& z() {
        return xyz[2];
    }
    /**
     * Array dereference operator.
     * @note    No checks are done on the value of i.
     * @return  A copy of the accessed value.
     */
    inline double operator[](const int i) const {
        return xyz[i];
    }
    /**
     * Array dereference operator.
     * @note    No checks are done on the value of i.
     * @note    This version returns a reference!
     * @return  A reference to the accessed value.
     */
    inline double& operator[](const int i) {
        return xyz[i];
    }

    /**
     * Dot product betwenn two points.
     */
    inline double dot(const Point& p) const {
        return xyz[0]*p.xyz[0] + xyz[1]*p.xyz[1] + xyz[2]*p.xyz[2];
    }
    /**
     * Cross product betwenn two points.
     */
    inline Point cross(const Point& p) const {
        return Point(
            xyz[1]*p.xyz[2] - p.xyz[1]*xyz[2],
            xyz[2]*p.xyz[0] - p.xyz[2]*xyz[0],
            xyz[0]*p.xyz[1] - p.xyz[0]*xyz[1]
        );
    }
    /**
     * Manhattan norm of this point.
     */
    inline double norm1() const {
        return std::abs(xyz[0]) + std::abs(xyz[1]) + std::abs(xyz[2]);
    }
    /**
     * Euclidian norm of this point.
     */
    inline double norm2() const {
        return std::sqrt(xyz[0]*xyz[0] + xyz[1]*xyz[1] + xyz[2]*xyz[2]);
    }
    /**
     * Maximum/Infinity norm of this point.
     */
    inline double normInf() const {
        return std::max(std::abs(xyz[0]), std::max(std::abs(xyz[1]), std::abs(xyz[2])));
    }
    /**
     * Apply a 4x4 affine transformation matrix to this point.
     * @param   m   4x4 matrix stored as an array of arrays.
     * @return  A reference to this point after the transformation.
     */
    inline Point& transform(const Matrix4d& m) {
        double new_x = xyz[0]*m[0][0] + xyz[1]*m[0][1] + xyz[2]*m[0][2] + m[0][3];
        double new_y = xyz[0]*m[1][0] + xyz[1]*m[1][1] + xyz[2]*m[1][2] + m[1][3];
        double new_z = xyz[0]*m[2][0] + xyz[1]*m[2][1] + xyz[2]*m[2][2] + m[2][3];
        xyz[0] = new_x, xyz[1] = new_y, xyz[2] = new_z;
        return *this;
    }

    // FRIEND FUNCTIONS
    friend std::ostream& operator<<(std::ostream& os, const Point& p);
};

/**
 * Stream writing operator for a Point.
 */
std::ostream& operator<<(std::ostream& os, const Point& p);

#endif//__RAYCAST_GEOMETRY__
