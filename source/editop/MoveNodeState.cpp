#include "ee2/MoveNodeState.h"

#include <ee0/CameraHelper.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

MoveNodeState::MoveNodeState(pt2::Camera& cam, ee0::SelectionSet<n0::NodeWithPos>& selection)
	: m_cam(cam)
{
	m_center.Set(0, 0);
	m_nodes.reserve(selection.Size());
	selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		m_nodes.push_back(nwp.node);
		auto& ctrans = nwp.node->GetUniqueComp<n2::CompTransform>();
		m_center += ctrans.GetTrans().GetPosition();
		return true;
	});
	m_center /= static_cast<float>(m_nodes.size());

	m_offset.reserve(selection.Size());
	for (auto& node : m_nodes) 
	{
		auto& ctrans = node->GetUniqueComp<n2::CompTransform>();
		m_offset.push_back(ctrans.GetTrans().GetPosition() - m_center);
	}
}

bool MoveNodeState::OnMouseMove(int x, int y)
{
	if (m_nodes.empty()) {
		return false;
	}

	GD_ASSERT(m_nodes.size() == m_offset.size(), "err size");
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	for (int i = 0, n = m_nodes.size(); i < n; ++i) 
	{
		auto& ctrans = m_nodes[i]->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*m_nodes[i], pos + m_offset[i]);
	}
	return true;
}

}