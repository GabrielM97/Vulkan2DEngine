#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 2) in vec2 inInstancePosition;
layout(location = 3) in vec2 inInstanceSize;
layout(location = 4) in float inInstanceRotation;
layout(location = 5) in vec2 inInstanceOrigin;
layout(location = 6) in float inTextureIndex;
layout(location = 7) in vec2 inInstanceUVMin;
layout(location = 8) in vec2 inInstanceUVMax;
layout(location = 9) in vec4 inTint;

layout(location = 0) out vec2 fragUV;
layout(location = 1) out vec4 fragColor;

layout(set = 0, binding = 0) uniform GlobalUBO
{
    mat4 viewProjection;
} ubo;

void main()
{
    vec2 origin = inInstanceOrigin;

    vec2 local = inPosition.xy - origin;
    local *= inInstanceSize;

    float c = cos(inInstanceRotation);
    float s = sin(inInstanceRotation);

    vec2 rotated;
    rotated.x = local.x * c - local.y * s;
    rotated.y = local.x * s + local.y * c;

    vec2 worldPosition = rotated + inInstancePosition + origin * inInstanceSize;

    gl_Position = ubo.viewProjection * vec4(worldPosition, inPosition.z, 1.0);

    fragUV = mix(inInstanceUVMin, inInstanceUVMax, inUV);
    fragColor = inTint;
}
