#version 450
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

layout (location = 0) out vec4 fragColor;

layout (push_constant) uniform PushConstants
{
    vec2 offset;
    vec2 scale;
    vec4 tint;
} pc;

void main() {

    vec2 scaledPosition = inPosition.xy * pc.scale;
    vec2 transformedPosition = scaledPosition + pc.offset;
    gl_Position = vec4(transformedPosition, inPosition.z, 1.0);
    fragColor = vec4(inColor, 1) * pc.tint;
}