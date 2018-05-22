#include "ee2/EditableNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#ifndef GAME_OBJ_ECS
#include <node0/SceneNode.h>
#include <node0/NodeFlagsHelper.h>
#include <node0/NodeFlags.h>
#else
#include <ee0/CompEntityEditor.h>
#include <entity0/World.h>
#endif // GAME_OBJ_ECS

namespace ee2
{

EditableNodeAO::EditableNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                           ECS_WORLD_PARAM
	                           const std::vector<ee0::GameObj>& objs)
	: m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_objs(objs)
{
}

void EditableNodeAO::Undo()
{
	for (auto& obj : m_objs)
	{
#ifndef GAME_OBJ_ECS
		n0::NodeFlagsHelper::SetFlag<n0::NodeNotEditable>(
			*obj, !n0::NodeFlagsHelper::GetFlag<n0::NodeNotEditable>(*obj));
#else
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
		ceditor.editable = !ceditor.editable;
#endif // GAME_OBJ_ECS
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void EditableNodeAO::Redo()
{
	for (auto& obj : m_objs)
	{
#ifndef GAME_OBJ_ECS
		n0::NodeFlagsHelper::SetFlag<n0::NodeNotEditable>(
			*obj, !n0::NodeFlagsHelper::GetFlag<n0::NodeNotEditable>(*obj));
#else
		auto& ceditor = m_world.GetComponent<ee0::CompEntityEditor>(obj);
		ceditor.editable = !ceditor.editable;
#endif // GAME_OBJ_ECS
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

std::string EditableNodeAO::ToString() const
{
	return "changed editable";
}

}