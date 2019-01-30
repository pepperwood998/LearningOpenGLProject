#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 stage_frag_pos;
out vec3 stage_normal;

void main()
{
    vec4 model_pos = model * vec4(pos, 1.0);
    gl_Position =  projection * view * model_pos;

    stage_frag_pos = vec3(model_pos);
    stage_normal = normal;
}
