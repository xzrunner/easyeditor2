#include "ee2/RotateNodeState.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

RotateNodeState::RotateNodeState(pt2::Camera& cam, ee0::SubjectMgr& sub_mgr,
	                             ee0::SelectionSet<n0::SceneNode>& selection, 
	                             const sm::vec2& first_pos)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
	, m_angle(0)
{
	m_first_pos = m_last_pos = first_pos;
}

bool RotateNodeState::OnMouseRelease(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	if (pos == m_first_pos) {
		return false;
	}

	// todo record

	return false;
}

bool RotateNodeState::OnMouseDrag(int x, int y)
{
	if (m_selection.Size() != 1) {
		return false;
	}

	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	m_selection.Traverse([&](const n0::SceneNodePtr& node)->bool 
	{
		auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
		sm::vec2 center = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
		float rot = sm::get_angle_in_direction(center, m_last_pos, pos);
		ctrans.GetTrans().SetAngle(ctrans.GetTrans().GetAngle() + rot);

		m_angle += rot;

		return true;
	});

	m_last_pos = pos;

	return true;
}

}