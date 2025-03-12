#ifndef Camera_hpp
#define Camera_hpp
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <string>

namespace gps {

    enum MOVE_DIRECTION { MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN };

    class Camera {
    public:
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);

        glm::mat4 getViewMatrix();
        void move(MOVE_DIRECTION direction, float speed);
        void rotate(float pitch, float yaw);
        void setPosition(const glm::vec3& position);
        void setTarget(const glm::vec3& target);
        void adjustPitch(float deltaPitch);
        void adjustYaw(float deltaYaw);
        glm::vec3 getPosition() const;
        glm::vec3 getTarget() const;

    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;

        float pitch = 0.0f;
        float yaw = -90.0f;
    };

}

#endif /* Camera_hpp */
