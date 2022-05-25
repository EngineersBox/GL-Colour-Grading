#include "pipeline.hpp"

namespace GLCG::Pipelines {
    size_t Pipeline::addPass(PipelinePass const& pass) {
//                this->graph.emplace_back(NormalNode<PipelinePass>(pass));
        return this->graph.num_vertices();
    }

    size_t Pipeline::addParallelPass(CoreGraph& parallelPasses, Vertex& toAppendAfter, Vertex& toAppendFrom) {
//                if (this->graph.empty()) {
//                    throw std::runtime_error("Parallel pass cannot be first node");
//                } else if (this->graph.back().type == NodeType::PARALLEL) {
//                    throw std::runtime_error("Cannot chain non-nested parallel passes without intermediary join");
//                }
//                this->graph.emplace_back(ParallelNode<PipelinePass>(parallelPasses));

        mergeGraphs(this->graph, toAppendAfter, parallelPasses, toAppendFrom);
        return this->graph.num_vertices();
    }

    size_t Pipeline::addBlendPass(PipelinePass const& blendPass, PipelinePassBlendMode blendMode) {
//                if (this->graph.empty()) {
//                    throw std::runtime_error("Parallel pass cannot be first node");
//                } else if (this->graph.back().type != NodeType::PARALLEL) {
//                    throw std::runtime_error("Cannot join from a non-parallel pass");
//                }
//                this->graph.emplace_back(JoinNode<PipelinePass, PipelinePassBlendMode>(blendPass, blendMode));
        return this->graph.num_vertices();
    }

    void Pipeline::removePass() {

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