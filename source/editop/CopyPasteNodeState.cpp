#include "ee2/CopyPasteNodeState.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

CopyPasteNodeState::CopyPasteNodeState(pt0::Camera& cam, 
	                                   const ee0::SubjectMgrPtr& sub_mgr,
	                                   ECS_WORLD_PARAM
	                                   ee0::SelectionSet<ee0::GameObjWithPos>& selection)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
{
	m_objs.reserve(selection.Size());
	selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
#ifndef GAME_OBJ_ECS
		m_objs.push_back(ee0::GameObjWithPos(opw.GetNode()->Clone(), opw.GetRoot(), 0));
#else
		m_objs.push_back(opw);
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
#ifndef GAME_OBJ_ECS
	for (auto& obj : m_objs)
	{
		auto& ctrans = obj.GetNode()->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*obj.GetNode(), ctrans.GetTrans().GetPosition() + offset);
	}
#else
	e2::SysTransform::Translate(m_world, m_objs, offset);
#endif
	m_last_pos = pos;

	return true;
}

}