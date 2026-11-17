#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texcoord;
layout(location = 3) in vec3 v_color;
// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float time;
// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
vec4 Compute(){
    vec3 position = v_position;
    //position.y += sin(time);
    //position.x -= cos(time);
    return Projection*View*Model* vec4(position, 1);
}

void main()
{
    // TODO(student): Send output to fragment shader

    // TODO(student): Compute gl_Position

    gl_Position = Compute();
    frag_color = vec3(sin(time), cos(time),0);
    gl_Position.y += sin(time);
}
