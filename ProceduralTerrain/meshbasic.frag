#version 440 core

in vec2 TexCoords;
out vec4 color;

layout (binding=0) uniform sampler2DArray textureArray;
layout (location=1) uniform int layer;

void main()
{    
    color = vec4(texture(textureArray, vec3(TexCoords.xy, layer)));
}