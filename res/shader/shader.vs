#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;

out vec3 stage_col;

void main()
{
    gl_Color = vec4(pos, 1.0);
    stage_col = col;
}
