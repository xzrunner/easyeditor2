#include "ee2/NodeSelectOP.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/DrawSelectRectState.h"
#include "ee2/BuildGroupAO.h"
#include "ee2/BreakUpAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/WxStagePage.h>
#include <ee0/MsgHelper.h>

#include <node0/SceneNode.h>
#include <node0/SceneTreeHelper.h>
#include <node2/CompComplex.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <guard/check.h>
#include <painting2/Color.h>
#include <painting2/PrimitiveDraw.h>

namespace ee2
{

NodeSelectOP::NodeSelectOP(ee0::WxStagePage& stage)
	: ee0::NodeSelectOP(stage)
	, m_draw_state_disable(false)
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");

	SetPrevEditOP(std::make_shared<CamControlOP>(*cam, stage.GetSubjectMgr()));

	m_draw_state = std::make_unique<DrawSelectRectState>(*cam, stage.GetSubjectMgr());
}

bool NodeSelectOP::OnKeyDown(int key_code)
{
	if (ee0::NodeSelectOP::OnKeyDown(key_code)) {
		return true;
	}

	// group
	if (m_stage.GetKeyState(WXK_CONTROL) && key_code == 'G') {
		BuildGroup();
	}  else if (m_stage.GetKeyState(WXK_CONTROL) && key_code == 'B') {
		BreakUpGroup();
	}

	return false;
}

bool NodeSelectOP::OnMouseLeftDown(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseLeftDown(x, y)) {
		return true;
	}

	if (!m_draw_state_disable) {
		m_draw_state->OnMousePress(x, y);
	}

	return false;
}

bool NodeSelectOP::OnMouseLeftUp(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseLeftUp(x, y)) {
		return true;
	}

	if (!m_draw_state_disable) {
		m_draw_state->OnMouseRelease(x, y);
	}

	m_draw_state_disable = false;
	m_draw_state->Clear();

	return false;
}

bool NodeSelectOP::OnMouseDrag(int x, int y)
{
	if (ee0::NodeSelectOP::OnMouseDrag(x, y)) {
		return true;
	}

	if (!m_draw_state_disable) {
		m_draw_state->OnMouseDrag(x, y);
	}

	return false;
}

bool NodeSelectOP::OnDraw() const
{
	if (ee0::NodeSelectOP::OnDraw()) {
		return true;
	}

	m_stage.GetSelection().Traverse([](const n0::NodeWithPos& nwp)->bool
	{
		CU_VEC<sm::vec2> bound;
		auto& cbb = nwp.GetNode()->GetUniqueComp<n2::CompBoundingBox>();
		cbb.GetBounding(*nwp.GetNode()).GetBoundPos(bound);

		sm::Matrix2D world_mt;
		std::vector<ee0::GameObj> path;
		n0::SceneTreeHelper::GetPathToRoot(nwp.GetRoot(), nwp.GetNodeID(), path);
		path.pop_back();
		for (auto& obj : path) {
			auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
			world_mt = world_mt * ctrans.GetTrans().GetMatrix();
		}

		for (auto& pos : bound) {
			pos = world_mt * pos;
		}

		pt2::PrimitiveDraw::SetColor(pt2::Color(255, 0, 0));
		pt2::PrimitiveDraw::Polyline(nullptr, bound, true);

		return true;
	});

	if (!m_draw_state_disable) {
		m_draw_state->OnDraw();
	}

	return false;
}

ee0::GameObj NodeSelectOP::QueryByPos(int screen_x, int screen_y) const
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*cam, screen_x, screen_y);

	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_LONG;
	var.m_val.l = ee0::WxStagePage::TRAV_QUERY;
	vars.SetVariant("type", var);

	ee0::GameObj ret = nullptr;
	m_stage.Traverse([&](const ee0::GameObj& obj)->bool
	{
		auto query = QueryByPos(obj, pos);
		if (query) 
		{
			m_draw_state_disable = true;
			ret = query;
			return false;
		}
		else
		{
			return true;
		}
	}, vars, true);

	return ret;
}

void NodeSelectOP::QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1, bool contain, 
	                           std::vector<ee0::GameObj>& result) const
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos0 = ee0::CameraHelper::TransPosScreenToProject(*cam, p0.x, p0.y);
	auto pos1 = ee0::CameraHelper::TransPosScreenToProject(*cam, p1.x, p1.y);
	sm::rect rect(pos0, pos1);

	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_LONG;
	var.m_val.l = ee0::WxStagePage::TRAV_QUERY;
	vars.SetVariant("type", var);

	m_stage.Traverse([&](const ee0::GameObj& obj)->bool {
		QueryByRect(obj, rect, contain, result);
		return true;
	}, vars);
}

ee0::GameObj NodeSelectOP::QueryByPos(const ee0::GameObj& obj, const sm::vec2& pos) const
{
	auto& cbb = obj->GetUniqueComp<n2::CompBoundingBox>();
	if (cbb.GetBounding(*obj).IsContain(pos)) {
		return obj;
	}

	if (obj->HasSharedComp<n2::CompComplex>())
	{
		auto mt = obj->GetUniqueComp<n2::CompTransform>().GetTrans().GetMatrix().Inverted();
		sm::vec2 child_pos = mt * pos;

		auto& ccomplex = obj->GetSharedComp<n2::CompComplex>();
		auto& children = ccomplex.GetAllChildren();
		for (auto& child : children) 
		{
			auto ret = QueryByPos(child, child_pos);
			if (ret) {
				return ret;
			}
		}
	}

	return nullptr;
}

void NodeSelectOP::QueryByRect(const ee0::GameObj& obj, const sm::rect& rect, 
	                           bool contain, std::vector<ee0::GameObj>& result) const
{
	auto& cbb = obj->GetUniqueComp<n2::CompBoundingBox>();
	auto& bb = cbb.GetBounding(*obj);
	if (contain && sm::is_rect_contain_rect(rect, bb.GetSize())) {
		result.push_back(obj);
	} else if (!contain && bb.IsIntersect(rect)) {
		result.push_back(obj);
	}
	m_draw_state_disable = !result.empty();
}

void NodeSelectOP::BuildGroup()
{
	auto ao = std::make_shared<BuildGroupAO>(m_stage.GetSubjectMgr(), m_stage.GetSelection());
	ao->Redo();
	m_stage.GetImpl().GetEditRecord().Add(ao);
	ee0::MsgHelper::SetEditorDirty(*m_stage.GetSubjectMgr(), true);
}

void NodeSelectOP::BreakUpGroup()
{
	auto ao = std::make_shared<BreakUpAO>(m_stage.GetSubjectMgr(), m_stage.GetSelection());
	ao->Redo();
	m_stage.GetImpl().GetEditRecord().Add(ao);
	ee0::MsgHelper::SetEditorDirty(*m_stage.GetSubjectMgr(), true);
}

}