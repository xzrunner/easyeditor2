#pragma once

#include <ee0/EditOpState.h>
#include <ee0/typedef.h>

#include "ee2/NodeCtrlPoint.h"

namespace ee0 { class EditRecord; }
namespace pt2 { class Camera; }

namespace ee2
{

class ScaleNodeState : public ee0::EditOpState
{
public:
	ScaleNodeState(pt2::Camera& cam, ee0::EditRecord& record,
		const ee0::SubjectMgrPtr& sub_mgr, const ee0::GameObj& obj,
		const NodeCtrlPoint::Node& ctrl_point);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Scale(const sm::vec2& curr);

	void SetScaleTimes(const sm::vec2& st);

private:
	pt2::Camera& m_cam;

	ee0::EditRecord&   m_record;
	ee0::SubjectMgrPtr m_sub_mgr;

	ee0::GameObj m_obj;

	NodeCtrlPoint::Node m_ctrl_point;

	sm::vec2 m_first_pos, m_first_scale;

}; // ScaleNodeState

}