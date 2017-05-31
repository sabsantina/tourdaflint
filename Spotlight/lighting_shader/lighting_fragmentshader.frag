#version 330 core

struct Material
{
//Here we use the diffuse light settings for both diffuse and ambient light
    sampler2D m_diffuse;
    sampler2D m_specular;
    float m_shininess;
};

struct Light
{
//Now we use a position since our light has an actual physical concrete presente
    /*
        The spotlight's mechanics require the use of:
            - vector LightDir: a vectore pointing from the fragment to the light source
            - vector SpotDir: the direction the spotlight is aiming at
            - Angle Phi: the cutoff angle that specifies the spotlight's radius. Everything outside this angle is not lit by the spotlight.
            - Angle Theta: the angle between the LightDir vector and the SpotDir vector. The Theta value should be smaller than the Phi angle value to be inside the spotlight.
        
        From those, we define the spotlight's position and the direction the spotlight is aiming at.
        We also take the angle Phi, in a way, but not as raw data. The data we'll get for the angle Phi will actually be the cosine value of the angle. The reason for this is that in the fragment shader we're calculating the dot product between the light direction vector and the spotlight direction vector and this dot product will return a cosine value and not an angle, and we can't directly compare an angle with a cosine value. To retrieve the angle we then would have to calculate the inverse cosine of the dot product's result, which is an expensive operation. So, to save some performance, we calculate the cosine value of a given cutoff angle and pass this result to the fragment shader.
    */
    vec3 m_position_of_spotlight;
    vec3 m_direction_of_spotlight;
    float m_cosine_of_cutoff_angle_PHI_radians;
    
    vec3 m_ambient;
    vec3 m_diffuse;
    vec3 m_specular; 
    
    /*
        See lighting_fragmentshader.frag from LightCasters_Point for info on constants and their use.
    */
    float m_constant_constant;
    float m_constant_linear;
    float m_constant_quadratic;
};

in vec3 VS_normal_vector;
in vec3 VS_fragment_position;
in vec2 VS_texture_coordinates;

out vec4 color;

uniform vec3 viewPosition;
uniform Material materialObj;//instance of above-defined Material struct
uniform Light lightObj;//instance of above-defined Light struct

void main()
{
vec3 light_direction_vector = normalize(lightObj.m_position_of_spotlight - VS_fragment_position);
    //we define the light_direction_vector as being a vector pointing from the fragment to the spotlight

//Check if lighting is inside the spotlight cone
float cosine_of_theta = dot(light_direction_vector, normalize(-lightObj.m_direction_of_spotlight));
/*
    Recall that we defined Theta as being an angle that was necessarily smaller than Phi where a given fragment was within the scope of the spotlight 
*/
if (cosine_of_theta > lightObj.m_cosine_of_cutoff_angle_PHI_radians)
{
    //Ambient lighting
    vec3 ambientLighting = lightObj.m_ambient * vec3(texture(materialObj.m_diffuse, VS_texture_coordinates));
    
    //Diffuse lighting
    vec3 normal_vector = normalize(VS_normal_vector);
    float diffuseFactor = max(dot(normal_vector, light_direction_vector), 0.0f);
    vec3 diffuseLighting = lightObj.m_diffuse * diffuseFactor * vec3(texture(materialObj.m_diffuse, VS_texture_coordinates));
    
    //Specular lighting
        //The view direction vector is defined as being the vector pointing from the fragment to the view position
    vec3 view_direction_vector = normalize(viewPosition - VS_fragment_position);
        //The reflection direction vector is defined as being the reflection vector for an incident vector that runs from the spotlight to the fragment with normalized normal view_direction_vector 
    vec3 reflection_direction_vector = reflect(-light_direction_vector, normal_vector);
    float specularFactor = pow(max(dot(view_direction_vector, reflection_direction_vector), 0.0), materialObj.m_shininess);
    vec3 specularLighting = lightObj.m_specular * specularFactor * vec3(texture(materialObj.m_specular, VS_texture_coordinates));
    
    //Attenuation
    float distance_of_light_to_fragment = length(lightObj.m_position_of_spotlight - VS_fragment_position);
    float attenuation_intensity = 
            1.0f / 
            (lightObj.m_constant_constant + 
                (lightObj.m_constant_linear * distance_of_light_to_fragment) +
                    lightObj.m_constant_quadratic * (distance_of_light_to_fragment * distance_of_light_to_fragment));
    
   //ambientLighting *= attenuation_intensity;
   /*
        Apparently, we're better off removing the attenuation from the ambient lighting because if we move too far (from the spotlight?) then the light from the spotlight would then be darker than outside the spotlight's light since everything has its own ambient lighting by default.
    */
   diffuseLighting *= attenuation_intensity;
   specularLighting *= attenuation_intensity;
   
   color = vec4(ambientLighting + diffuseLighting + specularLighting, 1.0f);
}
else//if cosine_of_theta <= lightObj.m_cosine_of_cutoff_angle_PHI_radians
{
    //else, if the fragment doesn't belong to the scope of the spotlight's rays, use ambient light to illuminate the scene such that the scene not be complete blackness outside the spotlight.
    color = vec4(lightObj.m_ambient * vec3(texture(materialObj.m_diffuse, VS_texture_coordinates)), 1.0f);
}
}