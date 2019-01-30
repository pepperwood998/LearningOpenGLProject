#version 330 core
out vec4 final_col;

uniform vec3 object_col;
uniform vec3 light_col;

void main()
{
    final_col = vec4(object_col * light_col, 1.0);
}
