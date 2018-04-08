#include "ee2/ShearNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

ShearNodeAO::ShearNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                     const ee0::GameObj& obj,
	                     const sm::vec2& new_shear, 
	                     const sm::vec2& old_shear)
    : m_sub_mgr(sub_mgr)
	, m_obj(obj)
    , m_new_shear(new_shear)
    , m_old_shear(old_shear)
{
}

void ShearNodeAO::Undo()
{
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetShear(*m_obj, m_old_shear);
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void ShearNodeAO::Redo()
{
	auto& ctrans = m_obj->GetUniqueComp<n2::CompTransform>();
	ctrans.SetShear(*m_obj, m_new_shear);
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}