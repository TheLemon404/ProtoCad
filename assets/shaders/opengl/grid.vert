#version 450

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 uvs;

layout(location = 0) out vec3 nearPoint;
layout(location = 1) out vec3 farPoint;
layout(location = 2) out mat4 fragView;
layout(location = 7) out mat4 fragProj;

vec3 UnprojectPoint(float x, float y, float z, mat4 view, mat4 projection) {
    mat4 viewInv = inverse(view);
    mat4 projInv = inverse(projection);
    vec4 unprojectedPoint =  viewInv * projInv * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

vec3 ProjectedPoint(float x, float y, float z, mat4 view, mat4 projection) {
    vec4 unprojectedPoint =  view * projection * vec4(x, y, z, 1.0);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

void main() {
    vec3 p = inPosition;
    nearPoint = UnprojectPoint(p.x, p.y, 0.0, view, projection).xyz; // unprojecting on the near plane
    farPoint = UnprojectPoint(p.x, p.y, 1.0, view, projection).xyz; // unprojecting on the far plane
    gl_Position = vec4(p, 1.0);
    fragView = view;
    fragProj = projection;
}