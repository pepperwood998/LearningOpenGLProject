#version 330 core
in vec2 stage_tex_coord;

struct Material
{
    sampler2D texture_diffuse1;
};

// Object's material
uniform Material material;

out vec4 final_col;

void main ()
{
    final_col = texture(material.texture_diffuse1, stage_tex_coord);
}
