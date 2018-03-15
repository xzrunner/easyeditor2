#include "ee2/OffsetNodeState.h"
#include "ee2/NodeCtrlPoint.h"
#include "ee2/ArrangeNodeCfg.h"
#include "ee2/OffsetNodeAO.h"

#include <ee0/CameraHelper.h>
#include <ee0/EditRecord.h>
#include <ee0/MsgHelper.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>
#include <painting2/Camera.h>
#include <painting2/OrthoCamera.h>

namespace ee2
{

OffsetNodeState::OffsetNodeState(pt2::Camera& cam, ee0::EditRecord& record,
	                             const ee0::SubjectMgrPtr& sub_mgr, const n0::SceneNodePtr& node)
	: m_cam(cam)
	, m_record(record)
	, m_sub_mgr(sub_mgr)
	, m_node(node)
{
	auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
	m_old_offset = ctrans.GetTrans().GetOffset();
}

bool OffsetNodeState::OnMouseRelease(int x, int y)
{
	float s = 1;
	if (m_cam.Type() == pt2::CAM_ORTHO2D) {
		s = static_cast<pt2::OrthoCamera&>(m_cam).GetScale();
	}

	float r = ArrangeNodeCfg::CTRL_NODE_RADIUS * s * 2;

	sm::vec2 ctrl_nodes[8];
	NodeCtrlPoint::GetNodeCtrlPoints(*m_node, ctrl_nodes);
	sm::vec2 fixed = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	auto& ctrans = m_node->GetUniqueComp<n2::CompTransform>();
	auto& pos = ctrans.GetTrans().GetPosition();
	if (sm::dis_pos_to_pos(fixed, pos) < r)
	{
		fixed = pos;
	} 
	else 
	{
		for (int i = 0; i < 8; ++i) {
			if (sm::dis_pos_to_pos(fixed, ctrl_nodes[i]) < r) {
				fixed = ctrl_nodes[i];
				break;
			}
		}
	}

	sm::vec2 new_offset = sm::rotate_vector(fixed - ctrans.GetTrans().GetCenter(), -ctrans.GetTrans().GetAngle());
	ctrans.SetOffset(*m_node, new_offset);

	// record
	m_record.Add(std::make_shared<OffsetNodeAO>(m_sub_mgr, m_node, new_offset, m_old_offset));
	ee0::MsgHelper::SetEditorDirty(*m_sub_mgr, true);

	return false;
}

bool OffsetNodeState::OnMouseDrag(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	auto& ctrans = m_node->GetUniqueComp<n2::CompTransform>();
	sm::vec2 offset = sm::rotate_vector(
		pos - ctrans.GetTrans().GetCenter(), 
		-ctrans.GetTrans().GetAngle()
	);
	ctrans.SetOffset(*m_node, offset);
	
	return true;
}

}