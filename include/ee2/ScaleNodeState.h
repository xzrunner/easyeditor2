#pragma once

#include <ee0/EditOpState.h>

#include "ee2/NodeCtrlPoint.h"

namespace pt2 { class Camera; }

namespace ee2
{

class ScaleNodeState : public ee0::EditOpState
{
public:
	ScaleNodeState(pt2::Camera& cam, const n0::SceneNodePtr& node,
		const NodeCtrlPoint::Node& ctrl_point);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Scale(const sm::vec2& curr);

	void SetScaleTimes(const sm::vec2& st);

private:
	pt2::Camera& m_cam;

	n0::SceneNodePtr m_node;

	NodeCtrlPoint::Node m_ctrl_point;

	sm::vec2 m_first_pos, m_first_scale;

}; // ScaleNodeState

}