#include "ee2/RotateNodeState.h"
#include "ee2/RotateNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>
#include <ee0/EditRecord.h>
#include <ee0/MsgHelper.h>

#include <SM_Calc.h>
#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

RotateNodeState::RotateNodeState(pt2::Camera& cam, ee0::EditRecord& record,
	                             ee0::SubjectMgr& sub_mgr,
	                             ee0::SelectionSet<n0::NodeWithPos>& selection, 
	                             const sm::vec2& first_pos)
	: m_cam(cam)
	, m_record(record)
	, m_sub_mgr(sub_mgr)
	, m_selection(selection)
	, m_angle(0)
{
	m_first_pos = m_last_pos = first_pos;
}

bool RotateNodeState::OnMouseRelease(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	if (pos == m_first_pos || m_selection.IsEmpty()) {
		return false;
	}

	// record
	std::vector<n0::SceneNodePtr> nodes;
	nodes.reserve(m_selection.Size());
	m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
		nodes.push_back(nwp.GetNode());
		return true;
	});
	m_record.Add(std::make_shared<RotateNodeAO>(m_sub_mgr, nodes, m_angle));
	ee0::MsgHelper::SetEditorDirty(m_sub_mgr, true);

	return false;
}

bool RotateNodeState::OnMouseDrag(int x, int y)
{
	if (m_selection.Size() != 1) {
		return false;
	}

	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		auto& ctrans = nwp.GetNode()->GetUniqueComp<n2::CompTransform>();
		sm::vec2 center = ctrans.GetTrans().GetPosition() + ctrans.GetTrans().GetOffset();
		float rot = sm::get_angle_in_direction(center, m_last_pos, pos);
		ctrans.SetAngle(*nwp.GetNode(), ctrans.GetTrans().GetAngle() + rot);

		m_angle += rot;

		return true;
	});

	m_last_pos = pos;

	return true;
}

}