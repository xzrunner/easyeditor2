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
#include "ee2/DownLayerNodeAO.h"
#include "ee2/UpLayerNodeAO.h"

#include <ee0/RightPopupMenu.h>
#include <ee0/KeyType.h>
#include <ee0/CameraHelper.h>
#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/MsgHelper.h>
#include <ee0/WxStagePage.h>
#include <ee0/WxStageCanvas.h>

#include <guard/check.h>
#include <SM_Calc.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/SysTransform.h>
#include <entity2/CompBoundingBox.h>
#endif // GAME_OBJ_ECS
#include <tessellation/Painter.h>
#include <unirender/Factory.h>
#include <unirender/RenderState.h>
#include <painting2/RenderSystem.h>
#include <painting2/OrthoCamera.h>

namespace ee2
{

ArrangeNodeImpl::ArrangeNodeImpl(ee0::WxStagePage& stage,
	                             const std::shared_ptr<pt0::Camera>& camera,
		                         ECS_WORLD_PARAM
	                             const ArrangeNodeCfg& cfg)
	: m_stage(stage)
	, m_camera(camera)
	, m_sub_mgr(stage.GetSubjectMgr())
	ECS_WORLD_SELF_ASSIGN
	, m_selection(stage.GetSelection())
	, m_cfg(cfg)
	, m_align(stage)
	, m_ctrl_node_radius(ArrangeNodeCfg::CTRL_NODE_RADIUS)
{
	m_align.SetOpen(cfg.is_auto_align_open);

    m_translate_op = std::make_shared<TranslateNodeState>(m_camera, m_sub_mgr, ECS_WORLD_SELF_VAR m_selection);

	m_left_down_pos.MakeInvalid();
	m_right_down_pos.MakeInvalid();
}

bool ArrangeNodeImpl::OnKeyDown(int keycode)
{
	if (wxGetKeyState(WXK_SHIFT) && OnSpriteShortcutKey(keycode)) {
		return true;
	}

	bool ret = false;
	switch (keycode)
	{
	case WXK_DELETE:
		ret = OnDeleteKeyDown();
		break;
	case WXK_PAGEUP:
		ret = UpOneLayer();
		break;
	case WXK_PAGEDOWN:
		ret = DownOneLayer();
		break;
	case '[':
		ret = DownOneLayer();
		break;
	case ']':
		ret = UpOneLayer();
		break;
	case 'm' : case 'M':
		ret = true;
		m_curr_op = std::make_shared<MoveNodeState>(m_camera, ECS_WORLD_SELF_VAR m_selection);
		break;
	case WXK_SPACE:
		ret = true;
		OnSpaceKeyDown();
		break;
    case 'H':
        ret = true;
        AutoSetCamera();
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
	case 'a': case WXK_LEFT:
		OnDirectionKeyDown(ee0::KEY_LEFT);
		break;
	case 'd': case WXK_RIGHT:
		OnDirectionKeyDown(ee0::KEY_RIGHT);
		break;
	case 's': case WXK_DOWN:
		OnDirectionKeyDown(ee0::KEY_DOWN);
		break;
	case 'w': case WXK_UP:
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
	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
	m_left_down_pos = pos;

	m_align.SetInvisible();

	if (m_selection.IsEmpty())
	{
		if (m_curr_op) {
			m_curr_op->OnMousePress(x, y);
		}
		return;
	}

	// copy & paste
	if (wxGetKeyState(WXK_ALT)) {
		m_curr_op = std::make_shared<CopyPasteNodeState>(
			m_camera, m_sub_mgr, ECS_WORLD_SELF_VAR m_selection);
	}

#ifndef GAME_OBJ_ECS
	ee0::GameObj selected = nullptr;
#else
	ee0::GameObj selected;
#endif // GAME_OBJ_ECS
	if (m_selection.Size() == 1)
	{
		m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
#ifndef GAME_OBJ_ECS
			selected = opw.GetNode();
#else
			selected = opw;
#endif // GAME_OBJ_ECS
			return false;
		});
	}
	if (!GAME_OBJ_VALID(selected)) {
		if (m_curr_op) {
			m_curr_op->OnMousePress(x, y);
		}
		return;
	}

	// offset
	if (m_cfg.is_offset_open)
	{
		sm::vec2 offset = GetNodeOffset(selected);
		if (sm::dis_pos_to_pos(offset, pos) < m_ctrl_node_radius) {
			m_curr_op = std::make_shared<OffsetNodeState>(m_camera, m_sub_mgr, ECS_WORLD_SELF_VAR selected);
			return;
		}
	}

	// scale
	if (m_cfg.is_deform_open && !wxGetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrl_nodes[8];
		NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_SELF_VAR selected, ctrl_nodes);
		for (int i = 0; i < 8; ++i)
		{
			if (sm::dis_pos_to_pos(ctrl_nodes[i], pos) < m_ctrl_node_radius)
			{
				NodeCtrlPoint::Node cn;
				cn.pos = ctrl_nodes[i];
				cn.type = NodeCtrlPoint::Type(i);
				m_curr_op = std::make_shared<ScaleNodeState>(m_camera, m_sub_mgr, ECS_WORLD_SELF_VAR selected, cn);
				return;
			}
		}
	}

	//// perspective
	//if (m_cfg.is_deform_open && m_stage.GetImpl().GetKeyState().GetKeyState(WXK_SHIFT))
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
    m_curr_op = m_translate_op;

	m_curr_op->OnMousePress(x, y);
}

void ArrangeNodeImpl::OnMouseLeftUp(int x, int y)
{
 	if (m_curr_op)
	{
		m_curr_op->OnMouseRelease(x, y);
		m_curr_op = nullptr;
	}

	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
	if (!m_selection.IsEmpty()) {
        m_curr_op = m_translate_op;
	}

	if (m_cfg.is_auto_align_open &&
		m_align.IsOpen() &&
		!m_selection.IsEmpty() &&
		m_left_down_pos != pos)
	{
		std::vector<ee0::GameObj> objs;
		m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
#ifndef GAME_OBJ_ECS
			objs.push_back(opw.GetNode());
#else
			objs.push_back(opw);
#endif // GAME_OBJ_ECS
			return false;
		});
		m_align.Align(ECS_WORLD_SELF_VAR objs);

		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
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
	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
	m_right_down_pos = pos;

#ifndef GAME_OBJ_ECS
	ee0::GameObj selected = nullptr;
#else
	ee0::GameObj selected;
#endif // GAME_OBJ_ECS
	if (m_selection.Size() == 1)
	{
		m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
#ifndef GAME_OBJ_ECS
			selected = opw.GetNode();
#else
			selected = opw;
#endif // GAME_OBJ_ECS
			return false;
		});
	}
	if (!GAME_OBJ_VALID(selected)) {
		return;
	}

	// shear
	if (m_cfg.is_deform_open)
	{
		sm::vec2 ctrl_nodes[8];
		NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_SELF_VAR selected, ctrl_nodes);
		for (int i = 0; i < 8; ++i)
		{
			if (sm::dis_pos_to_pos(ctrl_nodes[i], pos) < m_ctrl_node_radius)
			{
				NodeCtrlPoint::Node cn;
				cn.pos = ctrl_nodes[i];
				cn.type = NodeCtrlPoint::Type(i);
				m_curr_op = std::make_shared<ShearNodeState>(m_camera, ECS_WORLD_SELF_VAR selected, cn);
				return;
			}
		}
	}

	// rotate
	if (m_cfg.is_rotate_open) {
		m_curr_op = std::make_shared<RotateNodeState>(m_camera, m_sub_mgr, ECS_WORLD_SELF_VAR m_selection, pos);
	}
}

void ArrangeNodeImpl::OnMouseRightUp(int x, int y)
{
	if (!m_right_down_pos.IsValid()) {
		return;
	}

	sm::vec2 pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
	if (pos == m_right_down_pos)
	{
		wxMenu menu;
		SetRightPopupMenu(menu, x, y);
		m_stage.PopupMenu(&menu, x, y);
	}
	else if (m_curr_op)
	{
		m_curr_op->OnMouseRelease(x, y);
		m_curr_op = nullptr;
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
	//if (m_curr_op && m_curr_op->OnMouseMove(x, y))
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

	if (m_curr_op && m_curr_op->OnMouseDrag(x, y))
	{
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		// todo update panel
		//if (m_property_panel) {
		//	m_property_panel->EnablePropertyGrid(false);
		//}
	}
}

void ArrangeNodeImpl::OnPopMenuSelected(int type)
{
    auto popup = m_stage.GetImpl().GetPopupMenu();
    if (popup) {
        popup->OnRightPopupMenu(type);
    }
}

void ArrangeNodeImpl::OnDraw(float cam_scale) const
{
	m_ctrl_node_radius = ArrangeNodeCfg::CTRL_NODE_RADIUS * cam_scale;
	if ((m_cfg.is_deform_open || m_cfg.is_offset_open) && m_selection.Size() == 1)
	{
#ifndef GAME_OBJ_ECS
		ee0::GameObj selected = nullptr;
#else
		ee0::GameObj selected;
#endif // GAME_OBJ_ECS
		m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
#ifndef GAME_OBJ_ECS
			selected = opw.GetNode();
#else
			selected = opw;
#endif // GAME_OBJ_ECS
			return false;
		});

		GD_ASSERT(GAME_OBJ_VALID(selected), "null selected");
#ifndef GAME_OBJ_ECS
		auto& cbb = selected->GetUniqueComp<n2::CompBoundingBox>();
		auto& ctrans = selected->GetUniqueComp<n2::CompTransform>();
		auto& sz = cbb.GetSize();
		float sx = ctrans.GetTrans().GetScale().x,
			  sy = ctrans.GetTrans().GetScale().y;
#else
		auto& cbb = m_world.GetComponent<e2::CompBoundingBox>(selected);
		auto& sz = cbb.rect;
		auto scale = e2::SysTransform::GetScale(m_world, selected);
		float sx = scale.x, sy = scale.y;
#endif // GAME_OBJ_ECS

		float w = sz.Width() * fabs(sx);
		float h = sz.Height() * fabs(sy);
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
			tess::Painter pt;

			if (m_cfg.is_deform_open)
			{
				if (wxGetKeyState(WXK_SHIFT))
				{
					//sm::vec2 ctrl_nodes[4];
					//NodeCtrlPoint::GetNodeCtrlPointsExt(*selected, ctrl_nodes);
					//for (int i = 0; i < 4; ++i) {
					//	pt2::PrimitiveDraw::SetColor(pt0::Color(51, 204, 51));
					//	pt2::PrimitiveDraw::Circle(nullptr, ctrl_nodes[i], m_ctrl_node_radius, true);
					//}
				}
				else
				{
					sm::vec2 ctrl_nodes[8];
					NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_SELF_VAR selected, ctrl_nodes);
					for (int i = 0; i < 4; ++i) {
						pt.AddCircle(ctrl_nodes[i], m_ctrl_node_radius, 0xff33cc33, cam_scale);
					}
					for (int i = 4; i < 8; ++i) {
						pt.AddCircleFilled(ctrl_nodes[i], m_ctrl_node_radius, 0xff33cc33);
					}
				}
			}

			if (m_cfg.is_offset_open)
			{
				sm::vec2 offset = GetNodeOffset(selected);
				pt.AddCircleFilled(offset, m_ctrl_node_radius, 0xff3333cc);
			}

            auto& canvas = m_stage.GetImpl().GetCanvas();
			pt2::RenderSystem::DrawPainter(canvas->GetRenderDevice(),
                *canvas->GetRenderContext().ur_ctx, ur::DefaultRenderState2D(), pt);
		}
	}

    auto canvas = m_stage.GetImpl().GetCanvas();
	m_align.Draw(canvas->GetRenderDevice(), *canvas->GetRenderContext().ur_ctx, cam_scale);
}

void ArrangeNodeImpl::Clear()
{
}

ee0::GameObj ArrangeNodeImpl::QueryEditedNode(const sm::vec2& pos) const
{
#ifndef GAME_OBJ_ECS
	ee0::GameObj selected = nullptr;
	ee0::GameObj ret = nullptr;
#else
	ee0::GameObj selected;
	ee0::GameObj ret;
#endif // GAME_OBJ_ECS
	if (m_cfg.is_deform_open && m_selection.Size() == 1)
	{
		m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
#ifndef GAME_OBJ_ECS
			selected = opw.GetNode();
#else
			selected = opw;
#endif // GAME_OBJ_ECS
			return false;
		});
	}
	if (!GAME_OBJ_VALID(selected)) {
		return ret;
	}

	if (m_cfg.is_offset_open)
	{
		sm::vec2 offset = GetNodeOffset(selected);
		if (sm::dis_pos_to_pos(offset, pos) < m_ctrl_node_radius) {
			return selected;
		}
	}

	if (m_cfg.is_deform_open && !wxGetKeyState(WXK_SHIFT))
	{
		sm::vec2 ctrl_nodes[8];
		NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_SELF_VAR selected, ctrl_nodes);
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

	return ret;
}

void ArrangeNodeImpl::TraversePosIn(const sm::vec2& pos,
	                                std::function<bool(const ee0::GameObj&)> func) const
{
	m_stage.Traverse([&](const ee0::GameObj& obj)->bool
	{
		auto& cbb = obj->GetUniqueComp<n2::CompBoundingBox>();
		if (cbb.GetBounding(*obj).IsContain(pos)) {
			func(obj);
		}
		return true;
	});
}

bool ArrangeNodeImpl::IsSelectionEmpty() const
{
	return m_selection.IsEmpty();
}

void ArrangeNodeImpl::OnDirectionKeyDown(int type)
{
	if (!m_curr_op) return;

	bool dirty = m_curr_op->OnDirectionKeyDown(type);
	if (dirty)
	{
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		// todo update panel
		//if (m_property_panel) {
		//	m_property_panel->EnablePropertyGrid(false);
		//}
	}
}

void ArrangeNodeImpl::OnSpaceKeyDown()
{
	if (m_selection.IsEmpty()) {
		return;
	}

	auto comb = std::make_shared<CombineAO>();

	m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
#ifndef GAME_OBJ_ECS
		auto& obj = opw.GetNode();
		auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();

		auto& pos = ctrans.GetTrans().GetPosition();
		auto  angle = ctrans.GetTrans().GetAngle();
		auto& scale = ctrans.GetTrans().GetScale();
		auto& shear = ctrans.GetTrans().GetShear();
#else
		auto& obj = opw;

		auto pos = e2::SysTransform::GetPosition(m_world, obj);
		auto angle = e2::SysTransform::GetAngle(m_world, obj);
		auto scale = e2::SysTransform::GetScale(m_world, obj);
		auto shear = e2::SysTransform::GetShear(m_world, obj);
#endif // GAME_OBJ_ECS

		// record
		std::vector<ee0::GameObj> objs;
		objs.push_back(obj);
		comb->Add(std::make_shared<TranslateNodeAO>(m_sub_mgr, ECS_WORLD_SELF_VAR obj, - pos));
		comb->Add(std::make_shared<RotateNodeAO>(m_sub_mgr, ECS_WORLD_SELF_VAR objs, - angle));
		comb->Add(std::make_shared<ScaleNodeAO>(m_sub_mgr, ECS_WORLD_SELF_VAR obj, sm::vec2(1, 1), scale));
		comb->Add(std::make_shared<ShearNodeAO>(m_sub_mgr, ECS_WORLD_SELF_VAR obj, sm::vec2(0, 0), shear));

#ifndef GAME_OBJ_ECS
		ctrans.SetPosition(*obj, sm::vec2(0, 0));
		ctrans.SetAngle(*obj, 0);
		ctrans.SetShear(*obj, sm::vec2(0, 0));
		ctrans.SetScale(*obj, sm::vec2(1, 1));
#else
		e2::SysTransform::SetPosition(m_world, obj, sm::vec2(0, 0));
		e2::SysTransform::SetAngle(m_world, obj, 0);
		e2::SysTransform::SetShear(m_world, obj, sm::vec2(0, 0));
		e2::SysTransform::SetScale(m_world, obj, sm::vec2(1, 1));
#endif // GAME_OBJ_ECS

		return true;
	});

	ee0::MsgHelper::AddAtomicOP(*m_sub_mgr, comb);
	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void ArrangeNodeImpl::SetRightPopupMenu(wxMenu& menu, int x, int y)
{
    auto popup = m_stage.GetImpl().GetPopupMenu();
    if (popup) {
        popup->SetRightPopupMenu(menu, x, y);
    }
}

bool ArrangeNodeImpl::OnDeleteKeyDown()
{
	if (m_selection.IsEmpty()) {
		return false;
	}

	std::vector<ee0::GameObj> objs;
	objs.reserve(m_selection.Size());
	m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
#ifndef GAME_OBJ_ECS
		objs.push_back(opw.GetNode());
		ee0::MsgHelper::DeleteNode(*m_sub_mgr, opw.GetNode());
#else
		objs.push_back(opw);
		ee0::MsgHelper::DeleteNode(*m_sub_mgr, opw);
#endif // GAME_OBJ_ECS
		return true;
	});

	auto aop = std::make_shared<DeleteNodeAO>(m_sub_mgr, objs);
	ee0::MsgHelper::AddAtomicOP(*m_sub_mgr, aop);

	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	m_sub_mgr->NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);

	return true;
}

bool ArrangeNodeImpl::UpOneLayer()
{
	if (m_selection.IsEmpty()) {
		return false;
	}

	auto ao = std::make_shared<UpLayerNodeAO>(m_sub_mgr, m_selection);
	ao->Redo();
	ee0::MsgHelper::AddAtomicOP(*m_sub_mgr, ao);
	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	return true;
}

bool ArrangeNodeImpl::DownOneLayer()
{
	if (m_selection.IsEmpty()) {
		return false;
	}

	auto ao = std::make_shared<DownLayerNodeAO>(m_sub_mgr, m_selection);
	ao->Redo();
	ee0::MsgHelper::AddAtomicOP(*m_sub_mgr, ao);
	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	return true;
}

sm::vec2 ArrangeNodeImpl::GetNodeOffset(const ee0::GameObj& obj) const
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
	sm::vec2 offset = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
#else
	auto offset = e2::SysTransform::GetPosition(m_world, obj) +
		          e2::SysTransform::GetOffset(m_world, obj);
#endif // GAME_OBJ_ECS
	return offset;
}

bool ArrangeNodeImpl::OnSpriteShortcutKey(int keycode)
{
	// todo
	return false;
}

void ArrangeNodeImpl::AutoSetCamera()
{
    m_stage.Traverse([&](const ee0::GameObj& obj)->bool
    {
        auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
        auto type = m_camera->TypeID();
        if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
        {
            auto o_cam = std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera);
            o_cam->Set(ctrans.GetTrans().GetPosition(), 1);
        }
        return false;
    });
}

}