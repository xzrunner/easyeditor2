#pragma once

#include <ee0/SelectionSet.h>

#include <node0/NodeWithPos.h>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class NodeGroupHelper
{
public:
	static void BuildGroup(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection);
	static void BreakUp(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection);

}; // NodeGroupHelper

}