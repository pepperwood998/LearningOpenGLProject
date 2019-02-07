#version 330 core
in vec3 stage_frag_pos;
in vec3 stage_normal;
in vec2 stage_tex_coord;

struct Material
{
    sampler2D orig_col;
    sampler2D specular;

    float shininess;
};
struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};
struct SpotLight
{
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float cut_off;
    float outer_cut_off;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 view_pos;
// Object's material
uniform Material material;
// Lights
uniform DirLight dir_light;
uniform PointLight point_lights[4];
uniform SpotLight spot_light;

out vec4 final_col;

vec3 CalcDirLight   (DirLight light, vec3 norm_normal, vec3 view_inv_dir);
vec3 CalcPointLight (PointLight light, vec3 frag_pos, vec3 norm_normal, vec3 view_inv_dir);
vec3 CalcSpotLight  (SpotLight light, vec3 frag_pos, vec3 norm_normal, vec3 view_inv_dir);

void main ()
{
    vec3 result       = vec3(0.0);
    vec3 norm_normal  = normalize(stage_normal);
    vec3 view_inv_dir = normalize(view_pos - stage_frag_pos);

    result += CalcDirLight(dir_light, norm_normal, view_inv_dir);
    for (int i = 0; i < 4; ++i)
    {
        result += CalcPointLight(point_lights[i], stage_frag_pos, norm_normal, view_inv_dir);
    }
    result += CalcSpotLight(spot_light, stage_frag_pos, norm_normal, view_inv_dir);

    final_col = vec4(result, 1.0);
}

vec3 CalcDirLight (DirLight light, vec3 norm_normal, vec3 view_inv_dir)
{
    // Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.orig_col, stage_tex_coord));
    // Diffuse Lighting
    vec3 light_inv_dir = normalize(-light.direction);
    float diff         = max(dot(light_inv_dir, norm_normal), 0.0);
    vec3 diffuse       = light.diffuse * vec3(texture(material.orig_col, stage_tex_coord)) * diff;
    // Specular Lighting
    vec3 reflect_dir = reflect(-light_inv_dir, norm_normal);
    float spec       = pow(max(dot(reflect_dir, view_inv_dir), 0.0), material.shininess);
    vec3 specular    = light.specular * vec3(texture(material.specular, stage_tex_coord)) * spec;

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight (PointLight light, vec3 frag_pos, vec3 norm_normal, vec3 view_inv_dir)
{
    // Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.orig_col, stage_tex_coord));
    // Diffuse Lighting
    vec3 light_inv_dir = normalize(light.position - frag_pos);
    float diff         = max(dot(light_inv_dir, norm_normal), 0.0);
    vec3 diffuse       = light.diffuse * vec3(texture(material.orig_col, stage_tex_coord)) * diff;
    // Specular Lighting
    vec3 reflect_dir = reflect(-light_inv_dir, norm_normal);
    float spec       = pow(max(dot(reflect_dir, view_inv_dir), 0.0), material.shininess);
    vec3 specular    = light.specular * vec3(texture(material.specular, stage_tex_coord)) * spec;

    // Attenuation
    float dist_light_frag = length(light.position - frag_pos);
    float attenuation     = 1.0 / (light.constant + 
                                   light.linear * dist_light_frag + 
                                   light.quadratic * (dist_light_frag * dist_light_frag));
                                   
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight (SpotLight light, vec3 frag_pos, vec3 norm_normal, vec3 view_inv_dir)
{
    // Ambient Lighting
    vec3 ambient = light.ambient * vec3(texture(material.orig_col, stage_tex_coord));
    // Diffuse Lighting
    vec3 light_inv_hit = normalize(light.position - frag_pos);
    float diff         = max(dot(light_inv_hit, norm_normal), 0.0);
    vec3 diffuse       = light.diffuse * vec3(texture(material.orig_col, stage_tex_coord)) * diff;
    // Specular Lighting
    vec3 reflect_dir = reflect(-light_inv_hit, norm_normal);
    float spec       = pow(max(dot(reflect_dir, view_inv_dir), 0.0), material.shininess);
    vec3 specular    = light.specular * vec3(texture(material.specular, stage_tex_coord)) * spec;

    // Soft edges
    float theta     = max(dot(-light_inv_hit, normalize(light.direction)), 0.0);
    float epsilon   = light.cut_off - light.outer_cut_off;
    float intensity = clamp((theta - light.outer_cut_off) / epsilon, 0.0, 1.0);

    ambient  *= intensity;
    diffuse  *= intensity;
    specular *= intensity;

    // Attenuation
    float dist_light_frag = length(light.position - frag_pos);
    float attenuation     = 1.0 / (light.constant + 
                                   light.linear * dist_light_frag + 
                                   light.quadratic * (dist_light_frag * dist_light_frag));

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
