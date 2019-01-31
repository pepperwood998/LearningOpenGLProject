#version 330 core
out vec4 final_col;
in vec3 stage_frag_pos;
in vec3 stage_normal;
in vec2 stage_tex_coord;

struct Material
{
    sampler2D orig_col;
    sampler2D specular;
    float shininess;
};
struct Light
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 view_pos;

void main()
{
    // Ambient lighting
    vec3 ambient = light.ambient * vec3(texture(material.orig_col, stage_tex_coord));

    // Diffuse lighting
    vec3 light_inv_dir = normalize(light.position - stage_frag_pos);
    vec3 norm_normal = normalize(stage_normal);
    float diff = max(dot(light_inv_dir, norm_normal), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.orig_col, stage_tex_coord));

    // Specular lighting
    vec3 reflect_dir = reflect(-light_inv_dir, norm_normal);
    vec3 view_inv_dir = normalize(view_pos - stage_frag_pos);
    float spec = pow(max(dot(reflect_dir, view_inv_dir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, stage_tex_coord));

    vec3 result = ambient + diffuse + specular;
    final_col = vec4(result, 1.0);
}
