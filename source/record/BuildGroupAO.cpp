#include "ee2/BuildGroupAO.h"
#include "ee2/NodeGroupHelper.h"

namespace ee2
{

BuildGroupAO::BuildGroupAO(const ee0::SubjectMgrPtr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void BuildGroupAO::Undo()
{
	// copy, will change selection
	std::vector<n0::NodeWithPos> objs;
	CopyFromSelection(objs);

	for (auto& obj : objs) {
		NodeGroupHelper::BreakUp(*m_sub_mgr, obj);
	}
}

void BuildGroupAO::Redo()
{
	// copy, will change selection
	std::vector<n0::NodeWithPos> objs;
	CopyFromSelection(objs);

	NodeGroupHelper::BuildGroup(*m_sub_mgr, objs);
}

void BuildGroupAO::CopyFromSelection(std::vector<n0::NodeWithPos>& objs) const
{
	objs.reserve(m_selection.Size());
	m_selection.Traverse([&](const n0::NodeWithPos& nwp)->bool {
		objs.push_back(nwp);
		return true;
	});
}

}