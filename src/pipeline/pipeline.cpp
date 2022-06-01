#include "pipeline.hpp"

#include <utility>

namespace GLCG::Pipelines {
    std::string BlendVertex::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s, MODE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str(),
            blendModeToString(this->blendMode).c_str()
        );
    }

    CoreGraph::vertex_descriptor Pipeline::addVertex(const CoreVertexMeta& vertex) {
        if (Graph::hasVertex(this->graph, vertex.name)) {
            throw std::runtime_error("Vertex already exists");
        }
        return this->graph.add_vertex(vertex);
    }

    void Pipeline::removeVertex(auto vertexIterator) {
        this->graph.remove_vertex_and_renumber_indices(std::move(vertexIterator));
    }

    std::string Pipeline::graphToString() {
        std::stringstream ss;
        auto bundleMap = get(boost::vertex_bundle, this->graph);
        for (Vertex v : boost::make_iterator_range(vertices(this->graph))) {
            CoreVertexMeta& bundle = bundleMap[v];
            ss << bundle.toString();
        }
        return ss.str();
    }

    boost::iterator_range<CoreGraph::vertex_iterator> Pipeline::getVertexIteratorRange() noexcept {
        return boost::make_iterator_range(vertices(this->graph));
    }
    boost::iterator_range<CoreGraph::vertex_iterator> Pipeline::getVertexIteratorRange() const noexcept {
        return boost::make_iterator_range(vertices(this->graph));
    }

    boost::iterator_range<CoreGraph::edge_iterator> Pipeline::getEdgeIteratorRange() noexcept {
        return boost::make_iterator_range(edges(this->graph));
    }
    boost::iterator_range<CoreGraph::edge_iterator> Pipeline::getEdgeIteratorRange() const noexcept {
        return boost::make_iterator_range(edges(this->graph));
    }
}