#include "ee2/VisibleNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/CompNodeEditor.h>

#include <node0/SceneNode.h>

namespace ee2
{

VisibleNodeAO::VisibleNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes)
	: m_sub_mgr(sub_mgr)
	, m_nodes(nodes)
{
}

void VisibleNodeAO::Undo()
{
	for (auto& node : m_nodes) {
		auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetVisible(!ceditor.IsVisible());
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void VisibleNodeAO::Redo()
{
	for (auto& node : m_nodes) {
		auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetVisible(!ceditor.IsVisible());
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}