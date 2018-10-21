#include "ee2/OffsetNodeState.h"
#include "ee2/NodeCtrlPoint.h"
#include "ee2/ArrangeNodeCfg.h"
#include "ee2/OffsetNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/CompTransform.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS
#include <SM_Calc.h>
#include <painting2/OrthoCamera.h>

namespace ee2
{

OffsetNodeState::OffsetNodeState(const std::shared_ptr<pt0::Camera>& camera,
	                             const ee0::SubjectMgrPtr& sub_mgr,
	                             ECS_WORLD_PARAM
	                             const ee0::GameObj& obj)
	: ee0::EditOpState(camera)
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
	m_old_offset = ctrans.GetTrans().GetOffset();
#else
	m_old_offset = e2::SysTransform::GetOffset(m_world, obj);
#endif // GAME_OBJ_ECS
}

bool OffsetNodeState::OnMouseRelease(int x, int y)
{
	float s = 1;
	if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
		s = std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera)->GetScale();
	}

	float r = ArrangeNodeCfg::CTRL_NODE_RADIUS * s * 2;

	sm::vec2 ctrl_nodes[8];
	NodeCtrlPoint::GetNodeCtrlPoints(ECS_WORLD_SELF_VAR m_obj, ctrl_nodes);
	sm::vec2 fixed = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	auto& pos = ctrans.GetTrans().GetPosition();
#else
	sm::vec2 pos = e2::SysTransform::GetPosition(m_world, m_obj);
#endif // GAME_OBJ_ECS
	if (sm::dis_pos_to_pos(fixed, pos) < r)
	{
		fixed = pos;
	}
	else
	{
		for (int i = 0; i < 8; ++i) {
			if (sm::dis_pos_to_pos(fixed, ctrl_nodes[i]) < r) {
				fixed = ctrl_nodes[i];
				break;
			}
		}
	}

#ifndef GAME_OBJ_ECS
	sm::vec2 new_offset = sm::rotate_vector(fixed - ctrans.GetTrans().GetCenter(), -ctrans.GetTrans().GetAngle());
	ctrans.SetOffset(*m_obj, new_offset);
#else
	auto center = e2::SysTransform::CalcCenter(m_world, m_obj);
	float angle = e2::SysTransform::GetAngle(m_world, m_obj);
	sm::vec2 new_offset = sm::rotate_vector(fixed - center, -angle);
	e2::SysTransform::SetOffset(m_world, m_obj, new_offset);
#endif // GAME_OBJ_ECS

	if (new_offset == m_old_offset) {
		return false;
	}

	// record
	auto aop = std::make_shared<OffsetNodeAO>(m_sub_mgr, ECS_WORLD_SELF_VAR m_obj, new_offset, m_old_offset);
	ee0::MsgHelper::AddAtomicOP(*m_sub_mgr, aop);

	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	return false;
}

bool OffsetNodeState::OnMouseDrag(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	sm::vec2 offset = sm::rotate_vector(
		pos - ctrans.GetTrans().GetCenter(),
		-ctrans.GetTrans().GetAngle()
	);
	ctrans.SetOffset(*m_obj, offset);
#else
	auto center = e2::SysTransform::CalcCenter(m_world, m_obj);
	float angle = e2::SysTransform::GetAngle(m_world, m_obj);
	auto offset = sm::rotate_vector(pos - center, -angle);
	e2::SysTransform::SetOffset(m_world, m_obj, offset);
#endif // GAME_OBJ_ECS

	return true;
}

}