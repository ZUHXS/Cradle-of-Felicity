#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;

void main()
{
    //FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(0.941, 0.243, 0.2745, 1.0);
}

