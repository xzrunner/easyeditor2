#include "ee2/EditableNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/CompNodeEditor.h>

#include <node0/SceneNode.h>

namespace ee2
{

EditableNodeAO::EditableNodeAO(ee0::SubjectMgr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes)
	: m_sub_mgr(sub_mgr)
	, m_nodes(nodes)
{
}

void EditableNodeAO::Undo()
{
	for (auto& node : m_nodes) {
		auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetEditable(!ceditor.IsEditable());
	}
	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void EditableNodeAO::Redo()
{
	for (auto& node : m_nodes) {
		auto& ceditor = node->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetEditable(!ceditor.IsEditable());
	}
	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}