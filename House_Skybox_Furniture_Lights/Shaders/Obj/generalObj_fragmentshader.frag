#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 outNormal;
in vec3 fragPosition;

//Light Positions
uniform vec3 lightPos1;
uniform vec3 lightPos2;
uniform vec3 lightPos3;

//Light Color (to turn light on and off)
uniform vec3 lightCol1;
uniform vec3 lightCol2;
uniform vec3 lightCol3;

uniform vec3 viewPosition;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

vec3 specular_light;
float attenuation;

vec3 CalcPointLight(vec3 lightpos, vec3 lightCol, vec3 normal, vec3 fragPosition, vec3 viewDir);

void main()
{    
    vec3 tex_col = vec3(texture(texture_diffuse1, TexCoords));
	vec3 spec_col =  vec3(texture(texture_specular1, TexCoords));

	vec3 normal = normalize(outNormal);
	vec3 viewDir = normalize(viewPosition - fragPosition);

	vec3 result1 = CalcPointLight(lightPos1, lightCol1, normal, fragPosition, viewDir);
	vec3 spec_result = specular_light * attenuation * spec_col;
	vec3 result2 = CalcPointLight(lightPos2, lightCol2, normal, fragPosition, viewDir);
	spec_result += specular_light * attenuation * spec_col;
	vec3 result3 = CalcPointLight(lightPos3, lightCol3, normal, fragPosition, viewDir);
	spec_result += specular_light * attenuation * spec_col;
	
	vec3 result = (result1+result2+result3)*tex_col + spec_result;
	
	FragColor = vec4(result,1.0f);
	
}

vec3 CalcPointLight(vec3 lightpos, vec3 lightCol, vec3 normal, vec3 fragPosition, vec3 viewDir){
		vec3 lightDir = normalize(lightpos - fragPosition);
		float ambient = 0.3f;

		// Diffuse shading
		float diff = max(dot(normal, lightDir), 0.0);
		
		// Specular shading
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
		
		// Attenuation
		float distance = length(lightpos - fragPosition);
		attenuation = 1.0f / (1.0f+0.09f*distance + 0.032*(distance*distance));   
		 
		// Combine results
		vec3 ambient_light = ambient * lightCol;
		vec3 diffuse_light = diff * lightCol;
		specular_light = spec *lightCol;

		return (ambient_light + diffuse_light*attenuation);
}