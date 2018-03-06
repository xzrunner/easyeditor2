#include "ee2/CopyPasteNodeState.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

CopyPasteNodeState::CopyPasteNodeState(pt2::Camera& cam, ee0::SubjectMgr& sub_mgr,
	                                   ee0::SelectionSet<n0::SceneNode>& selection)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
	m_nodes.reserve(selection.Size());
	selection.Traverse([&](const n0::SceneNodePtr& node)->bool {
		m_nodes.push_back(node->Clone());
		return true;
	});

	selection.Clear();
	for (auto& node : m_nodes) {
		selection.Add(node);
		ee0::MsgHelper::InsertNode(m_sub_mgr, node);
	}
}

bool CopyPasteNodeState::OnMousePress(int x, int y)
{
	m_last_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	return false;
}

bool CopyPasteNodeState::OnMouseDrag(int x, int y)
{
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	sm::vec2 offset = pos - m_last_pos;
	for (auto& node : m_nodes) 
	{
		auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
		ctrans.GetTrans().SetPosition(ctrans.GetTrans().GetPosition() + offset);
	}
	m_last_pos = pos;

	return true;
}

}