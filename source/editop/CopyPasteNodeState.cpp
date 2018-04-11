#include "ee2/CopyPasteNodeState.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <ecsx/World.h>
#include <entity2/CompPosition.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

CopyPasteNodeState::CopyPasteNodeState(pt2::Camera& cam, 
	                                   const ee0::SubjectMgrPtr& sub_mgr,
#ifdef GAME_OBJ_ECS
                                       const ecsx::World& world,
#endif // GAME_OBJ_ECS
	                                   ee0::SelectionSet<ee0::GameObjWithPos>& selection)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
{
	m_objs.reserve(selection.Size());
	selection.Traverse([&](const ee0::GameObjWithPos& owp)->bool
	{
#ifndef GAME_OBJ_ECS
		m_objs.push_back(n0::NodeWithPos(owp.GetNode()->Clone(), owp.GetRoot(), 0));
#else
		m_objs.push_back(owp);
#endif // GAME_OBJ_ECS
		return true;
	});

	selection.Clear();
	for (auto& obj : m_objs) 
	{
		selection.Add(obj);
#ifndef GAME_OBJ_ECS
		ee0::MsgHelper::InsertNode(*m_sub_mgr, 
			std::const_pointer_cast<n0::SceneNode>(obj.GetNode()));
#else
		ee0::MsgHelper::InsertNode(*m_sub_mgr, obj);
#endif // GAME_OBJ_ECS
	}
}

bool CopyPasteNodeState::OnMousePress(int x, int y)
{
	m_last_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	return false;
}

bool CopyPasteNodeState::OnMouseDrag(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	sm::vec2 offset = pos - m_last_pos;
	for (auto& obj : m_objs) 
	{
#ifndef GAME_OBJ_ECS
		auto& ctrans = obj.GetNode()->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*obj.GetNode(), ctrans.GetTrans().GetPosition() + offset);
#else
		auto& cpos = m_world.GetComponent<e2::CompPosition>(obj);
		cpos.x += offset.x;
		cpos.y += offset.y;
#endif // GAME_OBJ_ECS
	}
	m_last_pos = pos;

	return true;
}

}