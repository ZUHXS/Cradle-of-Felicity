#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
//uniform int which_color;

void main()
{
    //FragColor = texture(texture_diffuse1, TexCoords);
    //FragColor = vec4(0.941, 0.243, 0.2745, 1.0);
	FragColor = vec4(objectColor, 1.0);
	//FragColor = which_color == 1 ? vec4(0.941, 0.243, 0.2745, 1.0) : vec4(0.0, 1.0, 0.0, 1.0);
}

