#include "ee2/OffsetNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#else
#include <entity2/SysTransform.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

OffsetNodeAO::OffsetNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                       ECS_WORLD_PARAM
	                       const ee0::GameObj& obj,
	                       const sm::vec2& new_offset,
	                       const sm::vec2& old_offset)
	: m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_obj(obj)
	, m_new_offset(new_offset)
	, m_old_offset(old_offset)
{
}

void OffsetNodeAO::Undo()
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetOffset(*m_obj, m_old_offset);
#else
	e2::SysTransform::SetOffset(m_world, m_obj, m_old_offset);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void OffsetNodeAO::Redo()
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetOffset(*m_obj, m_new_offset);
#else
	e2::SysTransform::SetOffset(m_world, m_obj, m_new_offset);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}