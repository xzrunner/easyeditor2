#include "ee2/BreakUpAO.h"
#include "ee2/NodeGroupHelper.h"

namespace ee2
{

BreakUpAO::BreakUpAO(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void BreakUpAO::Undo()
{
	// copy, will change selection
	std::vector<n0::NodeWithPos> nodes;
	CopyFromSelection(nodes);

	NodeGroupHelper::BuildGroup(m_sub_mgr, nodes);
}

void BreakUpAO::Redo()
{
	// copy, will change selection
	std::vector<n0::NodeWithPos> nodes;
	CopyFromSelection(nodes);

	printf("BreakUpAO::Redo count %d\n", nodes.size());

	for (auto& node : nodes) {
		NodeGroupHelper::BreakUp(m_sub_mgr, node);
	}
}

void BreakUpAO::CopyFromSelection(std::vector<n0::NodeWithPos>& nodes) const
{
	nodes.reserve(m_selection.Size());
	m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
		nodes.push_back(nwp);
		return true;
	});
}

}