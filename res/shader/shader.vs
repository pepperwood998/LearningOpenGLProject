#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 col;
layout (location = 2) in vec2 tex_coord;

out vec3 stage_col;
out vec2 stage_tex_coord;

void main()
{
    gl_Position = vec4(pos, 1.0);
    stage_col = col;
    stage_tex_coord = tex_coord;
}
