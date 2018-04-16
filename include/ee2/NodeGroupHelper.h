#pragma once

#include <ee0/GameObj.h>

#include <vector>

namespace ee0 { class SubjectMgr; }
ECS_WORLD_DECL

namespace ee2
{

class NodeGroupHelper
{
public:
	static void BuildGroup(ECS_WORLD_PARAM ee0::SubjectMgr& sub_mgr, 
		const std::vector<ee0::GameObjWithPos>& objs);
	static void BreakUp(ECS_WORLD_PARAM ee0::SubjectMgr& sub_mgr, 
		const ee0::GameObjWithPos& obj);

}; // NodeGroupHelper

}