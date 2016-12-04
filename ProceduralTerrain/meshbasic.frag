#version 330 core

in vec2 TexCoord;

out vec4 color;

/*layout (std140, binding = 1) uniform Textures
{
    sampler2D texture1;
	sampler2D texture2;
	sampler2D texture3;
	sampler2D texture4;
}; */

uniform sampler2D texture1;

void main()
{
    color = texture(texture1, TexCoord);
    //color = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//color = vec4(TexCoord.x, TexCoord.y, 1.0f, 1.0f);
}