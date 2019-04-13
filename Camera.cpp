#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

static float pitch = 0.0f;
static float yaw   = -90.0f;

RoadGL::Camera::Camera (glm::vec3 position, float speed, float sens, glm::vec3 front, glm::vec3 world_up)
: _position(position), _speed(speed), _sens(sens), _front(front), _world_up(world_up)
{
    this->UpdateCameraVectors();
}

RoadGL::Camera::~Camera ()
{
}

glm::mat4 RoadGL::Camera::GetViewMatrix ()
{
    glm::mat4 view = glm::lookAt(_position, _position + _front, _world_up);
    return view;
}

void RoadGL::Camera::UpdateCameraVectors ()
{
    glm::vec3 front = glm::vec3(0.0f);
    
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _world_up));
}

void RoadGL::Camera::Move (CameraDirection camera_dir, float delta_time)
{
    float frame_speed = _speed * delta_time;

    switch (camera_dir)
    {
        case CameraDirection::FORWARD:
            _position += _front * frame_speed;
            break;
        case CameraDirection::BACKWARD:
            _position -= _front * frame_speed;
            break;
        case CameraDirection::LEFT:
            _position -= _right * frame_speed;
            break;
        case CameraDirection::RIGHT:
            _position += _right * frame_speed;
            break;
        default:
            break;
    }
}

void RoadGL::Camera::Look (float offset_x, float offset_y)
{
    offset_x *= _sens;
    offset_y *= _sens;

    pitch += offset_y;
    yaw   += offset_x;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    } else if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    this->UpdateCameraVectors();
}
