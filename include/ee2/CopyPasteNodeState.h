#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>

#include <SM_Vector.h>
#include <node0/NodeWithPos.h>

#include <vector>

namespace pt2 { class Camera; }

namespace ee2
{

class CopyPasteNodeState : public ee0::EditOpState
{
public:
	CopyPasteNodeState(pt2::Camera& cam, const ee0::SubjectMgrPtr& sub_mgr,
		ee0::SelectionSet<n0::NodeWithPos>& selection);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	pt2::Camera&       m_cam;
	ee0::SubjectMgrPtr m_sub_mgr;

	sm::vec2 m_last_pos;

	std::vector<n0::NodeWithPos> m_nwps;
	
}; // CopyPasteNodeState

}