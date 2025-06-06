#version 450

uniform bool ortho;

layout(location = 0) in vec3 nearPoint; // nearPoint calculated in vertex shader
layout(location = 1) in vec3 farPoint; // farPoint calculated in vertex shader
layout(location = 2) in mat4 fragView;
layout(location = 7) in mat4 fragProj;

layout(location = 0) out vec4 outColor;

vec4 grid(vec3 fragPos3D, float scale, bool drawAxis) {
    vec2 coord = fragPos3D.xz * scale;
    vec2 derivative = fwidth(coord);
    vec2 grid = abs(fract(coord - 0.5) - 0.5) / derivative;
    float line = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    vec4 color = vec4(0.2, 0.2, 0.2, 1.0 - min(line, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
    {
        color = vec4(0, 0.4f, 0.8f, 1);
    }
    // x axis
    if(fragPos3D.z > -0.1 * minimumz && fragPos3D.z < 0.1 * minimumz)
    {
        color = vec4(0.8f, 0, 0, 1);
    }

    return color;
}

float computeDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    return (clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(vec3 pos) {
    vec4 clip_space_pos = fragProj * fragView * vec4(pos.xyz, 1.0);
    float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
    float linearDepth = (2.0 * 0.01f * 100f) / (100f + 0.01f - clip_space_depth * (100f - 0.01f)); // get linear value between 0.01 and 100
    return linearDepth / 100f; // normalize
}

void main() {
    float t = -nearPoint.y / (farPoint.y - nearPoint.y);
    vec3 fragPos3D = nearPoint + t * (farPoint - nearPoint);

    gl_FragDepth = computeDepth(fragPos3D);

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    if(ortho)
    {
        outColor = (grid(fragPos3D, 5, true) + grid(fragPos3D, 1, true)) / (clamp(distance(vec3(0), fragPos3D) / 10, 1, 10000000));
    }
    else
    {
        outColor = (grid(fragPos3D, 5, true) + grid(fragPos3D, 1, true)) * float(t > 0);
    }
    outColor.a *= fading;
}