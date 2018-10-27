#include "ee2/NodeSelectOP.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/DrawSelectRectState.h"
#include "ee2/BuildGroupAO.h"
#include "ee2/BreakUpAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/WxStagePage.h>
#include <ee0/MsgHelper.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/SceneTreeHelper.h>
#include <node0/CompComplex.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/CompBoundingBox.h>
#endif // GAME_OBJ_ECS
#include <SM_Test.h>
#include <guard/check.h>
#include <tessellation/Painter.h>
#include <painting2/RenderSystem.h>
#include <painting2/OrthoCamera.h>

namespace ee2
{

NodeSelectOP::NodeSelectOP(const std::shared_ptr<pt0::Camera>& camera,
	                       ECS_WORLD_PARAM
	                       ee0::WxStagePage& stage)
	: ee0::NodeSelectOP(camera, stage)
	ECS_WORLD_SELF_ASSIGN
	, m_draw_state_disable(false)
{
	SetPrevEditOP(std::make_shared<CamControlOP>(camera, stage.GetSubjectMgr()));

	m_draw_state = std::make_unique<DrawSelectRectState>(camera, stage.GetSubjectMgr());
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

	m_stage.GetSelection().Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
		CU_VEC<sm::vec2> bound;
#ifndef GAME_OBJ_ECS
		auto& cbb = opw.GetNode()->GetUniqueComp<n2::CompBoundingBox>();
		cbb.GetBounding(*opw.GetNode()).GetBoundPos(bound);
#else
		auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(opw);
		cbb.GetBoundPos(bound);
#endif // GAME_OBJ_ECS

#ifndef GAME_OBJ_ECS
		sm::Matrix2D world_mt;
		std::vector<ee0::GameObj> path;
		n0::SceneTreeHelper::GetPathToRoot(opw.GetRoot(), opw.GetNodeID(), path);
		path.pop_back();
		for (auto& obj : path) {
			auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
			world_mt = world_mt * ctrans.GetTrans().GetMatrix();
		}

		for (auto& pos : bound) {
			pos = world_mt * pos;
		}
#endif // GAME_OBJ_ECS

		float line_width = 1;
		if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
			line_width *= std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera)->GetScale();
		}

		tess::Painter pt;
		pt.AddPolygon(bound.data(), bound.size(), 0xff0000ff, line_width);
		pt2::RenderSystem::DrawPainter(pt);

		return true;
	});

	if (!m_draw_state_disable) {
		m_draw_state->OnDraw();
	}

	return false;
}

ee0::GameObj NodeSelectOP::QueryByPos(int screen_x, int screen_y) const
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, screen_x, screen_y);

	ee0::VariantSet vars;
	ee0::Variant var;
	var.m_type = ee0::VT_LONG;
	var.m_val.l = ee0::WxStagePage::TRAV_QUERY;
	vars.SetVariant("type", var);

#ifndef GAME_OBJ_ECS
	ee0::GameObj ret = nullptr;
#else
	ee0::GameObj ret;
#endif // GAME_OBJ_ECS
	m_stage.Traverse([&](const ee0::GameObj& obj)->bool
	{
		auto query = QueryByPos(obj, pos);
		if (GAME_OBJ_VALID(query))
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
	auto pos0 = ee0::CameraHelper::TransPosScreenToProject(*m_camera, p0.x, p0.y);
	auto pos1 = ee0::CameraHelper::TransPosScreenToProject(*m_camera, p1.x, p1.y);
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
#ifndef GAME_OBJ_ECS
	auto& cbb = obj->GetUniqueComp<n2::CompBoundingBox>();
	if (cbb.GetBounding(*obj).IsContain(pos)) {
		return obj;
	}
#else
	auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(obj);
	if (cbb.IsContain(pos)) {
		return obj;
	}
#endif // GAME_OBJ_ECS

#ifndef GAME_OBJ_ECS
	if (obj->HasSharedComp<n0::CompComplex>())
	{
		auto mt = obj->GetUniqueComp<n2::CompTransform>().GetTrans().GetMatrix().Inverted();
		sm::vec2 child_pos = mt * pos;

		auto& ccomplex = obj->GetSharedComp<n0::CompComplex>();
		auto& children = ccomplex.GetAllChildren();
		for (auto& child : children)
		{
			auto ret = QueryByPos(child, child_pos);
			if (ret) {
				return ret;
			}
		}
	}
#endif // GAME_OBJ_ECS

	return GAME_OBJ_NULL;
}

void NodeSelectOP::QueryByRect(const ee0::GameObj& obj, const sm::rect& rect,
	                           bool contain, std::vector<ee0::GameObj>& result) const
{
#ifndef GAME_OBJ_ECS
	auto& cbb = obj->GetUniqueComp<n2::CompBoundingBox>();
	auto& bb = cbb.GetBounding(*obj);
	if (contain && sm::is_rect_contain_rect(rect, bb.GetSize())) {
		result.push_back(obj);
	} else if (!contain && bb.IsIntersect(rect)) {
		result.push_back(obj);
	}
#else
	auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(obj);
	if (contain && sm::is_rect_contain_rect(rect, cbb.rect)) {
		result.push_back(obj);
	}
	else if (!contain && cbb.IsIntersect(rect)) {
		result.push_back(obj);
	}
#endif // GAME_OBJ_ECS

	m_draw_state_disable = !result.empty();
}

void NodeSelectOP::BuildGroup()
{
	auto& sub_mgr = m_stage.GetSubjectMgr();

	auto ao = std::make_shared<BuildGroupAO>(
		sub_mgr, ECS_WORLD_SELF_VAR m_stage.GetSelection()
	);
	ao->Redo();
	ee0::MsgHelper::AddAtomicOP(*sub_mgr, ao);

	ee0::MsgHelper::SetEditorDirty(*sub_mgr, true);
}

void NodeSelectOP::BreakUpGroup()
{
	auto& sub_mgr = m_stage.GetSubjectMgr();

	auto ao = std::make_shared<BreakUpAO>(
		sub_mgr, ECS_WORLD_SELF_VAR m_stage.GetSelection()
	);
	ao->Redo();
	ee0::MsgHelper::AddAtomicOP(*sub_mgr, ao);

	ee0::MsgHelper::SetEditorDirty(*sub_mgr, true);
}

}