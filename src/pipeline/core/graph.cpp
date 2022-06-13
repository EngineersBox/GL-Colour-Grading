#include "graph.hpp"

#include "stringUtils.hpp"

namespace GLCG::Pipelines {
    std::string CoreVertexMeta::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str()
        );
    }

    template<typename T>
    typename DirectedGraphWrapper<T>::InternalVertex DirectedGraphWrapper<T>::firstVertex() {
        return static_cast<InternalVertex>(static_cast<boost::iterator_range<CoreGraph::VertexIterator>>(boost::make_iterator_range(vertexBundlesIterator())).begin());
    }

    template<typename T>
    typename DirectedGraphWrapper<T>::InternalVertex DirectedGraphWrapper<T>::lastVertex() {
        return static_cast<InternalVertex>(static_cast<boost::iterator_range<CoreGraph::VertexIterator>>(boost::make_iterator_range(vertexBundlesIterator())).end());
    }

    template<typename T>
    typename DirectedGraphWrapper<T>::VertexIterator DirectedGraphWrapper<T>::findVertex(const std::string_view& name) {
        boost::iterator_range<CoreGraph::VertexIterator> iter = boost::make_iterator_range(vertices(*this));
        return std::find_if(
            iter.begin(),
            iter.end(),
            [this, &name](const CoreGraph::Vertex& current) -> bool{
                return this[current].name == name;
            }
        );
    }

    template<typename T>
    bool DirectedGraphWrapper<T>::hasVertex(const std::string_view& name) {
        boost::iterator_range<CoreGraph::VertexIterator> iter = boost::make_iterator_range(vertices(*this));
        return std::any_of(
            iter.begin(),
            iter.end(),
            [this, &name](const CoreGraph::Vertex& current) -> bool {
                return this[current].name == name;
            }
        );
    }

    template<typename T>
    void DirectedGraphWrapper<T>::mergeGraphs(Vertex graph1Vertex,
                                              const InternalCoreGraph<CoreVertexMeta>& graph2,
                                              Vertex graph2Vertex) {
        std::vector<CoreGraph::Vertex> orig2copy_data(num_vertices(graph2));
        auto mapV = make_iterator_property_map(orig2copy_data.begin(), get(boost::vertex_index, graph2));
        boost::copy_graph(graph2, *this, boost::orig_to_copy(mapV));
        CoreGraph::Vertex graph2SourceVertex = mapV[graph2Vertex];
        boost::add_edge(graph1Vertex, graph2SourceVertex, *this);
    }
}