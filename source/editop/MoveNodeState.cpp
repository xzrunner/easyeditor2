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
	m_objs.reserve(selection.Size());
	selection.Traverse([&](const n0::NodeWithPos& nwp)->bool
	{
		m_objs.push_back(nwp.GetNode());
		auto& ctrans = nwp.GetNode()->GetUniqueComp<n2::CompTransform>();
		m_center += ctrans.GetTrans().GetPosition();
		return true;
	});
	m_center /= static_cast<float>(m_objs.size());

	m_offset.reserve(selection.Size());
	for (auto& obj : m_objs) 
	{
		auto& ctrans = obj->GetUniqueComp<n2::CompTransform>();
		m_offset.push_back(ctrans.GetTrans().GetPosition() - m_center);
	}
}

bool MoveNodeState::OnMouseMove(int x, int y)
{
	if (m_objs.empty()) {
		return false;
	}

	GD_ASSERT(m_objs.size() == m_offset.size(), "err size");
	auto pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
	for (int i = 0, n = m_objs.size(); i < n; ++i) 
	{
		auto& ctrans = m_objs[i]->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*m_objs[i], pos + m_offset[i]);
	}
	return true;
}

}