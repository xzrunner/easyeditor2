#include "ee2/OffsetNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

OffsetNodeAO::OffsetNodeAO(ee0::SubjectMgr& sub_mgr, const n0::SceneNodePtr& node,
	                       const sm::vec2& new_offset, const sm::vec2& old_offset)
	: m_sub_mgr(sub_mgr)
	, m_node(node)
	, m_new_offset(new_offset)
	, m_old_offset(old_offset)
{
}

void OffsetNodeAO::Undo()
{
	auto& ctrans = m_node->GetUniqueComp<n2::CompTransform>();
	ctrans.SetOffset(*m_node, m_old_offset);
	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void OffsetNodeAO::Redo()
{
	auto& ctrans = m_node->GetUniqueComp<n2::CompTransform>();
	ctrans.SetOffset(*m_node, m_new_offset);
	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}