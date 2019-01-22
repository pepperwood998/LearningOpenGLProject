#version 330 core
in vec3 stage_col;

out vec4 final_col;

void main()
{
    final_col = vec4(stage_col, 1.0);
}
