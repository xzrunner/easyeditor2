#include "ee2/DownLayerNodeAO.h"
#include "ee2/NodeReorderHelper.h"

namespace ee2
{

DownLayerNodeAO::DownLayerNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                             const ee0::SelectionSet<n0::NodeWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void DownLayerNodeAO::Undo()
{
	NodeReorderHelper::UpOneLayer(*m_sub_mgr, m_selection);
}

void DownLayerNodeAO::Redo()
{
	NodeReorderHelper::DownOneLayer(*m_sub_mgr, m_selection);
}

}