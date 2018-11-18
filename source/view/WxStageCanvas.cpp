#include "ee2/WxStageCanvas.h"

#include <ee0/WxStagePage.h>
#include <ee0/color_config.h>
#include <ee0/EditOP.h>
#include <ee0/SubjectMgr.h>

#include <painting2/OrthoCamera.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>
#include <painting2/RenderSystem.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/RenderSystem.h>
#include <node2/CompUniquePatch.h>
#else
#include <entity2/SysRender.h>
#endif // GAME_OBJ_ECS
#include <tessellation/Painter.h>

namespace
{

const uint32_t MESSAGES[] =
{
	ee0::MSG_SET_CANVAS_DIRTY,
};

}

namespace ee2
{

WxStageCanvas::WxStageCanvas(ee0::WxStagePage* stage, ECS_WORLD_PARAM
                             const ee0::RenderContext* rc, const ee0::WindowContext* wc)
	: ee0::WxStageCanvas(stage, stage->GetImpl(), std::make_shared<pt2::OrthoCamera>(), rc, wc, HAS_2D)
	, m_stage(stage)
	ECS_WORLD_SELF_ASSIGN
{
	for (auto& msg : MESSAGES) {
		stage->GetSubjectMgr()->RegisterObserver(msg, this);
	}
}

WxStageCanvas::~WxStageCanvas()
{
	for (auto& msg : MESSAGES) {
		m_stage->GetSubjectMgr()->UnregisterObserver(msg, this);
	}
}

void WxStageCanvas::OnNotify(uint32_t msg, const ee0::VariantSet& variants)
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
	ee0::RenderContext::Reset2D(true);

	DrawBackground();
	DrawForeground();

	auto& op = m_stage->GetImpl().GetEditOP();
	if (op) {
		op->OnDraw();
	}
}

void WxStageCanvas::DrawBackground() const
{
	// draw cross

	float line_width = 1;
	if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
		line_width *= std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera)->GetScale();
	}

	const float HALF_EDGE = 50;
	tess::Painter pt;
	pt.AddLine(sm::vec2(-HALF_EDGE, 0), sm::vec2(HALF_EDGE, 0), ee0::LIGHT_GREY.ToABGR(), line_width);
	pt.AddLine(sm::vec2(0, -HALF_EDGE), sm::vec2(0, HALF_EDGE), ee0::LIGHT_GREY.ToABGR(), line_width);
	pt2::RenderSystem::DrawPainter(pt);
}

void WxStageCanvas::DrawForeground() const
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_LONG;
	var.m_val.l = ee0::WxStagePage::TRAV_DRAW;
	vars.SetVariant("type", var);

	m_stage->Traverse([&](const ee0::GameObj& obj)->bool
	{
#ifndef GAME_OBJ_ECS
		n2::RenderParams rp;
		if (obj->HasUniqueComp<n2::CompUniquePatch>())
		{
			auto patch = &obj->GetUniqueComp<n2::CompUniquePatch>();
			patch->Rewind();
			rp.SetPatch(patch);
		}

		if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
			rp.SetCamScale(std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera)->GetScale());
		}

		n2::RenderSystem::Instance()->Draw(obj, rp);
#else
		e2::RenderParams rp;
		e2::SysRender::Draw(m_world, obj, rp);
#endif // GAME_OBJ_ECS
		return true;
	}, vars);
}

}