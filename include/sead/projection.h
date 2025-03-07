#pragma once

#include "sead/matrix.h"
#include "sead/runtimetypeinfo.h"
#include "sead/graphics.h"
#include "sead/viewport.h"

namespace sead {

class Projection { // Size: 0x94
    SEAD_RTTI_BASE(Projection);

public:
    Projection();
    Projection(f32, f32, f32, f32, f32, f32);
    virtual ~Projection();

    virtual u32 getProjectionType() const = 0;
    virtual void doUpdateMatrix(Mtx44* mtx) const = 0;
    virtual void doUpdateDeviceMatrix(Mtx44*, const Mtx44&, Graphics::DevicePosture) const;
    virtual void doScreenPosToCameraPosTo(Vec3f*, const Vec3f&) const = 0;

    void updateMatrixImpl_() const;
    const Mtx44& getDeviceProjectionMatrix() const;

    mutable bool dirty;
    mutable bool deviceDirty;
    Mtx44 matrix;
    Mtx44 deviceMatrix;
    Graphics::DevicePosture devicePosture;
    f32 deviceZScale;
    f32 deviceZOffset;
};

static_assert(sizeof(Projection) == 0x94, "sead::Projection size mismatch");

class OrthoProjection : public Projection { // Size: 0xAC
    SEAD_RTTI_OVERRIDE(OrthoProjection, Projection);

public:
    OrthoProjection();
    OrthoProjection(f32 near, f32 far, const Viewport& viewport);
    virtual ~OrthoProjection();

    u32 getProjectionType() const override;
    void doUpdateMatrix(Mtx44* mtx) const override;

    void doScreenPosToCameraPosTo(Vec3f*, const Vec3f&) const override; // deleted

    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

    void setNearFarClip(f32 near, f32 far) {
        this->nearClip = near;
        this->farClip = far;
        this->dirty = true;
    }

    f32 nearClip;
    f32 farClip;
    f32 top;
    f32 bottom;
    f32 left;
    f32 right;
};

static_assert(sizeof(OrthoProjection) == 0xAC, "sead::OrthoProjection size mismatch");

class PerspectiveProjection : public Projection { // Size: 0xB8
    SEAD_RTTI_OVERRIDE(PerspectiveProjection, Projection);

public:
    PerspectiveProjection();
    PerspectiveProjection(f32 nearClip, f32 farClip, f32 fovyRad, f32 aspectRatio);
    virtual ~PerspectiveProjection();

    u32 getProjectionType() const override;
    void doUpdateMatrix(Mtx44* mtx) const override;

    void doScreenPosToCameraPosTo(Vec3f*, const Vec3f&) const override; // deleted

    void set(f32 nearClip, f32 farClip, f32 fov, f32 aspectRatio);

    void setFovx(f32 fovx);
    void setFovy_(f32 fovy);

    f32 nearClip;
    f32 farClip;
    f32 angle;
    f32 fovySin;
    f32 fovyCos;
    f32 fovyTan;
    f32 aspectRatio;
    Vec2f offset;
};

static_assert(sizeof(PerspectiveProjection) == 0xB8, "sead::PerspectiveProjection size mismatch");

class FrustumProjection : public Projection { // Size: 0xAC
    SEAD_RTTI_OVERRIDE(FrustumProjection, Projection);

public:
    FrustumProjection(f32 nearClip, f32 farClip, f32 top, f32 bottom, f32 left, f32 right);
    virtual ~FrustumProjection();

    u32 getProjectionType() const override;
    void doUpdateMatrix(Mtx44* mtx) const override;

    void doScreenPosToCameraPosTo(Vec3f*, const Vec3f&) const override; // deleted

    f32 getFovy();

    f32 getAspect();

    f32 getOffsetX();
    f32 getOffsetY();
    void getOffset(Vec2f& out);

    inline Vec2f getOffset() {
        Vec2f out;
        this->getOffset(out);
        return out;
    }

    f32 nearClip;
    f32 farClip;
    f32 top;
    f32 bottom;
    f32 left;
    f32 right;
};

static_assert(sizeof(FrustumProjection) == 0xAC, "sead::FrustumProjection size mismatch");

}
