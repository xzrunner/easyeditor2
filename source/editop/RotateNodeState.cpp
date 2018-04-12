#include "ee2/RotateNodeState.h"
#include "ee2/RotateNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>
#include <ee0/EditRecord.h>
#include <ee0/MsgHelper.h>

#include <SM_Calc.h>
#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <ecsx/World.h>
#include <entity2/CompTransform.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

RotateNodeState::RotateNodeState(pt2::Camera& cam, 
	                             ee0::EditRecord& record,
	                             const ee0::SubjectMgrPtr& sub_mgr,
#ifdef GAME_OBJ_ECS
                                 ecsx::World& world,
#endif // GAME_OBJ_ECS
	                             ee0::SelectionSet<ee0::GameObjWithPos>& selection,
	                             const sm::vec2& first_pos)
	: m_cam(cam)
	, m_record(record)
	, m_sub_mgr(sub_mgr)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_selection(selection)
	, m_angle(0)
{
	m_first_pos = m_last_pos = first_pos;
}

bool RotateNodeState::OnMouseRelease(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	if (pos == m_first_pos || m_selection.IsEmpty()) {
		return false;
	}

	// record
	std::vector<ee0::GameObj> objs;
	objs.reserve(m_selection.Size());
	m_selection.Traverse([&](const ee0::GameObjWithPos& owp)->bool {
#ifndef GAME_OBJ_ECS
		objs.push_back(owp.GetNode());
#else
		objs.push_back(owp);
#endif // GAME_OBJ_ECS
		return true;
	});
	m_record.Add(std::make_shared<RotateNodeAO>(m_sub_mgr, objs, m_angle));
	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	return false;
}

bool RotateNodeState::OnMouseDrag(int x, int y)
{
	if (m_selection.Size() != 1) {
		return false;
	}

	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	m_selection.Traverse([&](const ee0::GameObjWithPos& owp)->bool
	{
#ifndef GAME_OBJ_ECS
		auto& ctrans = owp.GetNode()->GetUniqueComp<n2::CompTransform>();
		sm::vec2 center = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
		float rot = sm::get_angle_in_direction(center, m_last_pos, pos);
		ctrans.SetAngle(*owp.GetNode(), ctrans.GetTrans().GetAngle() + rot);
#else
		sm::vec2 offset = e2::SysTransform::GetOffset(m_world, owp);
		sm::vec2 pos = e2::SysTransform::GetPosition(m_world, owp);
		sm::vec2 center = pos + offset;
		float rot = sm::get_angle_in_direction(center, m_last_pos, pos);
		e2::SysTransform::Rotate(m_world, owp, rot);
#endif // GAME_OBJ_ECS

		m_angle += rot;

		return true;
	});

	m_last_pos = pos;

	return true;
}

}