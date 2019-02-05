#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 tex_coord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal_mat;

out vec3 stage_frag_pos;
out vec3 stage_normal;
out vec2 stage_tex_coord;

void main()
{
    vec4 model_pos = model * vec4(pos, 1.0);
    gl_Position =  projection * view * model_pos;

    stage_frag_pos = vec3(model_pos);
    stage_normal = mat3(normal_mat) * normal;
    stage_tex_coord = tex_coord;
}
