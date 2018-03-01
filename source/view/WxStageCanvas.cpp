#include "ee2/WxStageCanvas.h"

#include <ee0/WxStagePage.h>
#include <ee0/color_config.h>
#include <ee0/EditOP.h>

#include <painting2/WindowContext.h>
#include <painting2/OrthoCamera.h>
#include <painting2/PrimitiveDraw.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <node2/RenderSystem.h>

namespace ee2
{

WxStageCanvas::WxStageCanvas(ee0::WxStagePage* stage, const ee0::RenderContext* rc, 
	                         const ee0::WindowContext* wc)
	: ee0::WxStageCanvas(stage, stage->GetImpl(), rc, wc, HAS_2D)
	, m_stage(stage)
{
	m_cam = std::make_shared<pt2::OrthoCamera>();

	stage->GetSubjectMgr().RegisterObserver(ee0::MSG_SET_CANVAS_DIRTY, this);
}

void WxStageCanvas::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	switch (msg)
	{
	case ee0::MSG_SET_CANVAS_DIRTY:
		SetDirty();
		break;
	}
}

void WxStageCanvas::OnSize(int w, int h)
{
	auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
	if (wc)
	{
		wc->SetViewport(0, 0, w, h);
		wc->SetScreen(w, h);
		wc->SetProjection(w, h);
	}
}

void WxStageCanvas::OnDrawSprites() const
{
	DrawBackground();
	DrawNodes();

	auto& op = m_stage->GetImpl().GetEditOP();
	if (op) {
		op->OnDraw();
	}
}

void WxStageCanvas::DrawBackground() const
{
	const float EDGE = 100;
	pt2::PrimitiveDraw::SetColor(ee0::LIGHT_GREY);
	pt2::PrimitiveDraw::Cross(nullptr, sm::vec2(0, 0), EDGE, EDGE);
}

void WxStageCanvas::DrawNodes() const
{
	m_stage->Traverse([&](const n0::SceneNodePtr& node)->bool {
		n2::RenderSystem::Draw(node, sm::Matrix2D());
		return true;
	});
}

}