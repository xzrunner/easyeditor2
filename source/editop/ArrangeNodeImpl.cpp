#include "ee2/ArrangeNodeImpl.h"

#include "ee2/CopyPasteNodeState.h"
#include "ee2/MoveNodeState.h"
#include "ee2/OffsetNodeState.h"
#include "ee2/RotateNodeState.h"
#include "ee2/ScaleNodeState.h"
#include "ee2/ShearNodeState.h"
#include "ee2/TranslateNodeState.h"

#include "ee2/CombineAO.h"
#include "ee2/TranslateNodeAO.h"
#include "ee2/RotateNodeAO.h"
#include "ee2/ShearNodeAO.h"
#include "ee2/ScaleNodeAO.h"
#include "ee2/DeleteNodeAO.h"

#include <ee0/KeysState.h>
#include <ee0/KeyType.h>
#include <ee0/CameraHelper.h>
#include <ee0/SubjectMgr.h>
#include <ee0/MsgHelper.h>
#include <ee0/EditRecord.h>

#include <guard/check.h>
#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#include <painting2/PrimitiveDraw.h>
#include <painting2/Color.h>

namespace ee2
{

ArrangeNodeImpl::ArrangeNodeImpl(pt2::Camera& cam, 
	                             ee0::EditRecord& record,
	                             ee0::SubjectMgr& sub_mgr,
	                             ee0::SelectionSet<n0::NodeWithPos>& selection, 
	                             ee0::NodeContainer& nodes,
	                             const ee0::KeysState& key_state,
	                             const ArrangeNodeCfg& cfg)
	: m_cam(cam)
	, m_record(record)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
	, m_key_state(key_state)
	, m_cfg(cfg)
	, m_align(nodes)
	, m_ctrl_node_radius(ArrangeNodeCfg::CTRL_NODE_RADIUS)
//	, m_popup(wnd, stage, sprites_impl, sprites_impl->GetSpriteSelection())
{
	m_align.SetOpen(cfg.is_auto_align_open);

	m_left_down_pos.MakeInvalid();
	m_right_down_pos.MakeInvalid();
}

bool ArrangeNodeImpl::OnKeyDown(int keycode)
{
	if (m_key_state.GetKeyState(WXK_SHIFT) && OnSpriteShortcutKey(keycode)) {
		return true;
	}

	bool ret = false;
	switch (keycode)
	{
	case WXK_DELETE:
		ret = true;
		OnDeleteKeyDown();
		break;
	case WXK_PAGEUP:
		ret = true;
		UpOneLayer();
		break;
	case WXK_PAGEDOWN:
		ret = true;
		DownOneLayer();
		break;
	case '[':
		ret = true;
		DownOneLayer();
		break;
	case ']':
		ret = true;
		UpOneLayer();
		break;
	case 'm' : case 'M':
		ret = true;
		m_op_state = std::make_unique<MoveNodeState>(m_cam, m_selection);
		break;
	case WXK_SPACE:
		ret = true;
		OnSpaceKeyDown();
		break;

// 		// for debug
// 	case 'O':
// 		ShaderMgr::Instance()->SetBufferData(true);
// 		break;
// 	case 'C':
// 		ShaderMgr::Instance()->SetBufferData(false);
// 		break;
	}

	return ret;
}

void ArrangeNodeImpl::OnKeyUp(int key_code)
{
	// todo update panel
	//if (m_property_panel)
	//{
	//	m_property_panel->EnablePropertyGrid(true);
	//	m_property_panel->UpdatePropertyGrid();
	//}
}

void ArrangeNodeImpl::OnChar(int keycode)
{
	switch (keycode)
	{
	case 'a':
		OnDirectionKeyDown(ee0::KEY_LEFT);
		break;
	case 'd':
		OnDirectionKeyDown(ee0::KEY_RIGHT);
		break;
	case 's':
		OnDirectionKeyDown(ee0::KEY_DOWN);
		break;
	case 'w':
		OnDirectionKeyDown(ee0::KEY_UP);
		break;
	case 'A':
		OnDirectionKeyDown(ee0::KEY_LEFT | ee0::KEY_SHIFT);
		break;
	case 'D':
		OnDirectionKeyDown(ee0::KEY_RIGHT | ee0::KEY_SHIFT);
		break;
	case 'S':
		OnDirectionKeyDown(ee0::KEY_DOWN | ee0::KEY_SHIFT);
		break;
	case 'W':
		OnDirectionKeyDown(ee0::KEY_UP | ee0::KEY_SHIFT);
		break;
	}
}

void ArrangeNodeImpl::OnMouseLeftDown(int x, int y)
{
	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	m_left_down_pos = pos;

	m_align.SetInvisible();

	if (m_selection.IsEmpty()) 
	{
		if (m_op_state) {
			m_op_state->OnMousePress(x, y);
		}
		return;
	}

	// copy & paste
	if (m_key_state.GetKeyState(WXK_ALT)) {
		m_op_state = std::make_unique<CopyPasteNodeState>(m_cam, m_sub_mgr, m_selection);
	}

	n0::SceneNodePtr selected = nullptr;
	if (m_selection.Size() == 1)
	{
		m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
			selected = nwp.node;
			return false;
		});
	}
	if (!selected) {
		if (m_op_state) {
			m_op_state->OnMousePress(x, y);
		}
		return;
	}

	// offset
	if (m_cfg.is_offset_open)
	{
		sm::vec2 offset = GetNodeOffset(selected);
		if (sm::dis_pos_to_pos(offset, pos) < m_ctrl_node_radius) {
			m_op_state = std::make_unique<OffsetNodeState>(m_cam, m_record, m_sub_mgr, selected);
			return;
		}
	}

	// scale
	if (m_cfg.is_deform_open && !m_key_state.GetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrlNodes[8];
		NodeCtrlPoint::GetNodeCtrlPoints(*selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (sm::dis_pos_to_pos(ctrlNodes[i], pos) < m_ctrl_node_radius)
			{
				NodeCtrlPoint::Node cn;
				cn.pos = ctrlNodes[i];
				cn.type = NodeCtrlPoint::Type(i);
				m_op_state = std::make_unique<ScaleNodeState>(m_cam, m_record, m_sub_mgr, selected, cn);
				return;
			}
		}
	}

	//// perspective
	//if (m_cfg.is_deform_open && m_key_state.GetKeyState(WXK_SHIFT))
	//{
	//	sm::vec2 ctrl_point[4];
	//	NodeCtrlPoint::GetNodeCtrlPointsExt(*selected, ctrl_point);
	//	for (int i = 0; i < 4; ++i) {
	//		if (sm::dis_pos_to_pos(ctrl_point[i], pos) < m_ctrl_node_radius) {
	//			NodeCtrlPoint::Node cn;
	//			cn.pos = ctrl_point[i];
	//			cn.type = NodeCtrlPoint::Type(i);
	//			ChangeOPState(CreatePerspectiveState(selected, cn));
	//			return;
	//		}
	//	}
	//}

	// translate
	m_op_state = std::make_unique<TranslateNodeState>(m_cam, m_record, m_sub_mgr, m_selection, pos);

	m_op_state->OnMousePress(x, y);
}

void ArrangeNodeImpl::OnMouseLeftUp(int x, int y)
{
	if (m_op_state) 
	{
		m_op_state->OnMouseRelease(x, y);
		m_op_state = nullptr;
	}

	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	if (!m_selection.IsEmpty()) {
		m_op_state = std::make_unique<TranslateNodeState>(m_cam, m_record, m_sub_mgr, m_selection, pos);
	}

	if (m_cfg.is_auto_align_open &&
		m_align.IsOpen() &&
		!m_selection.IsEmpty() &&
		m_left_down_pos != pos)
	{
		std::vector<n0::SceneNodePtr> nodes;
		m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
			nodes.push_back(nwp.node);
			return false;
		});
		m_align.Align(nodes);

		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}

	// todo update panel
	//if (m_property_panel)
	//{
	//	m_property_panel->EnablePropertyGrid(true);
	//	m_property_panel->UpdatePropertyGrid();
	//}

	m_left_down_pos.MakeInvalid();
}

void ArrangeNodeImpl::OnMouseRightDown(int x, int y)
{
	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	m_right_down_pos = pos;

	n0::SceneNodePtr selected = nullptr;
	if (m_selection.Size() == 1)
	{
		m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
			selected = nwp.node;
			return false;
		});
	}
	if (!selected) {
		return;
	}

	// shear
	if (m_cfg.is_deform_open)
	{
		sm::vec2 ctrlNodes[8];
		NodeCtrlPoint::GetNodeCtrlPoints(*selected, ctrlNodes);
		for (int i = 0; i < 8; ++i)
		{
			if (sm::dis_pos_to_pos(ctrlNodes[i], pos) < m_ctrl_node_radius)
			{
				NodeCtrlPoint::Node cn;
				cn.pos = ctrlNodes[i];
				cn.type = NodeCtrlPoint::Type(i);
				m_op_state = std::make_unique<ShearNodeState>(m_cam, selected, cn);
				return;
			}
		}
	}

	// rotate
	if (m_cfg.is_rotate_open) {
		m_op_state = std::make_unique<RotateNodeState>(m_cam, m_record, m_sub_mgr, m_selection, pos);
	}
}

void ArrangeNodeImpl::OnMouseRightUp(int x, int y)
{
	if (!m_right_down_pos.IsValid()) {
		return;
	}

	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	if (pos == m_right_down_pos)
	{
		wxMenu menu;
		SetRightPopupMenu(menu, x, y);
		// todo popup menu
//		m_stage->PopupMenu(&menu, x, y);
	}
	else if (m_op_state)
	{
		m_op_state->OnMouseRelease(x, y);
		m_op_state = nullptr;
	}

	// todo update panel
	//if (m_property_panel)
	//{
	//	m_property_panel->EnablePropertyGrid(true);
	//	m_property_panel->UpdatePropertyGrid();
	//}

	m_right_down_pos.MakeInvalid();
}

void ArrangeNodeImpl::OnMouseMove(int x, int y)
{
	// todo update panel
	//if (m_op_state && m_op_state->OnMouseMove(x, y))
	//{
	//	if (m_property_panel) {
	//		m_property_panel->EnablePropertyGrid(false);
	//	}
	//}
}

void ArrangeNodeImpl::OnMouseDrag(int x, int y)
{
// 	if (!m_left_down_pos.IsValid() && !m_right_down_pos.IsValid()) {
// 		return;
// 	}

	if (m_op_state && m_op_state->OnMouseDrag(x, y))
	{
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		// todo update panel
		//if (m_property_panel) {
		//	m_property_panel->EnablePropertyGrid(false);
		//}
	}
}

void ArrangeNodeImpl::OnPopMenuSelected(int type)
{
	// todo popup menu
	//m_popup.OnRightPopupMenu(type);
}

void ArrangeNodeImpl::OnDraw(float cam_scale) const
{
	m_ctrl_node_radius = ArrangeNodeCfg::CTRL_NODE_RADIUS * cam_scale;
	if ((m_cfg.is_deform_open || m_cfg.is_offset_open) && m_selection.Size() == 1)
	{
		n0::SceneNodePtr selected = nullptr;
		m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
			selected = nwp.node;
			return false;
		});

		GD_ASSERT(selected, "null selected");
		auto& cbb = selected->GetUniqueComp<n2::CompBoundingBox>();
		auto& ctrans = selected->GetUniqueComp<n2::CompTransform>();
		auto& sz = cbb.GetSize();
		float w = sz.Width() * fabs(ctrans.GetTrans().GetScale().x);
		float h = sz.Height() * fabs(ctrans.GetTrans().GetScale().y);
		float max_e = std::max(w, h);
		if (max_e / cam_scale < 100) 
		{
			m_ctrl_node_radius = 0;
		} 
		else if (m_ctrl_node_radius > max_e * 0.1f) 
		{
			m_ctrl_node_radius = 0;
		} 
		else 
		{
			if (m_cfg.is_deform_open)
			{
				if (m_key_state.GetKeyState(WXK_SHIFT)) 
				{
					//sm::vec2 ctrl_nodes[4];
					//NodeCtrlPoint::GetNodeCtrlPointsExt(*selected, ctrl_nodes);
					//for (int i = 0; i < 4; ++i) {
					//	pt2::PrimitiveDraw::SetColor(pt2::Color(51, 204, 51));
					//	pt2::PrimitiveDraw::Circle(nullptr, ctrl_nodes[i], m_ctrl_node_radius, true);
					//}
				}
				else
				{
					sm::vec2 ctrl_nodes[8];
					NodeCtrlPoint::GetNodeCtrlPoints(*selected, ctrl_nodes);
					for (int i = 0; i < 4; ++i) {
						pt2::PrimitiveDraw::SetColor(pt2::Color(51, 204, 51));
						pt2::PrimitiveDraw::Circle(nullptr, ctrl_nodes[i], m_ctrl_node_radius, false);
					}
					for (int i = 4; i < 8; ++i) {
						pt2::PrimitiveDraw::SetColor(pt2::Color(51, 204, 51));
						pt2::PrimitiveDraw::Circle(nullptr, ctrl_nodes[i], m_ctrl_node_radius, true);
					}
				}
			}

			if (m_cfg.is_offset_open)
			{
				sm::vec2 offset = GetNodeOffset(selected);
				pt2::PrimitiveDraw::SetColor(pt2::Color(204, 51, 51));
				pt2::PrimitiveDraw::Circle(nullptr, offset, m_ctrl_node_radius, true);
			}
		}
	}

	m_align.Draw();
}

void ArrangeNodeImpl::Clear()
{
}

n0::SceneNodePtr ArrangeNodeImpl::QueryEditedNode(const sm::vec2& pos) const
{
	n0::SceneNodePtr selected = nullptr;
	if (m_cfg.is_deform_open && m_selection.Size() == 1)
	{
		m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
			selected = nwp.node;
			return false;
		});
	}
	if (!selected) {
		return nullptr;
	}

	if (m_cfg.is_offset_open)
	{
		sm::vec2 offset = GetNodeOffset(selected);
		if (sm::dis_pos_to_pos(offset, pos) < m_ctrl_node_radius) {
			return selected;
		}
	}

	if (m_cfg.is_deform_open && !m_key_state.GetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrl_nodes[8];
		NodeCtrlPoint::GetNodeCtrlPoints(*selected, ctrl_nodes);
		for (int i = 0; i < 8; ++i) {
			if (sm::dis_pos_to_pos(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
				return selected;
			}
		}
	}

	//if (m_cfg.is_deform_open && m_key_state.GetKeyState(WXK_SHIFT))
	//{
	//	sm::vec2 ctrl_nodes[4];
	//	NodeCtrlPoint::GetNodeCtrlPointsExt(*selected, ctrl_nodes);
	//	for (int i = 0; i < 4; ++i) {
	//		if (sm::dis_pos_to_pos(ctrl_nodes[i], pos) < m_ctrl_node_radius) {
	//			return selected;
	//		}
	//	}
	//}

	return NULL;
}

bool ArrangeNodeImpl::IsSelectionEmpty() const
{
	return m_selection.IsEmpty();
}

void ArrangeNodeImpl::OnDirectionKeyDown(int type)
{
	if (!m_op_state) return;

	bool dirty = m_op_state->OnDirectionKeyDown(type);
	if (dirty)
	{
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		// todo update panel
		//if (m_property_panel) {
		//	m_property_panel->EnablePropertyGrid(false);
		//}
	}
}

void ArrangeNodeImpl::OnSpaceKeyDown()
{
	auto comb = std::make_shared<CombineAO>();

	m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		auto& ctrans = nwp.node->GetUniqueComp<n2::CompTransform>();

		// record
		std::vector<n0::SceneNodePtr> nodes;
		nodes.push_back(nwp.node);
		comb->Add(std::make_shared<TranslateNodeAO>(m_sub_mgr, nwp.node, - ctrans.GetTrans().GetPosition()));
		comb->Add(std::make_shared<RotateNodeAO>(m_sub_mgr, nodes, - ctrans.GetTrans().GetAngle()));
		comb->Add(std::make_shared<ScaleNodeAO>(m_sub_mgr, nwp.node, sm::vec2(1, 1), ctrans.GetTrans().GetScale()));
		comb->Add(std::make_shared<ShearNodeAO>(m_sub_mgr, nwp.node, sm::vec2(0, 0), ctrans.GetTrans().GetShear()));

		ctrans.SetPosition(*nwp.node, sm::vec2(0, 0));
		ctrans.SetAngle(*nwp.node, 0);
		ctrans.SetShear(*nwp.node, sm::vec2(0, 0));
		ctrans.SetScale(*nwp.node, sm::vec2(1, 1));

		return true;
	});

	m_record.Add(comb);
	ee0::MsgHelper::SetEditorDirty(m_sub_mgr, true);

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void ArrangeNodeImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
	// todo popup
//	m_popup.SetRightPopupMenu(menu, x, y);
}

void ArrangeNodeImpl::OnDeleteKeyDown()
{
	std::vector<n0::SceneNodePtr> nodes;
	nodes.reserve(m_selection.Size());
	m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		nodes.push_back(nwp.node);
		ee0::MsgHelper::DeleteNode(m_sub_mgr, nwp.node);
		return true;
	});

	m_record.Add(std::make_shared<DeleteNodeAO>(m_sub_mgr, nodes));
	ee0::MsgHelper::SetEditorDirty(m_sub_mgr, true);

	m_sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
}

void ArrangeNodeImpl::UpOneLayer()
{
	// todo
}

void ArrangeNodeImpl::DownOneLayer()
{
	// todo
}

sm::vec2 ArrangeNodeImpl::GetNodeOffset(const n0::SceneNodePtr& node) const
{
	auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
	sm::vec2 offset = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
	return offset;
}

bool ArrangeNodeImpl::OnSpriteShortcutKey(int keycode)
{
	// todo
	return false;
}

}