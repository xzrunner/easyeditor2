#pragma once

#include <ee0/GameObj.h>

#include <vector>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class NodeGroupHelper
{
public:
	static void BuildGroup(ee0::SubjectMgr& sub_mgr, const std::vector<ee0::GameObjWithPos>& objs);
	static void BreakUp(ee0::SubjectMgr& sub_mgr, const ee0::GameObjWithPos& obj);

}; // NodeGroupHelper

}