#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include <glm/gtc/type_ptr.hpp>

namespace RoadGL
{

enum class CameraDirection
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
private:
    glm::vec3 _position;
    glm::vec3 _front;
    glm::vec3 _world_up;
    glm::vec3 _right;

    float _speed;
    float _sens;
    
    void UpdateCameraVectors ();

public:
    Camera (
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        float speed        = 2.5f,
        float sens         = 0.1f,
        glm::vec3 front    = glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f));
    ~Camera ();

    glm::mat4 GetViewMatrix ();
    glm::vec3 GetPos        () { return _position; }
    glm::vec3 GetDir        () { return _front ;}

    void Move (CameraDirection camera_dir, float delta_time);
    void Look (float offset_x, float offset_y);
};

}

#endif // __CAMERA_HPP__
