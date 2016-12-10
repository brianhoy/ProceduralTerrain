#version 440 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2DArray textureArray;
layout (location=1) uniform int layer;

void main()
{    
    color = texture(textureArray, vec3(TexCoords.xy, layer));
}