#version 330 core
#define NUMBER_OF_POINT_LIGHTS 9
struct Material

{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight
{
    vec3 direction;
    
    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

};



struct PointLight

{

    vec3 position;

    

    float constant;

    float linear;

    float quadratic;

    

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

};



struct SpotLight

{

    vec3 position;

    vec3 direction;

    float cutOff;

    float outerCutOff;

    

    float constant;

    float linear;

    float quadratic;

    

    vec3 ambient;

    vec3 diffuse;

    vec3 specular;

};



in vec3 FragPos;

in vec3 Normal;

in vec2 TexCoords;



out vec4 color;



uniform vec3 viewPos;

uniform DirLight dirLight;

uniform PointLight pointLights[NUMBER_OF_POINT_LIGHTS];

uniform SpotLight spotLight;

uniform Material material;

uniform int transparency;

uniform int facadeLightsOn;

uniform int decorLightsOn;

uniform int wallTricolorOn;



// Function prototypes

vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir );

vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir );

vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir );



void main( )

{

    // Properties

    vec3 norm = normalize( Normal );

    vec3 viewDir = normalize( viewPos - FragPos );

    

    // Directional lighting

  vec3 result = CalcDirLight(dirLight, norm, viewDir);

for (int i = 0; i < NUMBER_OF_POINT_LIGHTS; i++)
{
    // Luces decorativas y faros
    if ((i < 5 || i == 8) && decorLightsOn == 1)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }

    // Luces de las fachadas
    if (i >= 5 && i < 8 && facadeLightsOn == 1)
    {
        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
}

result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

// Iluminacion tricolor exclusiva para las fachadas
if (wallTricolorOn == 1)
{
    vec3 verde = vec3(0.72, 1.0, 0.78);
    vec3 blanco = vec3(1.0, 0.94, 0.82);
    vec3 rojo = vec3(1.0, 0.72, 0.68);

    // Transiciones suaves entre los tres colores
    float transicionVerde = smoothstep(0.29, 0.38, TexCoords.x);
    float transicionRojo = smoothstep(0.62, 0.71, TexCoords.x);

    vec3 colorTricolor = mix(verde, blanco, transicionVerde);
    colorTricolor = mix(colorTricolor, rojo, transicionRojo);

    result *= colorTricolor;
}


   vec4 texColor = texture(material.diffuse, TexCoords);

float alpha = texColor.a;

if (transparency == 1)
{
    alpha = 0.5;
}

color = vec4(result, alpha);

if (color.a < 0.1)
    discard;



}



// Calculates the color when using a directional light.

vec3 CalcDirLight( DirLight light, vec3 normal, vec3 viewDir )

{

    vec3 lightDir = normalize( -light.direction );

    

    // Diffuse shading

    float diff = max( dot( normal, lightDir ), 0.0 );

    

    // Specular shading

    vec3 reflectDir = reflect( -lightDir, normal );

    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    

    // Combine results

    vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );

    vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );

    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );

    

    return ( ambient + diffuse + specular );

}



// Calculates the color when using a point light.

vec3 CalcPointLight( PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir )

{

    vec3 lightDir = normalize( light.position - fragPos );

    

    // Diffuse shading

    float diff = max( dot( normal, lightDir ), 0.0 );

    

    // Specular shading

    vec3 reflectDir = reflect( -lightDir, normal );

    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    

    // Attenuation

    float distance = length( light.position - fragPos );

    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    

    // Combine results

    vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );

    vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );

    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );

    

    ambient *= attenuation;

    diffuse *= attenuation;

    specular *= attenuation;

    

    return ( ambient + diffuse + specular );

}



// Calculates the color when using a spot light.

vec3 CalcSpotLight( SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir )

{

    vec3 lightDir = normalize( light.position - fragPos );

    

    // Diffuse shading

    float diff = max( dot( normal, lightDir ), 0.0 );

    

    // Specular shading

    vec3 reflectDir = reflect( -lightDir, normal );

    float spec = pow( max( dot( viewDir, reflectDir ), 0.0 ), material.shininess );

    

    // Attenuation

    float distance = length( light.position - fragPos );

    float attenuation = 1.0f / ( light.constant + light.linear * distance + light.quadratic * ( distance * distance ) );

    

    // Spotlight intensity

    float theta = dot( lightDir, normalize( -light.direction ) );

    float epsilon = light.cutOff - light.outerCutOff;

    float intensity = clamp( ( theta - light.outerCutOff ) / epsilon, 0.0, 1.0 );

    

    // Combine results

    vec3 ambient = light.ambient * vec3( texture( material.diffuse, TexCoords ) );

    vec3 diffuse = light.diffuse * diff * vec3( texture( material.diffuse, TexCoords ) );

    vec3 specular = light.specular * spec * vec3( texture( material.specular, TexCoords ) );

    

    ambient *= attenuation * intensity;

    diffuse *= attenuation * intensity;

    specular *= attenuation * intensity;

    

    return ( ambient + diffuse + specular );

} 