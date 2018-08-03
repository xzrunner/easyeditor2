#include "ee2/RotateNodeState.h"
#include "ee2/RotateNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>
#include <ee0/MsgHelper.h>

#include <SM_Calc.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

RotateNodeState::RotateNodeState(const std::shared_ptr<pt0::Camera>& camera,
	                             const ee0::SubjectMgrPtr& sub_mgr,
	                             ECS_WORLD_PARAM
	                             ee0::SelectionSet<ee0::GameObjWithPos>& selection,
	                             const sm::vec2& first_pos)
	: ee0::EditOpState(camera)
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_selection(selection)
	, m_angle(0)
{
	m_first_pos = m_last_pos = first_pos;
}

bool RotateNodeState::OnMouseRelease(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);

	if (pos == m_first_pos || m_selection.IsEmpty()) {
		return false;
	}

	// record
	std::vector<ee0::GameObj> objs;
	objs.reserve(m_selection.Size());
	m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
#ifndef GAME_OBJ_ECS
		objs.push_back(opw.GetNode());
#else
		objs.push_back(opw);
#endif // GAME_OBJ_ECS
		return true;
	});

	if (m_angle == 0) {
		return false;
	}

	// record
	auto aop = std::make_shared<RotateNodeAO>(m_sub_mgr, ECS_WORLD_SELF_VAR objs, m_angle);
	ee0::MsgHelper::AddAtomicOP(*m_sub_mgr, aop);

	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	return false;
}

bool RotateNodeState::OnMouseDrag(int x, int y)
{
	if (m_selection.Size() != 1) {
		return false;
	}

	auto pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
	m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
#ifndef GAME_OBJ_ECS
		auto& ctrans = opw.GetNode()->GetUniqueComp<n2::CompTransform>();
		sm::vec2 center = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
		float rot = sm::get_angle_in_direction(center, m_last_pos, pos);
		ctrans.SetAngle(*opw.GetNode(), ctrans.GetTrans().GetAngle() + rot);
#else
		sm::vec2 offset = e2::SysTransform::GetOffset(m_world, opw);
		sm::vec2 pos = e2::SysTransform::GetPosition(m_world, opw);
		sm::vec2 center = pos + offset;
		float rot = sm::get_angle_in_direction(center, m_last_pos, pos);
		e2::SysTransform::Rotate(m_world, opw, rot);
#endif // GAME_OBJ_ECS

		m_angle += rot;

		return true;
	});

	m_last_pos = pos;

	return true;
}

}