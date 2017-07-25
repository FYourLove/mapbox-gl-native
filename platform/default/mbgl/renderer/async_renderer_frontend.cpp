#include "async_renderer_frontend.hpp"

#include <mbgl/renderer/backend_scope.hpp>
#include <mbgl/renderer/renderer.hpp>

namespace mbgl {

AsyncRendererFrontend::AsyncRendererFrontend(std::unique_ptr<Renderer> renderer_, RendererBackend& backend, View& view_)
    : renderer(std::move(renderer_))
    , view(view_)
    , asyncInvalidate([&] {
        if (renderer && updateParameters) {
            BackendScope guard { backend };
            renderer->render(view, *updateParameters);
        }
    }) {
}

AsyncRendererFrontend::~AsyncRendererFrontend() = default;

void AsyncRendererFrontend::reset() {
    assert(renderer);
    renderer.reset();
}

void AsyncRendererFrontend::update(std::shared_ptr<UpdateParameters> updateParameters_) {
    updateParameters = updateParameters_;
    asyncInvalidate.send();
}

void AsyncRendererFrontend::setObserver(RendererObserver& observer_) {
    assert(renderer);
    renderer->setObserver(&observer_);
}
    
Renderer* AsyncRendererFrontend::getRenderer() {
    assert(renderer);
    return renderer.get();
}

} // namespace mbgl
