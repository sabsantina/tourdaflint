#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 VS_normal_vector;
in vec3 VS_fragment_position;

struct Material
{
//Here we use the diffuse light settings for both diffuse and ambient light
    sampler2D m_diffuse;
    sampler2D m_specular;
    sampler2D m_emission;
    float m_shininess;
};

struct Light
{
    vec3 m_position;
    vec3 m_ambient;
    vec3 m_diffuse;
    vec3 m_specular; 
};

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

/*the above uniform textures can be set to be the member variables of the Light and Material structs!!*/

uniform vec3 viewPosition;
uniform Material materialObj;//instance of above-defined Material struct
uniform Light lightObj;//instance of above-defined Light struct

void main()
{    
materialObj.m_diffuse = texture_diffuse1;
materialObj.m_specular = texture_specular1;

    //Ambient lighting
vec3 ambientLight = lightObj.m_ambient * vec3(texture(materialObj.m_diffuse, TexCoords));
        //Ambient and diffuse light are usually pretty similar in terms of their arguments

//Diffuse lighting
vec3 normal_vector = normalize(VS_normal_vector);
vec3 light_direction_vector = normalize(lightObj.m_position - VS_fragment_position);
float diffuseFactor = max(dot(normal_vector, light_direction_vector), 0.0f);
vec3 diffuseLight = lightObj.m_diffuse * diffuseFactor * vec3(texture(materialObj.m_diffuse, TexCoords));

//Specular lighting
vec3 viewDirection = normalize(viewPosition - VS_fragment_position);
vec3 reflectDirection = reflect(-light_direction_vector, normal_vector);
float specularFactor = pow(max(dot(viewDirection, reflectDirection), 0.0), materialObj.m_shininess);
vec3 specularLight = lightObj.m_specular * specularFactor * vec3(texture(materialObj.m_specular, TexCoords));

//Emission lighting (if any)
vec3 emissionLight = vec3(texture(materialObj.m_emission, VS_texture_coordinates));

result = ambientLight + diffuseLight + specularLight + emissionLight;


    //FragColor = texture(texture_diffuse1, TexCoords);
FragColor = vec4(result, 1.0f);
}

