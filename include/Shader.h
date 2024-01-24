#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"

#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>

/**
 * @class Shader
 * @brief
 *
 */
class Shader {
  public:
    unsigned int program_id;

    /**
     * @brief
     *
     * @param vertex_path
     * @param fragment_path
     */
    Shader(const char *vertex_shader_path, const char *fragment_shader_path) {
        std::string vertex_code;
        std::string fragment_code;
        std::ifstream vertex_shader_file;
        std::ifstream fragment_shader_file;

        vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            vertex_shader_file.open(vertex_shader_path);
            fragment_shader_file.open(fragment_shader_path);

            std::stringstream vertex_shader_stream, fragment_shader_stream;

            vertex_shader_stream << vertex_shader_file.rdbuf();
            fragment_shader_stream << fragment_shader_file.rdbuf();

            vertex_shader_file.close();
            fragment_shader_file.close();

            vertex_code = vertex_shader_stream.str();
            fragment_code = fragment_shader_stream.str();
        } catch (std::ifstream::failure &error) {
            std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << error.what() << std::endl;
        }

        const char *vertex_shader_code = vertex_code.c_str();
        const char *fragment_shader_code = fragment_code.c_str();

        // Compile the shaders
        unsigned int vertex, fragment;

        vertex = glCreateShader(GL_VERTEX_SHADER);
        fragment = glCreateShader(GL_FRAGMENT_SHADER);

        glShaderSource(vertex, 1, &vertex_shader_code, nullptr);
        glShaderSource(fragment, 1, &fragment_shader_code, nullptr);

        glCompileShader(vertex);
        check_compilation_errors(vertex, "VERTEX");

        glCompileShader(fragment);
        check_compilation_errors(fragment, "FRAGMENT");

        program_id = glCreateProgram();

        glAttachShader(program_id, vertex);
        glAttachShader(program_id, fragment);

        glLinkProgram(program_id);
        check_compilation_errors(program_id, "PROGRAM");

        glDetachShader(program_id, vertex);
        glDetachShader(program_id, fragment);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    /**
     * @brief
     */
    void use() {
        glUseProgram(program_id);
    }

    /**
     * @brief
     *
     * @param name
     * @param value
     */
    void set_bool(const std::string &name, bool value) const {
        glUniform1i(get_uniform_location(name), (int)value);
    }
    /**
     * @brief
     *
     * @param name
     * @param value
     */
    void set_int(const std::string &name, int value) const {
        glUniform1i(get_uniform_location(name), value);
    }
    /**
     * @brief
     *
     * @param name
     * @param value
     */
    void set_float(const std::string &name, float value) const {
        glUniform1f(get_uniform_location(name), value);
    }

    void set_mat(const std::string &name, glm::mat4 value) const {
        glUniformMatrix4fv(get_uniform_location(name), 1, GL_FALSE, glm::value_ptr(value));
    }

    int get_uniform_location(const std::string &name) const {
        return glGetUniformLocation(program_id, name.c_str());
    }

  private:
    void check_compilation_errors(unsigned int shader, std::string type) {
        int success;
        char info_log[1024];

        if (type == "PROGRAM") {
            glGetProgramiv(program_id, GL_LINK_STATUS, &success);

            if (success) {
                return;
            }

            glGetProgramInfoLog(program_id, 1024, nullptr, info_log);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << type << "\n" << info_log << std::endl;

        } else {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

            if (success) {
                return;
            }

            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FINISHED\n" << type << "\n" << info_log << std::endl;
        }
    }
};

#endif
