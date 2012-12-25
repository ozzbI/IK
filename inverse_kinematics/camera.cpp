#include "camera.h"
Camera::Camera()
{
    _cameraType = AIRCRAFT;

     scene_view=false;

    _pos   = Vector3d(0.0f, 0.0f, 0.0f);
    _right = Vector3d(1.0f, 0.0f, 0.0f);
    _up    = Vector3d(0.0f, 1.0f, 0.0f);
    _look  = Vector3d(0.0f, 0.0f, 1.0f);
}

Camera::Camera(CameraType cameraType)
{
    _cameraType = cameraType;

     scene_view=false;

    _pos   = Vector3d(0.0f, 0.0f, 0.0f);
    _right = Vector3d(1.0f, 0.0f, 0.0f);
    _up    = Vector3d(0.0f, 1.0f, 0.0f);
    _look  = Vector3d(0.0f, 0.0f, 1.0f);
}
Camera::~Camera()
{
}

void Camera::getPosition(Vector3d* pos)
{
    *pos = _pos;
}

void Camera::setPosition(Vector3d* pos)
{
    _pos = *pos;
}

void Camera::getRight(Vector3d* right)
{
    *right = _right;
}

void Camera::getUp(Vector3d* up)
{
    *up = _up;
}

void Camera::getLook(Vector3d* look)
{
    *look = -_look;
}

void Camera::walk(double units)
{
    // move only on xz plane for land object
    if( _cameraType == LANDOBJECT )
        _pos -= Vector3d(_look.x(), 0.0f, _look.z()) * units;

    if( _cameraType == AIRCRAFT )
        _pos -= _look * units;
}

void Camera::strafe(double units)
{
    // move only on xz plane for land object
    if( _cameraType == LANDOBJECT )
        _pos += Vector3d(_right.x(), 0.0f, _right.z()) * units;

    if( _cameraType == AIRCRAFT )
        _pos += _right * units;
}

void Camera::fly(double units)
{
    // move only on y-axis for land object
    if( _cameraType == LANDOBJECT )
        _pos(1) += units;

    if( _cameraType == AIRCRAFT )
        _pos += _up * units;
}

void Camera::pitch(double angle)
{

    Matrix3d T;
    T=AngleAxisd(angle,_right);
    _up=T*_up;
    _look=T*_look;
}

void Camera::yaw(double angle)
{
    Matrix3d T;

    // rotate around world y (0, 1, 0) always for land object
    //if( _cameraType == LANDOBJECT )
        T=AngleAxisd(angle,Vector3d(0,1,0));
    // rotate around own up vector for aircraft
   // if( _cameraType == AIRCRAFT )
        //T=AngleAxisd(angle,_up);
    // rotate _right and _look around _up or y-axis

    _right=T*_right;
    _look=T*_look;
}

void Camera::roll(double angle)
{
    // only roll for aircraft type
    if( _cameraType == AIRCRAFT )
    {
        Matrix3d T;
         T=AngleAxisd(angle,_look);
        // rotate _up and _right around _look vector
        _right=T*_right;
        _up=T*_up;
    }
}

void Camera::getViewMatrix(QMatrix4x4* V)
{
    // Keep camera's axes orthogonal to eachother
    _look.normalize();

    _up=_look.cross(_right);
    _up.normalize();

    _right=_up.cross(_look);
    _right.normalize();

    double x=-_right.dot(_pos);
    double y=-_up.dot(_pos);
    double z=-_look.dot(_pos);

    (*V)(0,0) = _right.x(); (*V)(0, 1) = _up.x(); (*V)(0, 2) = _look.x(); (*V)(0, 3) = 0.0f;
    (*V)(1,0) = _right.y(); (*V)(1, 1) = _up.y(); (*V)(1, 2) = _look.y(); (*V)(1, 3) = 0.0f;
    (*V)(2,0) = _right.z(); (*V)(2, 1) = _up.z(); (*V)(2, 2) = _look.z(); (*V)(2, 3) = 0.0f;
    (*V)(3,0) = x;          (*V)(3, 1) = y;       (*V)(3, 2) = z;         (*V)(3, 3) = 1.0f;

}

void Camera::setCameraType(CameraType cameraType)
{
    _cameraType = cameraType;
}

void Camera::enable_scene_cam(bool state)
{
    angle_1=0;angle_2=0;R=20;
    scene_view=state;

    if(state)
    {
    Quaterniond q1(AngleAxisd(angle_1,Vector3d(1,0,0))),q2(AngleAxisd(angle_2,Vector3d(0,1,0)));
    q1=q2*q1;

    Matrix3d T;
     T=q1.toRotationMatrix();
     _up=T*Vector3d(0,1,0);
    _look=T*Vector3d(0,0,-1);
    _right=_up.cross(_look);
    _right.normalize();

    _pos=_look*R;
    }
}

void Camera::rot_cam(double a1,double a2,double add_R)
{
    angle_1+=a1;angle_2+=a2;R+=add_R;
    Quaterniond q1(AngleAxisd(angle_1,Vector3d(1,0,0))),q2(AngleAxisd(angle_2,Vector3d(0,1,0)));
    q1=q2*q1;

    Matrix3d T;
     T=q1.toRotationMatrix();
     _up=T*Vector3d(0,1,0);
    _look=T*Vector3d(0,0,-1);
    _right=_up.cross(_look);
    _right.normalize();

    _pos=_look*R;

}
