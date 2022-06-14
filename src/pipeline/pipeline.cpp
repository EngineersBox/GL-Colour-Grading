#include "pipeline.hpp"

namespace GLCG::Pipelines {
    boost::iterator_range<CoreGraph::VertexIterator> Pipeline::getVertexIteratorRange() noexcept {
        return boost::make_iterator_range(boost::vertices(this->graph));
    }
    boost::iterator_range<CoreGraph::VertexIterator> Pipeline::getVertexIteratorRange() const noexcept {
        return boost::make_iterator_range(boost::vertices(this->graph));
    }

    boost::iterator_range<CoreGraph::EdgeIterator> Pipeline::getEdgeIteratorRange() noexcept {
        return boost::make_iterator_range(boost::edges(this->graph));
    }
    boost::iterator_range<CoreGraph::EdgeIterator> Pipeline::getEdgeIteratorRange() const noexcept {
        return boost::make_iterator_range(boost::edges(this->graph));
    }

    CoreGraph::VertexBundleIterator Pipeline::getVertexBundleIterator() noexcept {
        return this->graph.vertexBundlesIterator();
    }

    CoreGraph::VertexBundleIterator Pipeline::getNeighbouringInVertexBundleIterator(CoreGraph::Vertex vertex) noexcept {
        return this->graph.neighbouringInVertexBundlesIterator(vertex);
    }
    CoreGraph::VertexBundleIterator Pipeline::getNeighbouringOutVertexBundleIterator(CoreGraph::Vertex vertex) noexcept {
        return this->graph.neighbouringOutVertexBundlesIterator(vertex);
    }

    std::string ParallelMixerVertex::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s, MODE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str(),
            blendModeToString(this->blendMode).c_str()
        );
    }

    std::string LayerMixerVertex::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s, MODE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str(),
            blendModeToString(this->blendMode).c_str()
        );
    }

    CoreGraph::Vertex Pipeline::addVertex(const CoreVertexMeta& vertex) {
        if (this->graph.hasVertex(vertex.name)) {
            throw std::runtime_error("Vertex already exists");
        }
        return this->graph.add_vertex(vertex);
    }

    std::pair<CoreGraph::Edge, bool> Pipeline::addEdge(CoreGraph::Vertex vertex1, CoreGraph::Vertex vertex2) {
        return this->graph.add_edge(vertex1, vertex2);
    }

    void Pipeline::removeVertex(const CoreGraph::VertexIterator& vertexIterator) {
        /* Note: Manually calling remove then renumber since for some reason a compiler error is generated about an
         * ambiguous call to boost::next internally
         */
        this->graph.remove_vertex(*vertexIterator);
        this->graph.renumber_indices();
    }

    void Pipeline::removeVertex(const std::string &name) {
        boost::iterator_range<CoreGraph::VertexIterator> iter = getVertexIteratorRange();
        CoreGraph::VertexIterator matchedIter = this->graph.findVertex(name);
        if (matchedIter == iter.end()) {
            throw std::runtime_error(Utils::String::format(
                "Vertex \"%s\" does not exist",
                name.c_str()
            ));
        }
        removeVertex(matchedIter);
    }

    inline CoreVertexMeta& Pipeline::getVertex(CoreGraph::Vertex vertex) {
        return this->graph[vertex];
    }

    std::string Pipeline::graphToString() {
        std::stringstream ss;
        CoreGraph::VertexBundleIterator iter = getVertexBundleIterator();
        std::for_each(
            iter.begin(),
            iter.end(),
            [&ss](CoreVertexMeta& bundle) -> void {
                ss << bundle.toString();
            }
        );
        return ss.str();
    }
}