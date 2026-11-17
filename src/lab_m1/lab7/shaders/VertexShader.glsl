#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform vec3 object_color;

out vec3 color;

void main()
{
    vec3 world_position = vec3(Model * vec4(v_position, 1.0));
    vec3 world_normal = normalize(mat3(transpose(inverse(Model))) * v_normal);
    vec3 L = normalize(light_position - world_position);
    vec3 V = normalize(eye_position - world_position);
    vec3 H = normalize(L + V);
    vec3 R = reflect(-L, world_normal);
    
    float diffuse_light = material_kd * max(dot(world_normal, L), 0.0);
    float receives_light = diffuse_light > 0.0 ? 1.0 : 0.0;

    float specular_phong = material_ks * receives_light * pow(max(dot(V, R), 0.0), material_shininess);
    float specular_blinn = material_ks * receives_light * pow(max(dot(world_normal, H), 0.0), material_shininess);

    float distance_to_light = distance(light_position, world_position);
    float attenuation = 1.0 / (1.0 + 0.1 * distance_to_light + 0.05 * distance_to_light * distance_to_light);

    float ambient_light = material_kd * 0.6;
    float specular_light = specular_blinn;
    float light = ambient_light + attenuation * (diffuse_light + specular_light);

    color = object_color * light;
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
