#include "ee2/MoveNodeState.h"

#include <ee0/CameraHelper.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <entity0/World.h>
#include <entity2/CompTransform.h>
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

MoveNodeState::MoveNodeState(const std::shared_ptr<pt0::Camera>& camera, ECS_WORLD_PARAM
	                         ee0::SelectionSet<ee0::GameObjWithPos>& selection)
	: ee0::EditOpState(camera)
	ECS_WORLD_SELF_ASSIGN
{
	m_center.Set(0, 0);
	m_objs.reserve(selection.Size());
	selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
#ifndef GAME_OBJ_ECS
		m_objs.push_back(opw.GetNode());
		auto& ctrans = opw.GetNode()->GetUniqueComp<n2::CompTransform>();
		m_center += ctrans.GetTrans().GetPosition();
#else
		m_objs.push_back(opw);
		m_center += e2::SysTransform::GetPosition(world, opw);
#endif // GAME_OBJ_ECS
		return true;
	});
	m_center /= static_cast<float>(m_objs.size());
}

bool MoveNodeState::OnMouseMove(int x, int y)
{
	if (m_objs.empty()) {
		return false;
	}

	auto pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
	auto offset = pos - m_center;
#ifndef GAME_OBJ_ECS
	for (auto& obj : m_objs)
	{
		auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*obj, ctrans.GetTrans().GetPosition() + offset);
	}
#else
	e2::SysTransform::Translate(m_world, m_objs, offset);
#endif // GAME_OBJ_ECS
	return true;
}

}