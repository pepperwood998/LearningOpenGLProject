#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 stage_tex_coord;
out vec3 stage_frag_pos;
out vec3 stage_normal;

void main ()
{
    gl_Position =  projection * view * model * vec4(pos, 1.0);

    stage_tex_coord = tex_coord;
    stage_frag_pos  = vec3(model * vec4(pos, 1.0));
    stage_normal    = mat3(model) * normal;
}
