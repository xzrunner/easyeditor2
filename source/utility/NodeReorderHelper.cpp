#include "ee2/NodeReorderHelper.h"

#include <ee0/VariantSet.h>
#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/MsgHelper.h>

namespace ee2
{

void NodeReorderHelper::UpOneLayer(ee0::SubjectMgr& sub_mgr, 
	                               const ee0::SelectionSet<ee0::GameObjWithPos>& selection)
{
	if (selection.IsEmpty()) {
		return;
	}

	std::vector<ee0::GameObjWithPos> objs;
	objs.reserve(selection.Size());
	selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
		objs.push_back(opw);
		return true;
	});
	
	for (auto& itr = objs.rbegin(); itr != objs.rend(); ++itr)
	{
		ee0::VariantSet vars;

		ee0::Variant var_obj;
		var_obj.m_type = ee0::VT_PVOID;
#ifndef GAME_OBJ_ECS
		var_obj.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(itr->GetNode());
#else
		var_obj.m_val.pv = &(*itr);
#endif // GAME_OBJ_ECS
		vars.SetVariant("obj", var_obj);

		ee0::Variant var_up;
		var_up.m_type = ee0::VT_BOOL;
		var_up.m_val.bl = true;
		vars.SetVariant("up", var_up);

		sub_mgr.NotifyObservers(ee0::MSG_REORDER_SCENE_NODE, vars);
	}

	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
	ee0::MsgHelper::InsertSelection(sub_mgr, objs);

	sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void NodeReorderHelper::DownOneLayer(ee0::SubjectMgr& sub_mgr, 
	                                 const ee0::SelectionSet<ee0::GameObjWithPos>& selection)
{
	if (selection.IsEmpty()) {
		return;
	}

	std::vector<ee0::GameObjWithPos> objs;
	objs.reserve(selection.Size());
	selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
		objs.push_back(opw);
		return true;
	});

	selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool
	{
		ee0::VariantSet vars;

		ee0::Variant var_obj;
		var_obj.m_type = ee0::VT_PVOID;
#ifndef GAME_OBJ_ECS
		var_obj.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(opw.GetNode());
#else
		var_obj.m_val.pv = &const_cast<ee0::GameObjWithPos&>(opw);
#endif // GAME_OBJ_ECS
		vars.SetVariant("obj", var_obj);

		ee0::Variant var_up;
		var_up.m_type = ee0::VT_BOOL;
		var_up.m_val.bl = false;
		vars.SetVariant("up", var_up);

		sub_mgr.NotifyObservers(ee0::MSG_REORDER_SCENE_NODE, vars);

		return true;
	});

	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
	ee0::MsgHelper::InsertSelection(sub_mgr, objs);

	sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}