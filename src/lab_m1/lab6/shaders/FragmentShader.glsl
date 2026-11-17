#version 330

// Input
in vec3 frag_color;
// TODO(student): Get values from vertex shader

// Output
layout(location = 0) out vec3 out_color;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec3(1,0,0);
    out_color = vec3(frag_color);
}
