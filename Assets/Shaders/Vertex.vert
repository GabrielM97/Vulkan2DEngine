#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inUV;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec2 fragUV;

layout(set = 0, binding = 0) uniform GlobalUBO
{
    mat4 projection;
} ubo;

layout(push_constant) uniform PushConstants
{
    vec2 position;
    vec2 size;
    vec2 origin;
    float rotation;
    float padding;
    vec4 tint;
} pc;

void main()
{
    vec2 local = inPosition.xy - pc.origin;
    local *= pc.size;

    float c = cos(pc.rotation);
    float s = sin(pc.rotation);

    vec2 rotated;
    rotated.x = local.x * c - local.y * s;
    rotated.y = local.x * s + local.y * c;

    vec2 worldPosition = rotated + pc.position + pc.origin * pc.size;

    gl_Position = ubo.projection * vec4(worldPosition, inPosition.z, 1.0);
    fragColor = vec4(inColor, 1.0) * pc.tint;
    fragUV = inUV;
}