#include "ee2/ScaleNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

ScaleNodeAO::ScaleNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                     const n0::SceneNodePtr& node,
	                     const sm::vec2& new_scale, 
	                     const sm::vec2& old_scale)
	: m_sub_mgr(sub_mgr)
	, m_node(node)
	, m_new_scale(new_scale)
	, m_old_scale(old_scale)
{
}

void ScaleNodeAO::Undo()
{
	auto& ctrans = m_node->GetUniqueComp<n2::CompTransform>();
	ctrans.SetScale(*m_node, m_old_scale);
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void ScaleNodeAO::Redo()
{
	auto& ctrans = m_node->GetUniqueComp<n2::CompTransform>();
	ctrans.SetScale(*m_node, m_new_scale);
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}