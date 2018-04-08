#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>
#include <node0/NodeWithPos.h>

#include <vector>

namespace pt2 { class Camera; }

namespace ee2
{

class SpriteSelection;

class MoveNodeState : public ee0::EditOpState
{
public:
	MoveNodeState(pt2::Camera& cam, ee0::SelectionSet<n0::NodeWithPos>& selection);

	virtual bool OnMouseMove(int x, int y) override;

private:
	pt2::Camera& m_cam;

	std::vector<ee0::GameObj> m_objs;

	sm::vec2 m_center;
	std::vector<sm::vec2> m_offset;

}; // MoveNodeState

}