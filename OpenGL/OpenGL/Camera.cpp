#include "Camera.hpp"

namespace gps {

    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = glm::normalize(cameraUp);
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraFrontDirection, this->cameraUpDirection));
    }

    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    void Camera::setPosition(const glm::vec3& position) {
        cameraPosition = position;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }

    void Camera::setTarget(const glm::vec3& target) {
        cameraTarget = target;
        cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection));
    }

    void Camera::move(MOVE_DIRECTION direction, float speed) {
        if (direction == MOVE_FORWARD) {
            cameraPosition += speed * cameraFrontDirection;
        }
        if (direction == MOVE_BACKWARD) {
            cameraPosition -= speed * cameraFrontDirection;
        }
        if (direction == MOVE_LEFT) {
            cameraPosition -= glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection)) * speed;
        }
        if (direction == MOVE_RIGHT) {
            cameraPosition += glm::normalize(glm::cross(cameraFrontDirection, cameraUpDirection)) * speed;
        }
        if (direction == MOVE_UP) {
            cameraPosition += speed * cameraUpDirection;
        }
        if (direction == MOVE_DOWN) {
            cameraPosition -= speed * cameraUpDirection;
        }

        cameraTarget = cameraPosition + cameraFrontDirection;
    }

    void Camera::rotate(float pitch, float yaw) {
        if (pitch > 89.0f) {
            pitch = 89.0f;
        }
        if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        cameraFrontDirection = glm::normalize(front);
        cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
        cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }

    glm::vec3 Camera::getPosition() const {
        return cameraPosition;
    }



    glm::vec3 Camera::getTarget() const {
        return cameraTarget;
    }

    void Camera::adjustPitch(float deltaPitch) {
        pitch += deltaPitch;
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        rotate(pitch, yaw);
    }
    void Camera::adjustYaw(float deltaYaw) {
        yaw += deltaYaw;

        rotate(pitch, yaw);
    }

}