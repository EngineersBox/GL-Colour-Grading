# Pipelines

Based on <https://webglfundamentals.org/webgl/lessons/webgl-image-processing-continued.html>

* Tree node structuring
* Can split from one node to multiple and join back again with blending
* Each iteration/travseral of tree switches between textures and does a render iteration with shaders
* Tree can be customised to chain effects supporting multiple usages of effects in any ordering
* Base set of blending based on +-*/, screen (dark/light), etc

* Graph nodes:
  * Pass node: apply changes in sequence to buffered texture
  * Parallel node: parallel nested graphs, with copied buffered texture to each pass node
  * Blend node: blend together the results of previous parallel node