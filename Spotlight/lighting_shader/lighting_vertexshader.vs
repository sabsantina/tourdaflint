#version 330 core
layout (location = 0) in vec3 position_point;
layout (location = 1) in vec3 normal_vector;
layout (location = 2) in vec2 texture_coordinates;

out vec3 VS_normal_vector;
out vec3 VS_fragment_position;
out vec2 VS_texture_coordinates;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    gl_Position = projection_matrix * view_matrix *  model_matrix * vec4(position_point, 1.0f);
    VS_fragment_position = vec3(model_matrix * vec4(position_point, 1.0f));
    VS_normal_vector = mat3(transpose(inverse(model_matrix))) * normal_vector;  
    VS_texture_coordinates = texture_coordinates;
} 