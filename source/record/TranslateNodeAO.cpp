#include "ee2/TranslateNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

TranslateNodeAO::TranslateNodeAO(ee0::SubjectMgr& sub_mgr, 
	                             const ee0::SelectionSet<n0::SceneNode>& selection, 
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	, m_offset(offset)
{
	m_nodes.reserve(selection.Size());
	selection.Traverse([&](const n0::SceneNodePtr& node)->bool {
		m_nodes.push_back(node);
		return true;
	});
}

TranslateNodeAO::TranslateNodeAO(ee0::SubjectMgr& sub_mgr, 
	                             const n0::SceneNodePtr& node, 
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	, m_offset(offset)
{
	m_nodes.push_back(node);
}

TranslateNodeAO::TranslateNodeAO(ee0::SubjectMgr& sub_mgr, 
	                             const std::vector<n0::SceneNodePtr>& nodes,
	                             const sm::vec2& offset)
	: m_sub_mgr(sub_mgr)
	, m_offset(offset)
	, m_nodes(nodes)
{
}

void TranslateNodeAO::Undo()
{
	for (auto& node : m_nodes) 
	{
		auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*node, ctrans.GetTrans().GetPosition() - m_offset);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

void TranslateNodeAO::Redo()
{
	for (auto& node : m_nodes)
	{
		auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*node, ctrans.GetTrans().GetPosition() + m_offset);
		m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}
}

}