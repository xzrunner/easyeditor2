#pragma once

#include <ee0/EditOpState.h>

#include "ee2/NodeCtrlPoint.h"

namespace pt2 { class Camera; }
#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class ShearNodeState : public ee0::EditOpState
{
public:
	ShearNodeState(
		pt2::Camera& cam,
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::GameObj& obj,
		const NodeCtrlPoint::Node& ctrl_point);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Shear(const sm::vec2& curr);
	void Shear2(const sm::vec2& curr);

private:
	pt2::Camera& m_cam;
#ifdef GAME_OBJ_ECS
	ecsx::World& m_world;
#endif // GAME_OBJ_ECS

	ee0::GameObj m_obj;

	NodeCtrlPoint::Node m_ctrl_point;

	sm::vec2 m_first_shear;

}; // ShearNodeState

}