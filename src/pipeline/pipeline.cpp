#include "pipeline.hpp"

namespace GLCG::Pipelines {
    boost::iterator_range<VertexIterator> Pipeline::getVertexIteratorRange() noexcept {
        return boost::make_iterator_range(vertices(this->graph));
    }
    boost::iterator_range<VertexIterator> Pipeline::getVertexIteratorRange() const noexcept {
        return boost::make_iterator_range(vertices(this->graph));
    }

    boost::iterator_range<EdgeIterator> Pipeline::getEdgeIteratorRange() noexcept {
        return boost::make_iterator_range(edges(this->graph));
    }
    boost::iterator_range<EdgeIterator> Pipeline::getEdgeIteratorRange() const noexcept {
        return boost::make_iterator_range(edges(this->graph));
    }

    CoreGraph::VertexBundleIterator Pipeline::getVertexBundleIterator() noexcept {
        return this->graph.vertexBundlesIterator();
    }
    CoreGraph::VertexBundleConstIterator Pipeline::getVertexBundleIterator() const noexcept {
        return this->graph.vertexBundlesIterator();
    }

    std::string BlendVertex::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s, MODE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str(),
            blendModeToString(this->blendMode).c_str()
        );
    }

    Vertex Pipeline::addVertex(const CoreVertexMeta& vertex) {
        if (Graph::hasVertex(this->graph, vertex.name)) {
            throw std::runtime_error("Vertex already exists");
        }
        return this->graph.add_vertex(vertex);
    }

    std::pair<Edge, bool> Pipeline::addEdge(Vertex vertex1, Vertex vertex2) {
        return this->graph.add_edge(vertex1, vertex2);
    }

    void Pipeline::removeVertex(const VertexIterator& vertexIterator) {
        /* Note: Manually calling remove then renumber since for some reason a compiler error is generated about an
         * ambiguous call to boost::next internally
         */
        this->graph.remove_vertex(*vertexIterator);
        this->graph.renumber_indices();
    }

    void Pipeline::removeVertex(const std::string &name) {
        boost::iterator_range<VertexIterator> iter = getVertexIteratorRange();
        VertexIterator matchedIter = Graph::findVertex(this->graph, name);
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
        CoreGraph::VertexBundleIterator iter = getVertexBundleIterator();
        std::for_each(
            iter.begin(),
            iter.end(),
            [&ss](CoreVertexMeta& bundle) -> void{
                ss << bundle.toString();
            }
        );
        return ss.str();
    }
}