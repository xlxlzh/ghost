#include "Application.h"

using namespace ghost;

class SampleApplication : public Application
{
public:
    virtual void onInit() override;
    virtual void onExit() override;
    virtual void onUpdate() override;
    virtual void onTick(float deltaTime) override;
};