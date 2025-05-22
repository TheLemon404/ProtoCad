#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoords;

uniform vec2 resolution;

layout(location = 0) out vec4 outColor;

float checker(vec2 uv, float repeats) {
    float cx = floor(repeats * uv.x);
    float cy = floor(repeats * uv.y);
    float result = mod(cx + cy, 2.0);
    return sign(result);
}


void main() {
    vec2 uv = vec2(gl_FragCoord.xy / resolution.xy);

    //optionally fix aspect ratio
    uv.x *= resolution.x / resolution.y;

    //50x50 checkered background
    float gray = mix(0.05f, 0.07f, checker(uv, 50.0));

    outColor.rgb = vec3(gray, gray, gray + 0.02f);
    outColor.a = 1.0;
}