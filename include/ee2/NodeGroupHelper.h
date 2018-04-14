#pragma once

#include <ee0/GameObj.h>

#include <vector>

namespace ee0 { class SubjectMgr; }
#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class NodeGroupHelper
{
public:
	static void BuildGroup(
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		ee0::SubjectMgr& sub_mgr, 
		const std::vector<ee0::GameObjWithPos>& objs
	);
	static void BreakUp(
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		ee0::SubjectMgr& sub_mgr, 
		const ee0::GameObjWithPos& obj
	);

}; // NodeGroupHelper

}