#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Must match the output name from shader.vs

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;

void main()
{
    vec4 texColor = texture(material.texture_diffuse1, TexCoord);
    FragColor = texColor;
}