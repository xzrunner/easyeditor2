#include "ee2/TranslateNodeAO.h"

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

TranslateNodeAO::TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                             ECS_WORLD_PARAM
	                             const ee0::SelectionSet<ee0::GameObjWithPos>& selection,
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_offset(offset)
{
	m_objs.reserve(selection.Size());
	selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
#ifndef GAME_OBJ_ECS
		m_objs.push_back(opw.GetNode());
#else
		m_objs.push_back(opw);
#endif // GAME_OBJ_ECS
		return true;
	});
}

TranslateNodeAO::TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                             ECS_WORLD_PARAM
	                             const ee0::GameObj& obj, 
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_offset(offset)
{
	m_objs.push_back(obj);
}

TranslateNodeAO::TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                             ECS_WORLD_PARAM
	                             const std::vector<ee0::GameObj>& objs,
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_offset(offset)
	, m_objs(objs)
{
}

void TranslateNodeAO::Undo()
{
	for (auto& obj : m_objs) 
	{
#ifndef GAME_OBJ_ECS
		auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*obj, ctrans.GetTrans().GetPosition() - m_offset);
#else
		e2::SysTransform::Translate(m_world, obj, -m_offset);
#endif // GAME_OBJ_ECS
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

void TranslateNodeAO::Redo()
{
	for (auto& obj : m_objs)
	{
#ifndef GAME_OBJ_ECS
		auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*obj, ctrans.GetTrans().GetPosition() + m_offset);
#else
		e2::SysTransform::Translate(m_world, obj, m_offset);
#endif // GAME_OBJ_ECS
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

std::string TranslateNodeAO::ToString() const
{
	if (m_objs.size() == 0) {
		return "";
	} else if (m_objs.size() == 1) {
		return "translate 1 game obj";
	} else {
		return "translate " + std::to_string(m_objs.size()) + " game objs";
	}
}

std::string TranslateNodeAO::ToScript() const
{
	return "zz";
}

}