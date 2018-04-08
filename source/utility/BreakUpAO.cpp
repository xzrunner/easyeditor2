#include "ee2/BreakUpAO.h"
#include "ee2/NodeGroupHelper.h"

namespace ee2
{

BreakUpAO::BreakUpAO(const ee0::SubjectMgrPtr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void BreakUpAO::Undo()
{
	// copy, will change selection
	std::vector<n0::NodeWithPos> objs;
	CopyFromSelection(objs);

	NodeGroupHelper::BuildGroup(*m_sub_mgr, objs);
}

void BreakUpAO::Redo()
{
	// copy, will change selection
	std::vector<n0::NodeWithPos> objs;
	CopyFromSelection(objs);

	printf("BreakUpAO::Redo count %d\n", objs.size());

	for (auto& obj : objs) {
		NodeGroupHelper::BreakUp(*m_sub_mgr, obj);
	}
}

void BreakUpAO::CopyFromSelection(std::vector<n0::NodeWithPos>& objs) const
{
	objs.reserve(m_selection.Size());
	m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
		objs.push_back(nwp);
		return true;
	});
}

}