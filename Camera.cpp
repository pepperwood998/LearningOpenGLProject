#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

static float pitch = 0.0f;
static float yaw = -90.0f;

Camera::Camera(glm::vec3 position, float speed, float sens, glm::vec3 front, glm::vec3 world_up)
: _position(position), _speed(speed), _sens(sens), _front(front), _world_up(world_up)
{
    this->UpdateCameraVectors();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
    glm::mat4 view = glm::lookAt(_position, _position + _front, _world_up);
    return view;
}

void Camera::UpdateCameraVectors()
{
    glm::vec3 front = glm::vec3(0.0f);
    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

    _front = glm::normalize(front);
    _right = glm::normalize(glm::cross(_front, _world_up));
}

void Camera::Move(CameraDirection camera_dir)
{
    switch (camera_dir)
    {
        case CameraDirection::FORWARD:
            _position += _front * _speed;
            break;
        case CameraDirection::BACKWARD:
            _position -= _front * _speed;
            break;
        case CameraDirection::LEFT:
            _position -= _right * _speed;
            break;
        case CameraDirection::RIGHT:
            _position += _right * _speed;
            break;
        default:
            break;
    }
}

void Camera::Look(float offset_x, float offset_y)
{
    offset_x *= _sens;
    offset_y *= _sens;

    pitch += offset_y;
    yaw += offset_x;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    } else if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    this->UpdateCameraVectors();
}
