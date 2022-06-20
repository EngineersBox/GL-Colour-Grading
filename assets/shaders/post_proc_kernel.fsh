#version 410 core

precision mediump float;

in vec2 v_texCoord;

uniform sampler2D u_image;
uniform vec2 u_textureSize;
uniform float u_kernel[9];
uniform float u_kernelWeight;

out vec4 FragColor;

void main() {
    vec2 onePixel = vec2(1.0, 1.0) / u_textureSize;
    // Verbose addition is faster than for-driven iterative summation on a GPU
    vec4 colorSum =
        texture(u_image, v_texCoord + onePixel * vec2(-1, -1)) * u_kernel[0] +
        texture(u_image, v_texCoord + onePixel * vec2( 0, -1)) * u_kernel[1] +
        texture(u_image, v_texCoord + onePixel * vec2( 1, -1)) * u_kernel[2] +
        texture(u_image, v_texCoord + onePixel * vec2(-1,  0)) * u_kernel[3] +
        texture(u_image, v_texCoord + onePixel * vec2( 0,  0)) * u_kernel[4] +
        texture(u_image, v_texCoord + onePixel * vec2( 1,  0)) * u_kernel[5] +
        texture(u_image, v_texCoord + onePixel * vec2(-1,  1)) * u_kernel[6] +
        texture(u_image, v_texCoord + onePixel * vec2( 0,  1)) * u_kernel[7] +
        texture(u_image, v_texCoord + onePixel * vec2( 1,  1)) * u_kernel[8];
    FragColor = vec4((colorSum / u_kernelWeight).rgb, 1);
}