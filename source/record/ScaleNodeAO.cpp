#include "ee2/ScaleNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

ScaleNodeAO::ScaleNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                     const ee0::GameObj& obj,
	                     const sm::vec2& new_scale, 
	                     const sm::vec2& old_scale)
	: m_sub_mgr(sub_mgr)
	, m_obj(obj)
	, m_new_scale(new_scale)
	, m_old_scale(old_scale)
{
}

void ScaleNodeAO::Undo()
{
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetScale(*m_obj, m_old_scale);
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void ScaleNodeAO::Redo()
{
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetScale(*m_obj, m_new_scale);
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}