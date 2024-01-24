#define GLFW_INCLUDE_NONE
#include <Camera.h>
#include <Colors.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <SoundManager.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

/**
 * @brief
 *
 * @param window
 * @param key
 * @param scancode
 * @param action
 * @param mods
 */
static void key_callback(GLFWwindow *window);

static void mouse_callback(GLFWwindow *window, double x_position, double y_position);

/**
 * @brief Attaches the GLFW window object to the frame buffer.
 *
 * @param window The GLFW window object.
 * @param width The width of the window.
 * @param height The height of the window.
 */
static void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const float WIDTH = 800.0f;
const float HEIGHT = 800.0f;

Camera camera;

float delta_time = 0.0;
float last_frame = 0.0;

float last_x = WIDTH / 2.0f;
float last_y = HEIGHT / 2.0f;

/**
 * @brief The main driver code.
 *
 * This is where we initialize crucial OpenGL components.
 *
 * @return Nothing is returned.
 */
int main(void) {
    GLfloat vertices[] = {// Front
                          -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
                          -0.5f, -0.5f, -0.5f, -0.5f,

                          // Back
                          -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
                          -0.5f, -0.5f, 0.5f,

                          // Left
                          -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,
                          0.5f, -0.5f, 0.5f, 0.5f,

                          // Right
                          0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,
                          0.5f, 0.5f, 0.5f, 0.5f,

                          // Bottom
                          -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, -0.5f, -0.5f,
                          0.5f, -0.5f, -0.5f, -0.5f,

                          // Top
                          -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, 0.5f, 0.5f,
                          -0.5f, 0.5f, -0.5f};

    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "shapegl", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize shader object
    Shader shader("../shaders/vert.glsl", "../shaders/frag.glsl");
    SoundManager sound_manager;

    // Generate vertex buffer
    unsigned int VBO, VAO, CAO;

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &CAO);
    glGenVertexArrays(1, &VAO);

    // Bind the vertex array object - stores state of VAO
    glBindVertexArray(VAO);

    // Bind the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, CAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(COLOR_GOW), COLOR_GOW, GL_STATIC_DRAW);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        // Calculate the frames
        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        key_callback(window);

        // Clear the color buffers
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Restore the state of VBO
        shader.use();

        glm::mat4 m_model = glm::mat4(1.0f);
        glm::mat4 m_view = camera.get_view_matrix();
        glm::mat4 m_projection = glm::perspective(glm::radians(60.0f), WIDTH / HEIGHT, 0.1f, 100.0f);

        shader.set_mat("m_model", m_model);
        shader.set_mat("m_view", m_view);
        shader.set_mat("m_projection", m_projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Clear buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // De-allocate objects and buffers
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &CAO);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}

static void key_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.process_keyboard(FORWARD, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.process_keyboard(BACK, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.process_keyboard(LEFT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.process_keyboard(RIGHT, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.process_keyboard(UP, delta_time);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera.process_keyboard(DOWN, delta_time);
    }
}

static void mouse_callback(GLFWwindow *window, double x_position, double y_position) {
    float x_position_f = static_cast<float>(x_position);
    float y_position_f = static_cast<float>(y_position);

    float x_offset = x_position_f - last_x;
    float y_offset = last_y - y_position_f;

    last_x = x_position_f;
    last_y = y_position_f;

    camera.process_mouse(x_offset, y_offset);
}
static void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

