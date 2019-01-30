#version 330 core
out vec4 final_col;
in vec3 stage_frag_pos;
in vec3 stage_normal;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform vec3 light_pos;
uniform vec3 view_pos;
uniform vec3 light_col;

void main()
{
    // Ambient lighting
    vec3 ambient = light_col * material.ambient;

    // Diffuse lighting
    vec3 light_inv_dir = normalize(light_pos - stage_frag_pos);
    vec3 norm_normal = normalize(stage_normal);
    float diff = max(dot(light_inv_dir, norm_normal), 0.0);
    vec3 diffuse = light_col * diff * material.diffuse;

    // Specular lighting
    vec3 reflect_dir = reflect(-light_inv_dir, norm_normal);
    vec3 view_inv_dir = normalize(view_pos - stage_frag_pos);
    float spec = pow(max(dot(reflect_dir, view_inv_dir), 0.0), material.shininess);
    vec3 specular = light_col * spec * material.specular;

    vec3 result = ambient + diffuse + specular;
    final_col = vec4(result, 1.0);
}
