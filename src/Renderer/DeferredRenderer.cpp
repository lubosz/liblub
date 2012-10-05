#include "DeferredRenderer.h"
#include "Scene/Scene.h"
#include "Texture/Textures.h"
#include "System/TemplateEngine.h"
#include "Shader/Shaders.h"
#include "Scene/SceneGraph.h"
#include "Procedural/Geometry.h"


DeferredRenderer::DeferredRenderer() : drawTransparency(true) {
    timer = new Timer();
    isInitialized = false;
    useAO = false;
    useShadows = false;
    useReflection = false;
}

DeferredRenderer::~DeferredRenderer() {
    LogDebug << "Avarage Frame Time was:" << timer->avarageFrameTimeMs << "ms" << 1000.0 / timer->avarageFrameTimeMs << "fps";
}

void DeferredRenderer::draw() {

    foreach(DrawThing * pass, drawPasses) {
        if (pass->enabled) {
            pass->timer->startFrame();
            pass->draw();
            pass->timer->frameDone();
        }
    }

    timer->frameDone();
    timer->countAverage();

//    QString fps = QString::number(BILLION/frameTime.tv_nsec) + "fps";
//    QString secs = QString::number(frameTime.tv_sec) + "secs";
//    QString miliSecounds = QString::number(frameTime.tv_nsec / float(MILLION)) + "ms";

//    LogDebug << Timer::getTime()<< fps.toStdString() << miliSecounds.toStdString() << secs.toStdString();

//    LogDebug << avarageFrameTimeMs << "ms" << 1000.0 / avarageFrameTimeMs << "fps";
}

void DeferredRenderer::init() {
    res = Scene::Instance().getResolution();

    if (useShadows) {
        initShadowCasters();
    }
    initShadowReceivers();

    if (useAO) {
        initAo();
        sourcePasses.push_back(aoPass);
        sourcePasses.push_back(blurVPass);
    }

    initShadingPass();
    initSinkPass();

    sourcePasses.push_back(shadowReceivePass);
    sourcePasses.push_back(shadingPass);

    isInitialized = true;
}

void DeferredRenderer::initShadowCasters() {
    //
    // shadow passes
    //
    unsigned countLights = 0;
    foreach(Light * light, Scene::Instance().lights) {
      string name = "shadowDepthSource" + QString::number(countLights).toStdString();
      vector<Texture*> shadowSources = {new ShadowTexture(res, name)};
      ShadowCastPass * shadowCastPass = new ShadowCastPass(
          res, shadowSources, light
      );
      shadowCastPasses.push_back(shadowCastPass);
      drawPasses.push_back(shadowCastPass);
      countLights++;
    }
}

void DeferredRenderer::initShadowReceivers() {
    //
    // source pass
    //

    vector<Texture*> shadowReceiveTargets = {
        new ColorTexture(res, "positionTarget"),
        new ColorTexture(res, "normalTarget"),
        new ColorTexture(res, "diffuseTarget"),
        new ColorTexture(res, "tangentTarget"),
        new ColorTexture(res, "binormalTarget"),
        new ColorTexture(res, "normalMapTarget"),
    };

    if (useShadows) {
        shadowReceiveTargets.push_back(new ColorTexture(res, "shadowTarget"));
    }

    if (useAO) {
        shadowReceiveTargets.push_back(new DepthTexture(res, "depthTarget"));
    }

    vector<Texture*> shadowReceiveSources = {
//        Scene::Instance().getTexture("masonry-wall-normal-map","normalTexture"),
//        Scene::Instance().getTexture("masonry-wall-texture","diffuseTexture"),
    };

    if (useShadows) {
        foreach(ShadowCastPass * shadowCastPass, shadowCastPasses) {
          shadowReceiveSources.push_back(shadowCastPass->targets[0]);
        }

        // set sampler "array" via template engine
        QStringList shadowSamplers;
        foreach(ShadowCastPass * shadowCastPass, shadowCastPasses) {
          shadowSamplers <<
              QString::fromStdString(shadowCastPass->targets[0]->name);
        }
        TemplateEngine::Instance().c.insert("shadowSamplers", shadowSamplers);
        TemplateEngine::Instance().c.insert("shadowSamplerSize", shadowSamplers.size());
    }

    QList<string> tangentAttribs = QList<string> () << "uv" << "normal" << "tangent" << "bitangent";

    ShaderProgram * gatherShader = new VertFragProgram("Deferred/Gather", tangentAttribs);

    shadowReceivePass = new ShadowReceivePass(res,shadowReceiveSources,
        shadowReceiveTargets,gatherShader);

    drawPasses.push_back(shadowReceivePass);
}

void DeferredRenderer::initAo() {
    //
    // ao passes
    //

    QList<string> uv = QList<string> () << "uv";

    vector<Texture*> aoSources = {
        shadowReceivePass->getTarget("normalTarget"),
        shadowReceivePass->getTarget("depthTarget"),
        new TextureFile("noise.png", "noise"),
    };
    vector<Texture*> aoTargets = {
        new ColorTexture(res, "ao")
    };
    aoPass = new InOutPass(res, aoSources, aoTargets,
        new VertFragProgram("AO/ssao", uv));
    drawPasses.push_back(aoPass);


    vector<Texture*> mergeSources = {
        aoPass->getTarget("ao"),
        shadowReceivePass->getTarget("shadowTarget"),
    };
    vector<Texture*> mergeTargets = {
        new ColorTexture(res, "merge")
    };

    TemplateEngine::Instance().c.insert("source1", QString::fromStdString(aoPass->getTarget("ao")->name));
    TemplateEngine::Instance().c.insert("source2", QString::fromStdString(shadowReceivePass->getTarget("shadowTarget")->name));
    InOutPass * mergePass = new InOutPass(res, mergeSources, mergeTargets,
        new VertFragProgram("Deferred/Merge", uv));
    drawPasses.push_back(mergePass);

    vector<Texture*> blurHSources = {
        mergePass->getTarget("merge"),
    };
    vector<Texture*> blurHTargets = {
        new ColorTexture(res, "blurH")
    };
    InOutPass * blurHPass = new InOutPass(res, blurHSources, blurHTargets,
        new VertFragProgram("Post/blur_horizontal", uv));
    drawPasses.push_back(blurHPass);

    vector<Texture*> blurVTargets = {
        new ColorTexture(res, "finalAOTarget")
    };
    blurVPass = new InOutPass(res, blurHTargets, blurVTargets,
        new VertFragProgram("Post/blur_vertical", uv));
    drawPasses.push_back(blurVPass);
}

void DeferredRenderer::initShadingPass() {
    //
    // shading pass
    //

    vector<Texture*> shadingSources = {
        shadowReceivePass->getTarget("positionTarget"),
        shadowReceivePass->getTarget("normalTarget"),
        shadowReceivePass->getTarget("diffuseTarget"),
        shadowReceivePass->getTarget("tangentTarget"),
        shadowReceivePass->getTarget("binormalTarget"),
        shadowReceivePass->getTarget("normalMapTarget"),
        Scene::Instance().getTexture("sky", "envMap"),
//        shadowReceivePass->getTarget("uvTarget"),
    };

    if (useAO)
        shadingSources.push_back(
                    blurVPass->getTarget("finalAOTarget"));

    vector<Texture*> shadingTargets = {
        new ColorTexture(res, "finalTarget"),
//        new ColorTexture(res, "finalSpecularTarget"),
        new ColorTexture(res, "finalDiffuseTarget"),
        new ColorTexture(res, "envTarget")
    };

    TemplateEngine::Instance().c.insert("lightCount", Scene::Instance().lights.size());
//    TemplateEngine::Instance().c.insert("paralaxMap", true);
    QList<string> uv = QList<string> () << "uv";
    shadingPass = new InOutPass(res, shadingSources, shadingTargets,
        new VertFragProgram("Deferred/Shading", uv));
    drawPasses.push_back(shadingPass);

    // Init Light Uniform Buffer
    Scene::Instance().initLightBuffer(shadingPass->shader, "LightSourceBuffer");
}

vector<string> DeferredRenderer::getTargetNames() {
    vector<string> names;

    foreach (SourcePass * pass, sourcePasses) {
        foreach (Texture* tex, pass->targets) {
            names.push_back(tex->name);
        }
    }

    return names;
}

Texture* DeferredRenderer::getTargetFromName(const QString &name) {

    foreach (SourcePass * pass, sourcePasses)
        foreach (Texture* tex, pass->targets)
            if (tex->name == name.toStdString())
                return tex;

    LogError << name.toStdString() << "not found";

    return nullptr;
}

void DeferredRenderer::changePlaneSource(DebugPlane * plane, const QString &name) {
    plane->updateSource(getTargetFromName(name));
}


void DeferredRenderer::initSinkPass() {
    sinkPass = new SinkPass();

    // debug planes
//    sinkPass->debugTarget(QRectF(0.5, -1, 0.5, 0.5),
//        aoPass->getTarget("ao"));
//    sinkPass->debugTarget(QRectF(0.5, -0.5, 0.5, 0.5),
//        shadowReceivePass->getTarget("shadowTarget")
//    );
//    sinkPass->debugTarget(QRectF(0.5, 0, 0.5, 0.5),
//        blurVPass->getTarget("finalAOTarget")
//     );
//    sinkPass->debugTarget(QRectF(-1, 0, 0.5, 0.5),
//        shadowReceivePass->getTarget("normalTarget")
//     );
//    sinkPass->debugTarget(QRectF(-1, -0.5, 0.5, 0.5),
//        shadingPass->getTarget("envTarget")
//     );
//    sinkPass->debugTarget(QRectF(-1, -1, 0.5, 0.5),
//        shadingPass->getTarget("finalSpecularTarget")
//     );
//    sinkPass->debugTarget(QRectF(-1, 0.5, 0.5, 0.5),
//        shadingPass->getTarget("finalDiffuseTarget")
//     );
    sinkPass->debugTarget(QRectF(-1, -1, 2, 2),
        shadingPass->getTarget("finalTarget")
//          shadowReceivePass->getTarget("normalMapTarget")
     );
    drawPasses.push_back(sinkPass);
}
