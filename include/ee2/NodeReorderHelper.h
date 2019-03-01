#pragma once

#include <ee0/SelectionSet.h>
#include <ee0/GameObj.h>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class NodeReorderHelper
{
public:
	static void UpOneLayer(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<ee0::GameObjWithPos>& selection);
	static void DownOneLayer(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<ee0::GameObjWithPos>& selection);

    static void SortNodes(ee0::SubjectMgr& sub_mgr, const n0::SceneNodePtr& up, const n0::SceneNodePtr& down);

}; // NodeReorderHelper

}