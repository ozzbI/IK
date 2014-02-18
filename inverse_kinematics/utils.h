#ifndef UTILS_H
#define UTILS_H

#include<C:\Qt_projects\Eigen\Dense>

using namespace Eigen;

Eigen::Vector3d projection(const Vector3d& a,  const Vector3d& b);

Eigen::Vector3d reflect(const Vector3d& I,  const Vector3d& N);

#endif // UTILS_H
