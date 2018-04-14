#include "ee2/ScaleNodeAO.h"

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

ScaleNodeAO::ScaleNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
#ifdef GAME_OBJ_ECS
	                     ecsx::World& world,
#endif // GAME_OBJ_ECS
	                     const ee0::GameObj& obj,
	                     const sm::vec2& new_scale, 
	                     const sm::vec2& old_scale)
	: m_sub_mgr(sub_mgr)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_obj(obj)
	, m_new_scale(new_scale)
	, m_old_scale(old_scale)
{
}

void ScaleNodeAO::Undo()
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetScale(*m_obj, m_old_scale);
#else
	e2::SysTransform::SetScale(m_world, m_obj, m_old_scale);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void ScaleNodeAO::Redo()
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetScale(*m_obj, m_new_scale);
#else
	e2::SysTransform::SetScale(m_world, m_obj, m_new_scale);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}