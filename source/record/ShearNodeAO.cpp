#include "ee2/ShearNodeAO.h"

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

ShearNodeAO::ShearNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
#ifdef GAME_OBJ_ECS
	                     ecsx::World& world,
#endif // GAME_OBJ_ECS
	                     const ee0::GameObj& obj,
	                     const sm::vec2& new_shear, 
	                     const sm::vec2& old_shear)
    : m_sub_mgr(sub_mgr)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_obj(obj)
    , m_new_shear(new_shear)
    , m_old_shear(old_shear)
{
}

void ShearNodeAO::Undo()
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetShear(*m_obj, m_old_shear);
#else
	e2::SysTransform::SetShear(m_world, m_obj, m_old_shear);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void ShearNodeAO::Redo()
{
#ifndef GAME_OBJ_ECS
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetShear(*m_obj, m_new_shear);
#else
	e2::SysTransform::SetShear(m_world, m_obj, m_new_shear);
#endif // GAME_OBJ_ECS
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}