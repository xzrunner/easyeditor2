#include "ee2/BuildGroupAO.h"
#include "ee2/NodeGroupHelper.h"

namespace ee2
{

BuildGroupAO::BuildGroupAO(ee0::SubjectMgr& sub_mgr, const ee0::SelectionSet<n0::NodeWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void BuildGroupAO::Undo()
{
	NodeGroupHelper::BreakUp(m_sub_mgr, m_selection);
}

void BuildGroupAO::Redo()
{
	NodeGroupHelper::BuildGroup(m_sub_mgr, m_selection);
}

}