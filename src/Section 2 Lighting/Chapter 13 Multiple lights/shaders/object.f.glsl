#version 460

in vec3 v2f_pos;
in vec3 v2f_normal;
in vec2 v2f_uv;

out vec4 f_color;

uniform vec3 u_camera_pos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material u_material;

struct MaterialSample {
    vec3 normal;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    vec3 dir_view;
};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float falloff_constant;
    float falloff_linear;
    float falloff_quadratic;
};
const int n_point_lights = 4;
uniform PointLight u_point_lights[n_point_lights];

vec3 calc_point_light(PointLight point_light, MaterialSample material_sample, vec3 frag_position) {
    vec3 ambient = point_light.ambient * material_sample.diffuse;

    vec3 direction_light = normalize(point_light.position - frag_position);
    vec3 diffuse = point_light.diffuse * material_sample.diffuse
        * max(dot(material_sample.normal, direction_light), 0.0f);

    vec3 dir_reflected = reflect(-direction_light, material_sample.normal);
    vec3 specular = point_light.specular
        * material_sample.specular
        * pow(max(dot(material_sample.dir_view, dir_reflected), 0.0f),
              u_material.shininess);

    float d = distance(v2f_pos, point_light.position);
    float attenuation = 1.0f / (point_light.falloff_constant + point_light.falloff_linear * d + point_light.falloff_quadratic * d * d);

    return ambient + (diffuse + specular) * attenuation;
}

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirLight u_dir_light;

vec3 calc_dir_light(DirLight dir_light, MaterialSample material_sample) {
    vec3 ambient = dir_light.ambient * material_sample.diffuse;

    vec3 diffuse = dir_light.diffuse * material_sample.diffuse
        * max(dot(material_sample.normal, -dir_light.direction), 0.0f);

    vec3 dir_reflected = reflect(dir_light.direction, material_sample.normal);
    vec3 specular = dir_light.specular
        * material_sample.specular
        * pow(max(dot(material_sample.dir_view, dir_reflected), 0.0f),
              u_material.shininess);

    return ambient + diffuse + specular;
}

struct SpotLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;
    float inner;
    float outer;
};
uniform SpotLight u_spot_light;

vec3 calc_spot_light(SpotLight spot_light, MaterialSample material_sample, vec3 frag_position) {
    vec3 ambient = spot_light.ambient * material_sample.diffuse;

    vec3 direction_light = normalize(spot_light.position - frag_position);
    vec3 diffuse = spot_light.diffuse * material_sample.diffuse
        * max(dot(material_sample.normal, direction_light), 0.0f);

    vec3 dir_reflected = reflect(-direction_light, material_sample.normal);
    vec3 specular = spot_light.specular
        * material_sample.specular
        * pow(max(dot(material_sample.dir_view, dir_reflected), 0.0f),
              u_material.shininess);

    float attenuation = clamp(
        (dot(-direction_light, spot_light.direction) - spot_light.outer) / (spot_light.inner - spot_light.outer),
        0.0f, 1.0f
    );

    return ambient + (diffuse + specular) * attenuation;
}

void main() {
    MaterialSample material_sample;
    material_sample.normal = normalize(v2f_normal);
    material_sample.diffuse = vec3(texture(u_material.diffuse, v2f_uv));
    material_sample.specular = vec3(texture(u_material.specular, v2f_uv));
    material_sample.shininess = u_material.shininess;
    material_sample.dir_view = normalize(u_camera_pos - v2f_pos);

    vec3 color = vec3(0.0f);
    for (int i = 0; i < n_point_lights; ++i)
        color += calc_point_light(u_point_lights[i], material_sample, v2f_pos);
    color += calc_dir_light(u_dir_light, material_sample);
    color += calc_spot_light(u_spot_light, material_sample, v2f_pos);

    if(color.r > 1.0f)
        color = vec3(1.0f, 0.0f, 0.0f);

    f_color = vec4(color, 1.0f);
}