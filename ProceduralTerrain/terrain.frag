#version 440 core

in vec2 TexCoords;
out vec4 color;

uniform sampler2DArray textureArray;
layout (location=1) uniform int layer;

void main()
{    
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);//texture(textureArray, vec3(TexCoords.xy, layer));
}