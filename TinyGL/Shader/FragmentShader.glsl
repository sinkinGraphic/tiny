#version 330 core
out vec4 FragColor;
in vec3 vertexColor;
in vec2 TexCoord;

uniform vec4 ourColor;
uniform sampler2D Texture;
uniform sampler2D Texture2;


void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    //FragColor = ourColor;
    //FragColor = vec4(vertexColor,1.f);
    FragColor = mix(texture(Texture, TexCoord), texture(Texture2, TexCoord), .2f);
}