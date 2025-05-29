#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoords;

uniform vec2 resolution;
uniform sampler3D voxelTexture;

uniform int isOrtho;
uniform float orthoZoomFactor;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPosition;
uniform vec3 cameraForward;

layout(location = 0) out vec4 outColor;

struct Ray
{
    vec3 origin;
    vec3 direction;
};

vec3 initializeRay()
{
    return vec3(0);
}

vec4 trace(Ray ray)
{
    vec3 oc = ray.origin - vec3(0.0f);
    float a = dot(ray.direction, ray.direction);
    float b = 2.0 * dot(oc, ray.direction);
    float c = dot(oc,oc) - 0.5 * 0.5;
    float discriminant = b*b - 4*a*c;
    return vec4(discriminant>0);
}

void main() {
    vec2 UV = gl_FragCoord.xy / resolution;

    ivec3 voxelCoord = ivec3(gl_FragCoord.xyz);
    vec3 value = texture(voxelTexture, voxelCoord).xyz;

    vec3 rayOrigin;
    vec3 rayDirection;

    if(isOrtho == 1)
    {
        vec2 screenCenter = resolution / 2.0;
        vec2 nCoord = (gl_FragCoord.xy - screenCenter.xy) / screenCenter.y;

        float orthoWidth = (resolution.x / 1000) * orthoZoomFactor;
        float orthoHeight = (resolution.y / 1000) * orthoZoomFactor;

        float x = mix(-orthoWidth, orthoWidth, UV.x);
        float y = mix(-orthoHeight, orthoHeight, UV.y);

        rayOrigin = (inverse(view) * vec4(vec2(x, y), 1.0, 1.0)).xyz;
        rayDirection = normalize((inverse(view) * vec4(0,0,-1,0)).xyz);
    }
    else
    {
        vec4 clip = vec4(UV * 2.0 - 1.0, 1.0, 1.0);
        vec4 viewRay = inverse(projection) * clip;
        viewRay /= viewRay.w;
        vec4 worldDirection = inverse(view) * vec4(viewRay.xyz, 0.0);
        rayOrigin = cameraPosition;
        rayDirection = normalize(worldDirection.xyz);
    }

    Ray ray = {rayOrigin, rayDirection};

    //ray traversal
    outColor = trace(ray);
}