#ifndef DEFERREDRENDERER_H
#define DEFERREDRENDERER_H

#include <vector>

#include <time.h>

#include "common/Singleton.h"
#include "Renderer/RenderPasses.h"

using std::vector;

class DeferredRenderer : public Singleton<DeferredRenderer> {
public:
    DeferredRenderer();
    ~DeferredRenderer();

    vector<SourcePass*> sourcePasses;

    vector<DrawThing*> drawPasses;
    vector<ShadowCastPass*> shadowCastPasses;
    SourcePass * shadowReceivePass;
    InOutPass * blurVPass;
    InOutPass * aoPass;
    InOutPass * shadingPass;
    SinkPass * sinkPass;
    QSize res;
    bool drawTransparency;
    void initShadowCasters();
    void initShadowReceivers();
    void initAo();
    void initShadingPass();
    void initSinkPass();
    void init();
    void draw();

    timespec frameTime;
    timespec lastFrameTime;

    unsigned framesRendered;

    float avarageFrameTimeMs;

    vector<string> getTargetNames();

    Texture* getTargetFromName(const QString &name);

    void changePlaneSource(DebugPlane *plane, const QString &name);

};

#endif // DEFERREDRENDERER_H
