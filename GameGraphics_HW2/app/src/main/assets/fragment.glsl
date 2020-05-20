#version 300 es

precision mediump float;

uniform sampler2D textureDiff;
uniform sampler2D textureDissolve;

uniform vec3 matSpec, matAmbi, matEmit;
uniform float matSh;
uniform vec3 srcDiffL, srcSpecL, srcAmbiL;
uniform vec3 srcDiffR, srcSpecR, srcAmbiR;
uniform float threshold;

in vec3 v_normal;
in vec2 v_texCoord;
in vec3 v_view, v_lightL, v_lightR;
in float v_attL, v_attR;

layout(location = 0) out vec4 fragColor;

void main() {

    vec3 color = texture(textureDiff, v_texCoord).rgb;

    // re-normalize unit vectors (normal, view, and light vectors)
    vec3 normal = normalize(v_normal);
    vec3 view = normalize(v_view);
    vec3 lightL = normalize(v_lightL);
    vec3 lightR = normalize(v_lightR);

    // diffuse term
    vec3 matDiff = color;
    vec3 diffL = (max(dot(normal, lightL), 0.0f) * srcDiffL * matDiff) * v_attL;
    vec3 diffR = (max(dot(normal, lightR), 0.0f) * srcDiffR * matDiff) * v_attR;
    vec3 diff = diffL + diffR;

    // specular term
    vec3 reflL = 2. * normal * dot(normal, lightL) - lightL;
    vec3 reflR = 2. * normal * dot(normal, lightR) - lightR;
    vec3 specL = (pow(max(dot(reflL, view), 0.0f), matSh) * srcSpecL * matSpec) * v_attL;
    vec3 specR = (pow(max(dot(reflR, view), 0.0f), matSh) * srcSpecR * matSpec) * v_attR;
    vec3 spec = specL + specR;

    // ambient term
    vec3 ambiL = srcAmbiL * matAmbi * v_attL;
    vec3 ambiR = srcAmbiR * matAmbi * v_attR;
    vec3 ambi = ambiL + ambiR;

    color = (diff + spec + ambi + matEmit);

    float alpha = 1.0f;

    // dissolving
    float dissolve = (texture(textureDissolve, v_texCoord).rgb)[0];
    if (dissolve < threshold)
        alpha = 0.0;

    // final output color with alpha
    fragColor = vec4(color, alpha);
}