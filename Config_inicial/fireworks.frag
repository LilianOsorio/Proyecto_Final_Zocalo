#version 330 core

out vec4 FragColor;
uniform vec3 objectColor;

void main()
{
    vec2 coord = gl_PointCoord - vec2(0.5);
    float distance = length(coord);

    if (distance > 0.5)
    {
        discard;
    }

    // Difuminado suave para un aspecto de chispa real
    float alpha = smoothstep(0.5, 0.05, distance);

    // Núcleo blanco brillante en el centro que se funde con el color tricolor
    vec3 finalColor = mix(vec3(1.0), objectColor, smoothstep(0.0, 0.3, distance));

    FragColor = vec4(finalColor * (1.4 - distance), alpha);
}