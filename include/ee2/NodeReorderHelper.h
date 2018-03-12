#pragma once

#include <ee0/SelectionSet.h>

#include <node0/NodeWithPos.h>

namespace ee0 { class SubjectMgr; }

namespace ee2
{

class NodeReorderHelper
{
public:
	static void UpOneLayer(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection);
	static void DownOneLayer(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection);

}; // NodeReorderHelper

}