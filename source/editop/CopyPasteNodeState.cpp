#include "ee2/CopyPasteNodeState.h"

#include <ee0/CameraHelper.h>
#include <ee0/MsgHelper.h>

#include <node0/SceneNode.h>
#include <node2/CompTransform.h>

namespace ee2
{

CopyPasteNodeState::CopyPasteNodeState(pt2::Camera& cam, ee0::SubjectMgr& sub_mgr,
	                                   ee0::SelectionSet<n0::NodeWithPos>& selection)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
	m_nwps.reserve(selection.Size());
	selection.Traverse([&](const n0::NodeWithPos& nwp)->bool 
	{
		m_nwps.push_back(n0::NodeWithPos(nwp.GetNode()->Clone(), nwp.GetRoot(), 0));
		return true;
	});

	selection.Clear();
	for (auto& nwp : m_nwps) 
	{
		selection.Add(nwp);
		ee0::MsgHelper::InsertNode(m_sub_mgr, 
			std::const_pointer_cast<n0::SceneNode>(nwp.GetNode()));
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
	for (auto& nwp : m_nwps) 
	{
		auto& ctrans = nwp.GetNode()->GetUniqueComp<n2::CompTransform>();
		ctrans.SetPosition(*nwp.GetNode(), ctrans.GetTrans().GetPosition() + offset);
	}
	m_last_pos = pos;

	return true;
}

}