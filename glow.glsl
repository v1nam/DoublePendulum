#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform int showOldTrails;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

const vec2 size = vec2(600, 700);   // render size
const float samples = 3.5;          // pixels per axis; higher = bigger glow, worse performance
const float quality = 1.5;          // lower = smaller glow, better quality

void main()
{
    // Texel color fetching from texture sampler
    vec4 source = texture(texture0, fragTexCoord);

    vec4 sum = vec4(0);
    vec2 sizeFactor = vec2(1)/size*quality;

    const int range = 2;

    for (int x = -range; x <= range; x++)
    {
        for (int y = -range; y <= range; y++)
        {
            sum += texture(texture0, fragTexCoord + vec2(x, y)*sizeFactor);
        }
    }

    // Calculate final fragment color
    finalColor = ((sum/(samples*samples)) + source);
    if (showOldTrails == 1)
        finalColor = vec4(finalColor.rgb, 1.);
}
