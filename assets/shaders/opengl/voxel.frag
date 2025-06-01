#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 texCoords;

uniform vec2 resolution;

uniform sampler3D voxelVolume;
uniform sampler3D gasVolume;
uniform sampler3D distortionVolume;

uniform int isOrtho;
uniform float orthoZoomFactor;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPosition;
uniform vec3 cameraForward;

layout(location = 0) out vec4 outColor;

struct Ray
{
    vec3 position;
    vec3 velocity;
};

vec3 initializeRay()
{
    return vec3(0);
}

float ray_aabb(vec3 b_min, vec3 b_max, Ray ray)
{
    float tmin = 0, tmax = 1e30f;

    for(int axis = 0; axis < 3; axis++)
    {
        float t1 = (b_min[axis] - ray.position[axis]) / ray.velocity[axis];
        float t2 = (b_max[axis] - ray.position[axis]) / ray.velocity[axis];

        float dmin = min(t1, t2);
        float dmax = max(t1, t2);

        tmin = max(dmin, tmin);
        tmax = min(dmax, tmax);
    }

    if(tmax >= tmin) return tmin;

    return 1e30f;
}

vec4 traverse(Ray ray)
{
    //constants
    const vec3 gridMin = vec3(-1);
    const vec3 gridMax = vec3(1);

    const float entry_t = ray_aabb(gridMin, gridMax, ray);
    const int GRID_SIZE = textureSize(voxelVolume, 0).x;
    const int DISTORTION_SIZE = textureSize(distortionVolume, 0).x;

    const int MAX_STEPS = DISTORTION_SIZE * 3;
    const vec3 unit = DISTORTION_SIZE / (gridMax - gridMin);

    if(entry_t == 1e30f) return vec4(0);

    vec3 entryPos = ((ray.position + ray.velocity * entry_t) - gridMin) * unit;
    vec3 step = vec3(sign(ray.velocity.x), sign(ray.velocity.y), sign(ray.velocity.z));
    vec3 delta = abs(1.0f/ray.velocity);

    //updated every step
    vec3 pos = clamp(floor(entryPos), 0, DISTORTION_SIZE);
    vec3 tmax = (pos - entryPos + max(step, 0)) / ray.velocity;

    int axis = 0;
    for (int steps = 0; steps < MAX_STEPS; ++steps)
    {
        vec3 voxelCoord = pos / vec3(DISTORTION_SIZE);
        vec4 voxel = texture(voxelVolume, voxelCoord);
        vec4 gas = texture(gasVolume, voxelCoord);
        vec4 distortion = texture(distortionVolume, voxelCoord);

        if(voxel.a != 0)
        {
            return voxel;
        }

        if(distortion.a != 0)
        {
            ray.velocity += distortion.xyz / DISTORTION_SIZE;
            step = vec3(sign(ray.velocity.x), sign(ray.velocity.y), sign(ray.velocity.z));
            delta = abs(1.0f/ray.velocity);
        }

        //checking for event horizon
        if(steps == MAX_STEPS - 1)
        {
            return vec4(0,0,0,1.0);
        }

        if(tmax.x < tmax.y)
        {
            if(tmax.x < tmax.z)
            {
                pos.x += step.x;
                if(pos.x < 0 || pos.x >= DISTORTION_SIZE) break;
                axis = 0;
                tmax.x += delta.x;
            } else
            {
                pos.z += step.z;
                if(pos.z < 0 || pos.z >= DISTORTION_SIZE) break;
                axis = 2;
                tmax.z += delta.z;
            }
        }
        else
        {
            if(tmax.y < tmax.z)
            {
                pos.y += step.y;
                if (pos.y < 0 || pos.y >= DISTORTION_SIZE) break;
                axis = 1;
                tmax.y += delta.y;
            }
            else
            {
                pos.z += step.z;
                if (pos.z < 0 || pos.z >= DISTORTION_SIZE) break;
                axis = 2;
                tmax.z += delta.z;
            }
        }
    }

    return vec4(0);
}

void main() {
    vec2 UV = gl_FragCoord.xy / resolution;

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
    outColor = traverse(ray);
}