#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
uniform sampler2D tex0;

void main() {
    vec4 nextColor = texture(tex0, texCoord);
    FragColor = vec4(nextColor.rrr, nextColor.a);
}