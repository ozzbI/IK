#ifndef UTILS_H
#define UTILS_H

#include<C:\Qt_projects\Eigen\Dense>

Vector3d projection(const Vector3d& a,  const Vector3d& b)//b на a
{
    Vector3d p = a*( a.dot(b)/(a.x()*a.x()+a.y()*a.y()+a.z()*a.z()) ) ;
    return  p;
}

Vector3d reflect(const Vector3d& I,  const Vector3d& N)
{
    return I - 2.0 * N.dot(I) * N;
}



#endif // UTILS_H
