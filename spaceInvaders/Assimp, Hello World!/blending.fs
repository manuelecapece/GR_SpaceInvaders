#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;
uniform float alpha;
uniform vec3 colorcube;


void main()
{             
    vec4 texColor = texture(texture1, TexCoords)*vec4(colorcube, 1.0);
    texColor.a = alpha;
    FragColor = texColor;
}