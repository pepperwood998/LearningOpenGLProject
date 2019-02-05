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
struct PointLight
{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform PointLight point_light;
uniform vec3 view_pos;

vec3 CalcPointLight(PointLight light, vec3 frag_pos, vec3 norm_normal, vec3 view_inv_dir);

void main()
{
    vec3 result = vec3(0.0);
    vec3 norm_normal = normalize(stage_normal);
    vec3 view_inv_dir = normalize(view_pos - stage_frag_pos);

    result += CalcPointLight(point_light, stage_frag_pos, norm_normal, view_inv_dir);

    final_col = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 frag_pos, vec3 norm_normal, vec3 view_inv_dir)
{
    // Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.orig_col, stage_tex_coord));
    // Diffuse Lighting
    vec3 light_inv_dir = normalize(light.position - frag_pos);
    float diff = max(dot(light_inv_dir, norm_normal), 0.0);
    vec3 diffuse = light.diffuse * vec3(texture(material.orig_col, stage_tex_coord)) * diff;
    // Specular Lighting
    vec3 reflect_dir = reflect(-light_inv_dir, norm_normal);
    float spec = pow(max(dot(reflect_dir, view_inv_dir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(texture(material.specular, stage_tex_coord)) * spec;

    return (ambient + diffuse + specular);
}
