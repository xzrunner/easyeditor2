#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>
#include <ee0/GameObj.h>

#include <vector>

ECS_WORLD_DECL

namespace ee2
{

class SpriteSelection;

class MoveNodeState : public ee0::EditOpState
{
public:
	MoveNodeState(const std::shared_ptr<pt0::Camera>& camera, 
		ECS_WORLD_PARAM ee0::SelectionSet<ee0::GameObjWithPos>& selection);

	virtual bool OnMouseMove(int x, int y) override;

private:
	ECS_WORLD_SELF_DEF

	std::vector<ee0::GameObj> m_objs;

	sm::vec2 m_center;

}; // MoveNodeState

}