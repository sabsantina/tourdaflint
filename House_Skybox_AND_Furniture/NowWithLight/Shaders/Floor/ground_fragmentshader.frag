#version 330 core

#define NR_LIGHTS 3

in vec3 outNormal;
in vec3 fragPosition;
in vec2 outUV;

uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;

uniform vec3 viewPosition;
uniform vec3 object_color;
uniform sampler2D m_texture;
uniform float shininess;

out vec4 color;

float CalcPointLight(vec3 lightpos, vec3 normal, vec3 fragPosition, vec3 viewDir);

void main()
{
	vec3 tex_col = vec3(texture(m_texture, outUV));
	vec3 light_col = vec3(1.0f);

	vec3 normal = normalize(outNormal);
	vec3 viewDir = normalize(viewPosition - fragPosition);



	float result1 = CalcPointLight(lightPos1, normal, fragPosition, viewDir);
	float result2 = CalcPointLight(lightPos2, normal, fragPosition, viewDir);
	float result3 = CalcPointLight(lightPos3, normal, fragPosition, viewDir);

	vec3 result = (result1+result2+result3)*tex_col;

	color = vec4(result,1.0f);

}

float CalcPointLight(vec3 lightpos, vec3 normal, vec3 fragPosition, vec3 viewDir){
		vec3 lightDir = normalize(lightpos - fragPosition);
		float ambient = 0.3f;

		// Diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		// Specular shading
		//vec3 reflectDir = reflect(-lightDir, normal);
		//float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
		// Attenuation
		float distance = length(lightpos - fragPosition);
		float attenuation = 1.0f / (1.0f+0.09f*distance + 0.032*(distance*distance));    
		// Combine results
		//vec3 ambient_light = ambient * light_col;
		//vec3 diffuse_light = diff * light_col;
		//vec3 specular_light = spec *light_col;
		return (ambient + diff*attenuation);
		
}