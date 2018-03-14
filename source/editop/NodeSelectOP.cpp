#include "ee2/NodeSelectOP.h"
#include "ee2/WxStageCanvas.h"
#include "ee2/DrawSelectRectState.h"
#include "ee2/BuildGroupAO.h"
#include "ee2/BreakUpAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/WxStagePage.h>
#include <ee0/MsgHelper.h>

#include <node0/SceneNode.h>
#include <node0/CompAsset.h>
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

	m_stage.GetNodeSelection().Traverse([](const n0::NodeWithPos& nwp)->bool
	{
		CU_VEC<sm::vec2> bound;
		auto& cbb = nwp.node->GetUniqueComp<n2::CompBoundingBox>();
		cbb.GetBounding(*nwp.node).GetBoundPos(bound);

		sm::Matrix2D world_mt;
		if (nwp.root && nwp.node_id != 0)
		{
			size_t curr_id = 0;
			auto curr_node = nwp.root;
			auto& ctrans = curr_node->GetUniqueComp<n2::CompTransform>();
			world_mt = world_mt * ctrans.GetTrans().GetMatrix();
			while (curr_id != nwp.node_id)
			{
				auto& casset = curr_node->GetSharedComp<n0::CompAsset>();
				curr_id += 1;
				GD_ASSERT(nwp.node_id >= curr_id && nwp.node_id < curr_id + casset.GetNodeCount(), "err id");
				casset.Traverse([&](const n0::SceneNodePtr& node)->bool
				{
					auto& casset = node->GetSharedComp<n0::CompAsset>();
					if (nwp.node_id == curr_id)
					{
						curr_node = node;
						return false;
					}
					else if (nwp.node_id < curr_id + casset.GetNodeCount()) 
					{
						curr_node = node;
						auto& ctrans = curr_node->GetUniqueComp<n2::CompTransform>();
						world_mt = world_mt * ctrans.GetTrans().GetMatrix();
						return false;
					} 
					else 
					{
						curr_id += casset.GetNodeCount();
						return true;
					}
				});
			}
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

n0::SceneNodePtr NodeSelectOP::QueryByPos(int screen_x, int screen_y) const
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*cam, screen_x, screen_y);

	n0::SceneNodePtr ret = nullptr;
	m_stage.Traverse([&](const n0::SceneNodePtr& node)->bool
	{
		auto query = QueryByPos(node, pos);
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
	}, ee0::VariantSet(), true);

	return ret;
}

void NodeSelectOP::QueryByRect(const sm::ivec2& p0, const sm::ivec2& p1, bool contain, 
	                           std::vector<n0::SceneNodePtr>& result) const
{
	auto cam = std::dynamic_pointer_cast<WxStageCanvas>(m_stage.GetImpl().GetCanvas())->GetCamera();
	GD_ASSERT(cam, "null cam");
	auto pos0 = ee0::CameraHelper::TransPosScreenToProject(*cam, p0.x, p0.y);
	auto pos1 = ee0::CameraHelper::TransPosScreenToProject(*cam, p1.x, p1.y);
	sm::rect rect(pos0, pos1);

	m_stage.Traverse([&](const n0::SceneNodePtr& node)->bool {
		QueryByRect(node, rect, contain, result);
		return true;
	});
}

n0::SceneNodePtr NodeSelectOP::QueryByPos(const n0::SceneNodePtr& node, const sm::vec2& pos) const
{
	auto& cbb = node->GetUniqueComp<n2::CompBoundingBox>();
	if (cbb.GetBounding(*node).IsContain(pos)) {
		return node;
	}

	if (node->HasSharedComp<n2::CompComplex>())
	{
		auto mt = node->GetUniqueComp<n2::CompTransform>().GetTrans().GetMatrix().Inverted();
		sm::vec2 child_pos = mt * pos;

		auto& ccomplex = node->GetSharedComp<n2::CompComplex>();
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

void NodeSelectOP::QueryByRect(const n0::SceneNodePtr& node, const sm::rect& rect, 
	                           bool contain, std::vector<n0::SceneNodePtr>& result) const
{
	auto& cbb = node->GetUniqueComp<n2::CompBoundingBox>();
	auto& bb = cbb.GetBounding(*node);
	if (contain && sm::is_rect_contain_rect(rect, bb.GetSize())) {
		result.push_back(node);
	} else if (!contain && bb.IsIntersect(rect)) {
		result.push_back(node);
	}

	if (node->HasSharedComp<n2::CompComplex>())
	{
		// todo
		//	auto& mt = node->GetUniqueComp<n2::CompTransform>().GetTransformMat();

		auto& ccomplex = node->GetSharedComp<n2::CompComplex>();
		auto& children = ccomplex.GetAllChildren();
		for (auto& child : children) {
			QueryByRect(child, rect, contain, result);
		}
	}

	m_draw_state_disable = !result.empty();
}

void NodeSelectOP::BuildGroup()
{
	auto ao = std::make_shared<BuildGroupAO>(m_stage.GetSubjectMgr(), m_stage.GetNodeSelection());
	ao->Redo();
	m_stage.GetImpl().GetEditRecord().Add(ao);
	ee0::MsgHelper::SetEditorDirty(m_stage.GetSubjectMgr(), true);
}

void NodeSelectOP::BreakUpGroup()
{
	auto ao = std::make_shared<BreakUpAO>(m_stage.GetSubjectMgr(), m_stage.GetNodeSelection());
	ao->Redo();
	m_stage.GetImpl().GetEditRecord().Add(ao);
	ee0::MsgHelper::SetEditorDirty(m_stage.GetSubjectMgr(), true);
}

}