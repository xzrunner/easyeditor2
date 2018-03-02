#include "ee2/WxStagePage.h"
#include "ee2/WxStageDropTarget.h"

#include <guard/check.h>

namespace ee2
{

WxStagePage::WxStagePage(wxWindow* parent, ee0::WxLibraryPanel* library)
	: ee0::WxStagePage(parent)
{
	m_sub_mgr.RegisterObserver(ee0::MSG_INSERT_SCENE_NODE, this);
	m_sub_mgr.RegisterObserver(ee0::MSG_DELETE_SCENE_NODE, this);
	m_sub_mgr.RegisterObserver(ee0::MSG_CLEAR_SCENE_NODE, this);

	if (library) {
		SetDropTarget(new WxStageDropTarget(library, this));
	}
}

void WxStagePage::OnNotify(ee0::MessageID msg, const ee0::VariantSet& variants)
{
	ee0::WxStagePage::OnNotify(msg, variants);

	switch (msg)
	{
	case ee0::MSG_INSERT_SCENE_NODE:
		InsertSceneNode(variants);
		break;
	case ee0::MSG_DELETE_SCENE_NODE:
		DeleteSceneNode(variants);
		break;
	case ee0::MSG_CLEAR_SCENE_NODE:
		ClearSceneNode();
		break;
	}
}

void WxStagePage::Traverse(std::function<bool(const n0::SceneNodePtr&)> func,
	                       const ee0::VariantSet& variants) const
{
	for (auto& node : m_nodes) {
		if (!func(node)) {
			break;
		}
	}
}

void WxStagePage::InsertSceneNode(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");
	if (m_node_selection.IsEmpty()) {
		m_nodes.push_back(*node);
	}

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxStagePage::DeleteSceneNode(const ee0::VariantSet& variants)
{
	auto var = variants.GetVariant("node");
	GD_ASSERT(var.m_type == ee0::VT_PVOID, "no var in vars: node");
	n0::SceneNodePtr* node = static_cast<n0::SceneNodePtr*>(var.m_val.pv);
	GD_ASSERT(node, "err scene node");
	if (DeleteSceneNode(*node)) {
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

void WxStagePage::ClearSceneNode()
{
	m_nodes.clear();
	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

bool WxStagePage::DeleteSceneNode(const n0::SceneNodePtr& node)
{
	for (auto itr = m_nodes.begin(); itr != m_nodes.end(); ++itr) {
		if (*itr == node) {
			m_nodes.erase(itr);
			return true;
		}
	}
	return false;
}

}