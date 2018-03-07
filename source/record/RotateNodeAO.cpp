#include "ee2/RotateNodeAO.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

RotateNodeAO::RotateNodeAO(ee0::SubjectMgr& sub_mgr, 
	                       const std::vector<n0::SceneNodePtr>& nodes, 
	                       const sm::vec2& start, 
	                       const sm::vec2& end)
	: m_sub_mgr(sub_mgr)
	, m_inited(true)
	, m_start(start)
	, m_end(end)
	, m_angle(0)
	, m_nodes(nodes)
{
}

RotateNodeAO::RotateNodeAO(ee0::SubjectMgr& sub_mgr, 
	                       const std::vector<n0::SceneNodePtr>& nodes, 
	                       float angle)
	: m_sub_mgr(sub_mgr)
	, m_inited(false)
	, m_angle(angle)
	, m_nodes(nodes)
{
}

void RotateNodeAO::Undo()
{
	if (m_inited) 
	{
		for (auto& node : m_nodes) 
		{
			auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
			float angle = sm::get_angle_in_direction(ctrans.GetTrans().GetPosition(), m_start, m_end);
			ctrans.SetAngle(*node, ctrans.GetTrans().GetAngle() - angle);
		}
	} 
	else if (m_angle != 0) 
	{
		for (auto& node : m_nodes) 
		{
			auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
			ctrans.SetAngle(*node, ctrans.GetTrans().GetAngle() - m_angle);
		}
	}
	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void RotateNodeAO::Redo()
{
	if (m_inited)
	{
		for (auto& node : m_nodes)
		{
			auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
			float angle = sm::get_angle_in_direction(ctrans.GetTrans().GetPosition(), m_start, m_end);
			ctrans.SetAngle(*node, ctrans.GetTrans().GetAngle() + angle);
		}
	}
	else if (m_angle != 0)
	{
		for (auto& node : m_nodes)
		{
			auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
			ctrans.SetAngle(*node, ctrans.GetTrans().GetAngle() + m_angle);
		}
	}
	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}