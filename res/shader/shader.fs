#version 330 core
in vec2 stage_tex_coord;

out vec4 final_col;
uniform sampler2D tex;

void main()
{
    final_col = texture(tex, stage_tex_coord);
}
