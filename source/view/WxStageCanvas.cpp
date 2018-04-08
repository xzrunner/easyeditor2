#include "ee2/WxStageCanvas.h"

#include <ee0/WxStagePage.h>
#include <ee0/color_config.h>
#include <ee0/EditOP.h>
#include <ee0/SubjectMgr.h>

#include <painting2/OrthoCamera.h>
#include <painting2/PrimitiveDraw.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>
#include <node0/SceneNode.h>
#include <node2/RenderSystem.h>
#include <node2/CompUniquePatch.h>

namespace
{

const uint32_t MESSAGES[] =
{
	ee0::MSG_SET_CANVAS_DIRTY,
};

}

namespace ee2
{

WxStageCanvas::WxStageCanvas(ee0::WxStagePage* stage, const ee0::RenderContext* rc, 
	                         const ee0::WindowContext* wc)
	: ee0::WxStageCanvas(stage, stage->GetImpl(), rc, wc, HAS_2D)
	, m_stage(stage)
{
	m_cam = std::make_shared<pt2::OrthoCamera>();

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
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_LONG;
	var.m_val.l = ee0::WxStagePage::TRAV_DRAW;
	vars.SetVariant("type", var);

	m_stage->Traverse([&](const ee0::GameObj& obj)->bool 
	{
		n2::RenderParams rp;
		if (obj->HasUniqueComp<n2::CompUniquePatch>()) 
		{
			auto patch = &obj->GetUniqueComp<n2::CompUniquePatch>();
			patch->Rewind();
			rp.SetPatch(patch);
		}

		n2::RenderSystem::Draw(obj, rp);

		return true;
	}, vars);
}

}