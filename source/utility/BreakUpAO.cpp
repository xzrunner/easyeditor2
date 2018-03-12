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
	NodeGroupHelper::BuildGroup(m_sub_mgr, m_selection);
}

void BreakUpAO::Redo()
{
	NodeGroupHelper::BreakUp(m_sub_mgr, m_selection);
}

}