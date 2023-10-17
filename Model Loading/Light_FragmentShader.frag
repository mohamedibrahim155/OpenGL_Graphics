#version 330 core

out vec4 FragColor;

struct Material {
   // vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;    
    float shininess;
}; 

struct Light {
   // vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPosition;  
in vec3 Normal;  
in vec2 TextureCoordinates;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;
//uniform vec3 objectColor;
//uniform vec3 lightColor;

//void main()
//{
//    FragColor = vec4(lightColor * objectColor, 1.0);
//    //FragColor = vec4(lightColor , 1.0);
//}

//void main()
//{
//    float ambientStrength = 0.1;
//    vec3 ambient = ambientStrength * lightColor;
//
//    vec3 result = ambient * objectColor;
//    FragColor = vec4(result, 1.0);
//}  


void main()
{
    // ambient
  //  float ambientStrength = 0.1;
    vec3 ambient = light.ambient * texture(material.diffuse,TextureCoordinates).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff *texture(material.diffuse,TextureCoordinates).rgb;

    //specular
    
    vec3 viewDir = normalize(viewPos - FragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TextureCoordinates).rgb;  
            
   // vec3 result = (ambient + diffuse) * objectColor;
    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
} 