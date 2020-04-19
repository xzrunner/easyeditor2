#pragma once

#include <ee0/WxStageCanvas.h>
#include <ee0/Observer.h>
#include <ee0/Config.h>
#include <ee0/GameObj.h>

#include <SM_Rect.h>

#include <memory>

namespace ee0 { class WxStagePage; }
ECS_WORLD_DECL

namespace ee2
{

class WxStageCanvas : public ee0::WxStageCanvas, public ee0::Observer
{
public:
	WxStageCanvas(const ur2::Device& dev, ee0::WxStagePage* stage, ECS_WORLD_PARAM
		const ee0::RenderContext* rc = nullptr, const ee0::WindowContext* wc = nullptr);
	virtual ~WxStageCanvas();

	virtual void OnNotify(uint32_t msg, const ee0::VariantSet& variants) override;

protected:
	virtual void OnDrawSprites() const override;

	virtual void DrawBackground() const;
	virtual void DrawForeground() const;

protected:
    sm::rect CalcScreenRegion() const;
    bool ScreenCullingTest(const sm::rect& screen, const ee0::GameObj& obj) const;

protected:
	ee0::WxStagePage* m_stage;

	ECS_WORLD_SELF_DEF

}; // WxStageCanvas

}