#include "ee2/NodeReorderHelper.h"

#include <ee0/VariantSet.h>
#include <ee0/SubjectMgr.h>
#include <ee0/MsgHelper.h>

namespace ee2
{

void NodeReorderHelper::UpOneLayer(ee0::SubjectMgr& sub_mgr, 
	                               const ee0::SelectionSet<n0::NodeWithPos>& selection)
{
	if (selection.IsEmpty()) {
		return;
	}

	std::vector<n0::SceneNodePtr> nodes;
	nodes.reserve(selection.Size());
	selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		nodes.push_back(nwp.node);
		return true;
	});
	
	for (auto& itr = nodes.rbegin(); itr != nodes.rend(); ++itr)
	{
		ee0::VariantSet vars;

		ee0::Variant var_node;
		var_node.m_type = ee0::VT_PVOID;
		var_node.m_val.pv = &(*itr);
		vars.SetVariant("node", var_node);

		ee0::Variant var_up;
		var_up.m_type = ee0::VT_BOOL;
		var_up.m_val.bl = true;
		vars.SetVariant("up", var_up);

		sub_mgr.NotifyObservers(ee0::MSG_REORDER_SCENE_NODE, vars);
	}

	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
	ee0::MsgHelper::InsertNodeSelection(sub_mgr, nodes);

	sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void NodeReorderHelper::DownOneLayer(ee0::SubjectMgr& sub_mgr, 
	                                 const ee0::SelectionSet<n0::NodeWithPos>& selection)
{
	if (selection.IsEmpty()) {
		return;
	}

	std::vector<n0::SceneNodePtr> nodes;
	nodes.reserve(selection.Size());
	selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		nodes.push_back(nwp.node);
		return true;
	});

	selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		ee0::VariantSet vars;

		ee0::Variant var_node;
		var_node.m_type = ee0::VT_PVOID;
		var_node.m_val.pv = &std::const_pointer_cast<n0::SceneNode>(nwp.node);
		vars.SetVariant("node", var_node);

		ee0::Variant var_up;
		var_up.m_type = ee0::VT_BOOL;
		var_up.m_val.bl = false;
		vars.SetVariant("up", var_up);

		sub_mgr.NotifyObservers(ee0::MSG_REORDER_SCENE_NODE, vars);

		return true;
	});

	sub_mgr.NotifyObservers(ee0::MSG_NODE_SELECTION_CLEAR);
	ee0::MsgHelper::InsertNodeSelection(sub_mgr, nodes);

	sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}