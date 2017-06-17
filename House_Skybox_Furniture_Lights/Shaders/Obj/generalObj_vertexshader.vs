#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 outNormal;
out vec3 fragPosition;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    TexCoords = aTexCoords;    
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(aPos, 1.0);
	
	fragPosition = vec3(model_matrix * vec4(aPos, 1.0f));
	outNormal = mat3(transpose(inverse(model_matrix))) * aNormal;
}