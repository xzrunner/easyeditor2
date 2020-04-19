#include "ee2/WxStageCanvas.h"
#include "ee2/Utility.h"

#include <ee0/WxStagePage.h>
#include <ee0/color_config.h>
#include <ee0/EditOP.h>
#include <ee0/SubjectMgr.h>

#include <unirender2/Context.h>
#include <unirender2/RenderState.h>
#include <painting2/OrthoCamera.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>
#include <painting2/RenderSystem.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/RenderSystem.h>
#include <node2/CompUniquePatch.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#else
#include <entity2/SysRender.h>
#endif // GAME_OBJ_ECS
#include <tessellation/Painter.h>
#include <SM_Calc.h>

namespace
{

const uint32_t MESSAGES[] =
{
	ee0::MSG_SET_CANVAS_DIRTY,
};

}

namespace ee2
{

WxStageCanvas::WxStageCanvas(const ur2::Device& dev, ee0::WxStagePage* stage, ECS_WORLD_PARAM
                             const ee0::RenderContext* rc, const ee0::WindowContext* wc)
	: ee0::WxStageCanvas(dev, stage, stage->GetImpl(), std::make_shared<pt2::OrthoCamera>(), rc, wc, HAS_2D)
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

void WxStageCanvas::OnDrawSprites() const
{
	ee0::RenderContext::Reset2D(true);

	DrawBackground();
	DrawForeground();

	auto& op = m_stage->GetImpl().GetEditOP();
	if (op) {
		op->OnDraw(m_dev, *GetRenderContext().ur_ctx);
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

	pt2::RenderSystem::DrawPainter(m_dev,
        *GetRenderContext().ur_ctx, Utility::GetRenderState2D(), pt);
}

void WxStageCanvas::DrawForeground() const
{
	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_LONG;
	var.m_val.l = ee0::WxStagePage::TRAV_DRAW;
	vars.SetVariant("type", var);

    ur2::RenderState rs = Utility::GetRenderState2D();

    auto screen_region = CalcScreenRegion();
	m_stage->Traverse([&](const ee0::GameObj& obj)->bool
	{
#ifndef GAME_OBJ_ECS
        if (screen_region.IsValid() && ScreenCullingTest(screen_region, obj)) {
            return true;
        }

		n2::RenderParams rp;
        rp.SetScreenRegion(screen_region);

		if (obj->HasUniqueComp<n2::CompUniquePatch>())
		{
			auto patch = &obj->GetUniqueComp<n2::CompUniquePatch>();
			patch->Rewind();
			rp.SetPatch(patch);
		}

		if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
			rp.SetCamScale(std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera)->GetScale());
		}

		n2::RenderSystem::Instance()->Draw(m_dev, *GetRenderContext().ur_ctx, rs, obj, rp);
#else
		e2::RenderParams rp;
		e2::SysRender::Draw(m_world, obj, rp);
#endif // GAME_OBJ_ECS
		return true;
	}, vars);
}

sm::rect WxStageCanvas::CalcScreenRegion() const
{
    sm::rect r;
    if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>())
    {
        auto o_cam = std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera);

        const float w = m_screen_sz.x * o_cam->GetScale();
        const float h = m_screen_sz.y * o_cam->GetScale();

        r.Build(w, h);
        r.Translate(o_cam->GetPosition());
    }
    return r;
}

bool WxStageCanvas::ScreenCullingTest(const sm::rect& screen, const ee0::GameObj& obj) const
{
    assert(screen.IsValid());
    auto& cbb = obj->GetUniqueComp<n2::CompBoundingBox>();

    auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
    auto& mt = ctrans.GetTrans().GetMatrix();

    auto& sz = cbb.GetSize();
    const sm::vec2 min = mt * sm::vec2(sz.xmin, sz.ymin);
    const sm::vec2 max = mt * sm::vec2(sz.xmax, sz.ymax);

    return !sm::is_rect_intersect_rect(screen, sm::rect(min, max));
}

}