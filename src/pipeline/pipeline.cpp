#include "pipeline.hpp"

namespace GLCG::Pipelines {
    std::string BlendVertex::toString() {
        return Utils::String::format(
            "{ NAME: %s, TYPE: %s, MODE: %s }",
            this->name.c_str(),
            vertexTypeToString(this->type).c_str(),
            blendModeToString(this->blendMode).c_str()
        );
    }

    size_t Pipeline::addPass(const std::string& name, PipelinePass const& pass) {
        if (Graph::hasVertex(this->graph, name)) {
            throw std::runtime_error("Vertex already exists");
        }
        return this->graph.num_vertices();
    }

    size_t Pipeline::addParallelPass(const std::string& name, CoreGraph& parallelPasses, Vertex& toAppendAfter, Vertex& toAppendFrom) {
        if (Graph::hasVertex(this->graph, name)) {
            throw std::runtime_error("Vertex already exists");
        }
//                if (this->graph.empty()) {
//                    throw std::runtime_error("Parallel pass cannot be first node");
//                } else if (this->graph.back().type == NodeType::PARALLEL) {
//                    throw std::runtime_error("Cannot chain non-nested parallel passes without intermediary join");
//                }
//                this->graph.emplace_back(ParallelNode<PipelinePass>(parallelPasses));

        /* NOTE: Verify this behaves as expected, can always resort to std::vector<PipelinePass>
         * NOTE: and construct parallel nodes manually as well as link edges.
         */
        Graph::mergeGraphs(this->graph, toAppendAfter, parallelPasses, toAppendFrom);
        return this->graph.num_vertices();
    }

    size_t Pipeline::addBlendPass(const std::string& name, PipelinePass const& blendPass, PipelinePassBlendMode blendMode) {
        if (Graph::hasVertex(this->graph, name)) {
            throw std::runtime_error("Vertex already exists");
        }
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