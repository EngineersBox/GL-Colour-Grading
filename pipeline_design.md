# Pipelines

Based on <https://webglfundamentals.org/webgl/lessons/webgl-image-processing-continued.html>

* Tree value structuring
* Can split from one value to multiple and join back again with blending
* Each iteration/travseral of tree switches between textures and does a render iteration with shaders
* Tree can be customised to chain effects supporting multiple usages of effects in any ordering
* Base set of blending based on +-*/, screen (dark/light), etc

* Graph nodes:
  * Pass value: apply changes in sequence to buffered texture
  * Parallel value: parallel nested graphs, with copied buffered texture to each pass value
  * Blend value: blend together the results of previous parallel value