#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_col;

uniform mat4 m_model;
uniform mat4 m_view;
uniform mat4 m_projection;

out vec3 f_col;

void main() {
    mat4 mvp = m_projection * m_view * m_model;

    gl_Position = mvp * vec4(a_pos, 1.0);

    f_col = a_col;
}
