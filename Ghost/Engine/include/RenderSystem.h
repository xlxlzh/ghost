#ifndef _RENDERSYSTEM_H_
#define _RENDERSYSTEM_H_

namespace ghost
{
    class RenderSystem
    {
    public:
        virtual bool initRendersystem() = 0;

        virtual void setRenderTarget() = 0;
        virtual void setRenderTargets() = 0;
        virtual void setDepthstencil() = 0;
        virtual void setClearColor() = 0;
        virtual void clearRenderTarget() = 0;
        virtual void clearRenderTargets() = 0;

        virtual void drawPrimitive() = 0;
        virtual void drawPrimitiveIndexed() = 0;
        virtual void drawPrimitiveInstance() = 0;

        virtual void endScene() = 0;

    protected:
    };
}

#endif