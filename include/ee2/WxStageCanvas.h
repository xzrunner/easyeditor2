#pragma once

#include <ee0/WxStageCanvas.h>
#include <ee0/Observer.h>

#include <memory>

namespace ee0 { class WxStagePage; }
namespace pt2 { class Camera; }

namespace ee2
{

class WxStageCanvas : public ee0::WxStageCanvas, public ee0::Observer
{
public:
	WxStageCanvas(ee0::WxStagePage* stage, const ee0::RenderContext* rc = nullptr, 
		const ee0::WindowContext* wc = nullptr);

	virtual void OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants) override;

	std::shared_ptr<pt2::Camera>& GetCamera() { return m_cam; }
	const std::shared_ptr<pt2::Camera>& GetCamera() const { return m_cam; }

protected:
	virtual void OnSize(int w, int h) override;
	virtual void OnDrawSprites() const override;

	virtual void DrawBackground() const;

private:
	void DrawNodes() const;

private:
	ee0::WxStagePage* m_stage;

	std::shared_ptr<pt2::Camera> m_cam = nullptr;

}; // WxStageCanvas

}