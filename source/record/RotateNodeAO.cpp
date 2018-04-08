#include "ee2/RotateNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

RotateNodeAO::RotateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                       const std::vector<ee0::GameObj>& objs, 
	                       const sm::vec2& start, 
	                       const sm::vec2& end)
	: m_sub_mgr(sub_mgr)
	, m_inited(true)
	, m_start(start)
	, m_end(end)
	, m_angle(0)
	, m_objs(objs)
{
}

RotateNodeAO::RotateNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                       const std::vector<ee0::GameObj>& objs, 
	                       float angle)
	: m_sub_mgr(sub_mgr)
	, m_inited(false)
	, m_angle(angle)
	, m_objs(objs)
{
}

void RotateNodeAO::Undo()
{
	if (m_inited) 
	{
		for (auto& obj : m_objs) 
		{
			auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
			float angle = sm::get_angle_in_direction(ctrans.GetTrans().GetPosition(), m_start, m_end);
			ctrans.SetAngle(*obj, ctrans.GetTrans().GetAngle() - angle);
		}
	} 
	else if (m_angle != 0) 
	{
		for (auto& obj : m_objs) 
		{
			auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
			ctrans.SetAngle(*obj, ctrans.GetTrans().GetAngle() - m_angle);
		}
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void RotateNodeAO::Redo()
{
	if (m_inited)
	{
		for (auto& obj : m_objs)
		{
			auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
			float angle = sm::get_angle_in_direction(ctrans.GetTrans().GetPosition(), m_start, m_end);
			ctrans.SetAngle(*obj, ctrans.GetTrans().GetAngle() + angle);
		}
	}
	else if (m_angle != 0)
	{
		for (auto& obj : m_objs)
		{
			auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
			ctrans.SetAngle(*obj, ctrans.GetTrans().GetAngle() + m_angle);
		}
	}
	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}