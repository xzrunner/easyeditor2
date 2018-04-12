#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>
#include <ee0/GameObj.h>

#include <vector>

namespace pt2 { class Camera; }
#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class SpriteSelection;

class MoveNodeState : public ee0::EditOpState
{
public:
	MoveNodeState(
		pt2::Camera& cam, 
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		ee0::SelectionSet<ee0::GameObjWithPos>& selection
	);

	virtual bool OnMouseMove(int x, int y) override;

private:
	pt2::Camera& m_cam;
#ifdef GAME_OBJ_ECS
	ecsx::World& m_world;
#endif // GAME_OBJ_ECS

	std::vector<ee0::GameObj> m_objs;

	sm::vec2 m_center;

}; // MoveNodeState

}