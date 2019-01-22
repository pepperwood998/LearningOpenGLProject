#version 330 core
in vec3 stage_col;
in vec2 stage_tex_coord;

out vec4 final_col;
uniform sampler2D tex;

void main()
{
    final_col = texture(tex, stage_tex_coord);
}
