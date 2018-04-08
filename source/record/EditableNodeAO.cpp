#include "ee2/EditableNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/CompNodeEditor.h>

#include <node0/SceneNode.h>

namespace ee2
{

EditableNodeAO::EditableNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<ee0::GameObj>& objs)
	: m_sub_mgr(sub_mgr)
	, m_objs(objs)
{
}

void EditableNodeAO::Undo()
{
	for (auto& obj : m_objs) {
		auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetEditable(!ceditor.IsEditable());
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void EditableNodeAO::Redo()
{
	for (auto& obj : m_objs) {
		auto& ceditor = obj->GetUniqueComp<ee0::CompNodeEditor>();
		ceditor.SetEditable(!ceditor.IsEditable());
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}