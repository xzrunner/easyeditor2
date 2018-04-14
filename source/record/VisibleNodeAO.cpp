#include "ee2/VisibleNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#ifndef GAME_OBJ_ECS
#include <ee0/CompNodeEditor.h>
#include <node0/SceneNode.h>
#else
#include <ee0/CompEntityEditor.h>
#include <ecsx/World.h>
#endif // GAME_OBJ_ECS
namespace ee2
{

VisibleNodeAO::VisibleNodeAO(const ee0::SubjectMgrPtr& sub_mgr, 
#ifdef GAME_OBJ_ECS
	                         const ecsx::World& world,
#endif // GAME_OBJ_ECS
	                         const std::vector<ee0::GameObj>& objs)
	: m_sub_mgr(sub_mgr)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
	, m_objs(objs)
{
}

void VisibleNodeAO::Undo()
{
	for (auto& obj : m_objs) 
	{
#ifndef GAME_OBJ_ECS
		auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetVisible(!ceditor.IsVisible());
#else
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
		ceditor.visible = !ceditor.visible;
#endif // GAME_OBJ_ECS
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void VisibleNodeAO::Redo()
{
	for (auto& obj : m_objs) 
	{
#ifndef GAME_OBJ_ECS
		auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetVisible(!ceditor.IsVisible());
#else
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
		ceditor.visible = !ceditor.visible;
#endif // GAME_OBJ_ECS
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}