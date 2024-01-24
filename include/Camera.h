#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

enum DIRECTIONS { F, B, L, R, U, D };

const float YAW = -90.f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.05f;

class Camera {
  public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 global_up;

    float yaw;
    float pitch;
    float speed;
    float sensitivity;

    Camera(glm::vec3 position = glm::vec3(0.0, 0.0, 0.0), glm::vec3 up = glm::vec3(0.0, 1.0, 0.0), float yaw = YAW,
           float pitch = PITCH)
        : front(glm::vec3(0.0, 0.0, -1.0)), speed(SPEED), sensitivity(SENSITIVITY) {
        position = position;
        global_up = up;
        yaw = yaw;
        pitch = pitch;
        update();
    }

    glm::mat4 get_view_matrix() {
        return glm::lookAt(position, position + front, up);
    }

    void process_keyboard(DIRECTIONS direction, float delta_time) {
        float velocity = speed * delta_time;

        if (direction == F) {
            position += front * velocity;
        }

        if (direction == B) {
            position -= front * velocity;
        }

        if (direction == L) {
            position -= right * velocity;
        }

        if (direction == R) {
            position += right * velocity;
        }

        if (direction == U) {
            position += up * velocity;
        }

        if (direction == D) {
            position -= up * velocity;
        }
    }

    void process_mouse(float x_offset, float y_offset) {
        x_offset *= sensitivity;
        y_offset *= sensitivity;

        yaw += x_offset;
        pitch = glm::clamp(pitch + y_offset, -89.0f, 89.0f);

        update();
    }

  private:
    void update() {
        glm::vec3 view_direction;

        view_direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        view_direction.y = sin(glm::radians(pitch));
        view_direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        front = glm::normalize(view_direction);
        right = glm::normalize(glm::cross(front, global_up));
        up = glm::normalize(glm::cross(right, front));
    }
};
#endif
