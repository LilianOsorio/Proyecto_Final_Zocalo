
#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Control del movimiento de la bandera
uniform float windTime;
uniform int windEnabled;

void main()
{
    vec3 posicionAnimada = position;

    // Activar el viento solamente para la bandera
    if (windEnabled == 1)
    {
        
// La parte cercana al asta permanece fija
float influencia = smoothstep(-0.70, 0.75, position.x);

// Intensidad variable del viento
float fuerzaViento =
    0.24 + 0.07 * sin(windTime * 0.9);

// Onda principal amplia
float fase1 =
    windTime * 3.2 - (position.x + 0.75) * 5.0;

// Onda secundaria mas rapida
float fase2 =
    windTime * 4.8 - (position.x + 0.75) * 9.0;

// Combinar ondas
float onda =
    (sin(fase1) * 0.75 +
     sin(fase2) * 0.25) *
    fuerzaViento *
    influencia;

// Movimiento adicional para dar sensacion de tela
float ondulacionExtra =
    sin(windTime * 2.0 - position.x * 4.0)
    * 0.06 * influencia;

// Aplicar movimiento
posicionAnimada.z += onda + ondulacionExtra;

    }

    vec4 posicionMundo = model * vec4(posicionAnimada, 1.0);

    gl_Position = projection * view * posicionMundo;

    FragPos = vec3(posicionMundo);

    Normal = mat3(transpose(inverse(model))) * normal;

    TexCoords = texCoords;
}
