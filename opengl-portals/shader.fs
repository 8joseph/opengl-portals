#version 330 core
out vec4 FragColor;

in vec2 TexCoord; // Must match the output name from shader.vs

struct Material {
    sampler2D texture_diffuse1;
};
uniform Material material;

void main()
{
    // 1. Sample the full color (RGBA)
    vec4 texColor = texture(material.texture_diffuse1, TexCoord);
    
    // 2. Output the color
    FragColor = texColor;
    
    // DEBUG: If it's still weird, uncomment the line below to force-show the Texture Coordinates
    // FragColor = vec4(TexCoord.x, TexCoord.y, 0.0, 1.0); 
}