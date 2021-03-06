#pragma once

#include <ee0/EditOpState.h>

#include "ee2/NodeCtrlPoint.h"

ECS_WORLD_DECL

namespace ee2
{

class ShearNodeState : public ee0::EditOpState
{
public:
	ShearNodeState(const std::shared_ptr<pt0::Camera>& camera, ECS_WORLD_PARAM
		const ee0::GameObj& obj, const NodeCtrlPoint::Node& ctrl_point);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Shear(const sm::vec2& curr);
	void Shear2(const sm::vec2& curr);

private:
	ECS_WORLD_SELF_DEF

	ee0::GameObj m_obj;

	NodeCtrlPoint::Node m_ctrl_point;

	sm::vec2 m_first_shear;

}; // ShearNodeState

}