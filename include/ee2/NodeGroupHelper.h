#pragma once

#include <vector>

namespace ee0 { class SubjectMgr; }
namespace n0 { class NodeWithPos; }

namespace ee2
{

class NodeGroupHelper
{
public:
	static void BuildGroup(ee0::SubjectMgr& sub_mgr, const std::vector<n0::NodeWithPos>& nodes);
	static void BreakUp(ee0::SubjectMgr& sub_mgr, const n0::NodeWithPos& node);

}; // NodeGroupHelper

}