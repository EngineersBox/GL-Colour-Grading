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

    std::pair<CoreGraph::edge_descriptor, bool> Pipeline::addEdge(CoreGraph::vertex_descriptor vertex1, CoreGraph::vertex_descriptor vertex2) {
        return this->graph.add_edge(vertex1, vertex2);
    }

    void Pipeline::removeVertex(const CoreGraph::vertex_iterator& vertexIterator) {
        /* Note: Manually calling remove then renumber since for some reason a compiler error is generated about an
         * ambiguous call to boost::next internally
         */
        this->graph.remove_vertex(*vertexIterator);
        this->graph.renumber_indices();
    }

    void Pipeline::removeVertex(const std::string &name) {
        boost::iterator_range<CoreGraph::vertex_iterator> iter = getVertexIteratorRange();
        CoreGraph::vertex_iterator matchedIter = Graph::findVertex(this->graph, name);
        if (matchedIter == iter.end()) {
            throw std::runtime_error(Utils::String::format(
                "Vertex \"%s\" does not exist",
                name.c_str()
            ));
        }
        removeVertex(matchedIter);
    }

    std::string Pipeline::graphToString() {
        std::stringstream ss;
        auto bundleMap = get(boost::vertex_bundle, this->graph);
        for (Vertex v : getVertexIteratorRange()) {
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