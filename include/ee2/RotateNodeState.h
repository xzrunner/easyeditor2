#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>

#include <SM_Vector.h>
#include <node0/NodeWithPos.h>

namespace pt2 { class Camera; }
namespace ee0 { class EditRecord; }

namespace ee2
{

class RotateNodeState : public ee0::EditOpState
{
public:
	RotateNodeState(pt2::Camera& cam, ee0::EditRecord& record, 
		const ee0::SubjectMgrPtr& sub_mgr, ee0::SelectionSet<n0::NodeWithPos>& selection, 
		const sm::vec2& first_pos);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Rotate(const sm::vec2& dst);

private:
	pt2::Camera& m_cam;

	ee0::EditRecord&   m_record;
	ee0::SubjectMgrPtr m_sub_mgr;

	ee0::SelectionSet<n0::NodeWithPos>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	float m_angle;

}; // RotateNodeState

}