#include "ee2/TranslateNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

TranslateNodeAO::TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                             const ee0::SelectionSet<n0::NodeWithPos>& selection, 
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	, m_offset(offset)
{
	m_objs.reserve(selection.Size());
	selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
		m_objs.push_back(nwp.GetNode());
		return true;
	});
}

TranslateNodeAO::TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                             const ee0::GameObj& obj, 
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	, m_offset(offset)
{
	m_objs.push_back(obj);
}

TranslateNodeAO::TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                             const std::vector<ee0::GameObj>& objs,
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	, m_offset(offset)
	, m_objs(objs)
{
}

void TranslateNodeAO::Undo()
{
	for (auto& obj : m_objs) 
	{
		auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*obj, ctrans.GetTrans().GetPosition() - m_offset);
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

void TranslateNodeAO::Redo()
{
	for (auto& obj : m_objs)
	{
		auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*obj, ctrans.GetTrans().GetPosition() + m_offset);
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

}