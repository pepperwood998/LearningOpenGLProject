#version 330 core
in vec2 stage_tex_coord;
in vec3 stage_frag_pos;
in vec3 stage_normal;

struct Material
{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;

    float shininess;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform vec3 view_pos;
// Object's material
uniform Material material;
// Light
uniform DirLight dir_light;

out vec4 final_col;

vec3 CalcDirLight (DirLight light, vec3 norm_normal, vec3 view_inv_dir);

void main ()
{
    vec3 norm_normal  = normalize(stage_normal);
    vec3 view_inv_dir = normalize(view_pos - stage_frag_pos);

    vec3 result = vec3(0.0);

    result += CalcDirLight(dir_light, norm_normal, view_inv_dir);

    final_col = vec4(result, 1.0);
}

vec3 CalcDirLight (DirLight light, vec3 norm_normal, vec3 view_inv_dir)
{
    // Ambient Light
    vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, stage_tex_coord));
    // Diffuse Light
    vec3 light_inv_dir = normalize(-light.direction);
    float diff         = max(dot(light_inv_dir, norm_normal), 0.0);
    vec3 diffuse       = light.diffuse * vec3(texture(material.texture_diffuse1, stage_tex_coord)) * diff;
    // Specular Light
    vec3 reflect_dir = reflect(-light_inv_dir, norm_normal);
    float spec       = pow(max(dot(reflect_dir, view_inv_dir), 0.0), material.shininess);
    vec3 specular    = light.specular * vec3(texture(material.texture_specular1, stage_tex_coord)) * spec;

    return (ambient + diffuse + specular);
}
