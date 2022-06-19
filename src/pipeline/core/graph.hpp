#pragma once

#ifndef GL_COLOUR_GRADING_GRAPH_HPP
#define GL_COLOUR_GRADING_GRAPH_HPP

#include <string>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <utility>
#include <functional>
#include <boost/graph/directed_graph.hpp>
#include <boost/graph/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>

namespace GLCG::Pipelines {
    enum class VertexType {
        NONE,
        SERIAL,
        PARALLEL_MIXER,
        LAYER_MIXER
    };

    static std::string vertexTypeToString(VertexType type) {
        switch (type) {
            case VertexType::SERIAL: return "NORMAL";
            case VertexType::PARALLEL_MIXER: return "PARALLEL_MIXER";
            case VertexType::LAYER_MIXER: return "LAYER_MIXER";
            default: throw std::invalid_argument("Unknown vertex type");
        }
    }

    struct CoreVertexMeta {
        CoreVertexMeta() = default;
        explicit CoreVertexMeta(std::string_view name, VertexType type):
            name(name),
            type(type){}
        std::string name;
        VertexType type = VertexType::NONE;
        virtual std::string toString();
    };

    template<typename T>
    using InternalCoreGraph = boost::directed_graph<T, boost::no_property, boost::no_property>;

    template<typename T>
    class DirectedGraphWrapper: public InternalCoreGraph<T> {
        public:
            using Vertex = typename InternalCoreGraph<T>::vertex_descriptor;
            using VertexIterator = typename InternalCoreGraph<T>::vertex_iterator;
            using Edge = typename InternalCoreGraph<T>::edge_descriptor;
            using EdgeIterator = typename InternalCoreGraph<T>::edge_iterator;
            using InEdgeIterator = typename InternalCoreGraph<T>::in_edge_iterator;
            using OutEdgeIterator = typename InternalCoreGraph<T>::out_edge_iterator;
        private:
            template<typename R>
            using VertexAccessor = std::function<R(const Vertex)>;

            template<typename R>
            using InternalVertexBundleIterator = boost::range_detail::transformed_range<
                VertexAccessor<R>,
                const std::pair<VertexIterator, VertexIterator>
            >;

            template<typename R>
            [[nodiscard]]
            VertexAccessor<R> generateAccessor() noexcept {
                return [map = boost::get(boost::vertex_bundle, *this)](const Vertex v) -> R {
                    return map[v];
                };
            }

            template<typename R>
            using EdgeAccessor = std::function<R(const Edge)>;

            template<typename R, typename U>
            using InternalInVertexBundleIterator = boost::range_detail::transformed_range<
                VertexAccessor<R>,
                const boost::range_detail::transformed_range<
                    EdgeAccessor<U>,
                    const std::pair<
                        InEdgeIterator,
                        InEdgeIterator
                    >
                >
            >;

            template<typename R, typename U>
            using InternalOutVertexBundleIterator = boost::range_detail::transformed_range<
                VertexAccessor<R>,
                const boost::range_detail::transformed_range<
                    EdgeAccessor<U>,
                    const std::pair<
                        OutEdgeIterator,
                        OutEdgeIterator
                    >
                >
            >;

            template<typename R>
            [[nodiscard]]
            EdgeAccessor<R> generateOutEdgeAccessor() noexcept {
                return [this](const Edge e) -> R {
                    return boost::target(e, *this);
                };
            }

            template<typename R>
            [[nodiscard]]
            EdgeAccessor<R> generateInEdgeAccessor() noexcept {
                return [this](const Edge e) -> R {
                    return boost::source(e, *this);
                };
            }
        public:
            DirectedGraphWrapper() = default;

            using VertexBundleIterator = InternalVertexBundleIterator<T&>;
            using InVertexBundleIterator = InternalInVertexBundleIterator<T&, Vertex>;
            using OutVertexBundleIterator = InternalOutVertexBundleIterator<T&, Vertex>;

            [[nodiscard]]
            VertexBundleIterator vertexBundlesIterator() noexcept {
                return boost::vertices(*this)
                    | boost::adaptors::transformed(generateAccessor<T&>());
            }

            [[nodiscard]]
            OutVertexBundleIterator neighbouringOutVertexBundlesIterator(const Vertex vertex) noexcept {
                return boost::out_edges(vertex, *this)
                    | boost::adaptors::transformed(generateOutEdgeAccessor<Vertex>())
                    | boost::adaptors::transformed(generateAccessor<T&>());
            }

            [[nodiscard]]
            InVertexBundleIterator neighbouringInVertexBundlesIterator(const Vertex vertex) noexcept {
                return boost::in_edges(vertex, *this)
                    | boost::adaptors::transformed(generateInEdgeAccessor<Vertex>())
                    | boost::adaptors::transformed(generateAccessor<T&>());
            }

            // For some incredibly stupid reason, these three methods are not known during the link stage
            // if they are defined here and implemented in the source file. I don't have the patience to deal with
            // a compiler's and linker's bullshit, so I'm just gonna declare them in the header.
            VertexIterator findVertex(const std::string_view& name) {
                boost::iterator_range<VertexIterator> iter = boost::make_iterator_range(boost::vertices(*this));
                return std::find_if(
                    iter.begin(),
                    iter.end(),
                    [map = boost::get(boost::vertex_bundle, *this), &name](const Vertex& current) -> bool {
                        return map[current].name == name;
                    }
                );
            };
            [[nodiscard]]
            bool hasVertex(const std::string_view& name) const {
                boost::iterator_range<VertexIterator> iter = boost::make_iterator_range(boost::vertices(*this));
                return std::any_of(
                    iter.begin(),
                    iter.end(),
                    [map = boost::get(boost::vertex_bundle, *this), &name](const Vertex& current) -> bool {
                        return map[current].name == name;
                    }
                );
            };
            void mergeGraphs(Vertex graph1Vertex,
                             const InternalCoreGraph<CoreVertexMeta>& graph2,
                             Vertex graph2Vertex) {
                std::vector<Vertex> orig2copy_data(boost::num_vertices(graph2));
                auto mapV = boost::make_iterator_property_map(orig2copy_data.begin(), boost::get(boost::vertex_index, graph2));
                boost::copy_graph(graph2, *this, boost::orig_to_copy(mapV));
                Vertex graph2SourceVertex = mapV[graph2Vertex];
                boost::add_edge(graph1Vertex, graph2SourceVertex, *this);
            }
            [[nodiscard]]
            bool hasInVertices(Vertex vertex) const;
            [[nodiscard]]
            bool hasOutVertices(Vertex vertex) const;
    };

    using CoreGraph = DirectedGraphWrapper<CoreVertexMeta>;
}

#endif //GL_COLOUR_GRADING_GRAPH_HPP
