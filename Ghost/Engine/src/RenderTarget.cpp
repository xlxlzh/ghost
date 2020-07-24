#include "RenderTarget.h"

namespace ghost
{
    RenderTarget::RenderTarget(unsigned w, unsigned h, unsigned numRTs, 
        GhostColorFormat* formats, bool depth, bool msaa) :
        _width(w), _height(h), _depthAttach(depth), _msaa(msaa)
    {
        _numRTs = numRTs > GHOST_MAX_RENDERTARGETS ? GHOST_MAX_RENDERTARGETS: numRTs;
        for (unsigned i = 0; i < GHOST_MAX_RENDERTARGETS; ++i)
        {
            _formats[i] = GHOST_FORMAT_UNKNOWN;
        }

        for (unsigned i = 0; i < _numRTs; ++i)
        {
            _formats[i] = formats[i];
        }

        _onCreateRenderTarget();
    }

    void RenderTarget::setSize(unsigned w, unsigned h, unsigned d /* = 1 */)
    {
        _width = w;
        _height = h;
        _depth = d;
    }
}