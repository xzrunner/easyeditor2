#include "ee2/UpLayerNodeAO.h"
#include "ee2/NodeReorderHelper.h"

namespace ee2
{

UpLayerNodeAO::UpLayerNodeAO(const ee0::SubjectMgrPtr& sub_mgr,
	                         const ee0::SelectionSet<ee0::GameObjWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	, m_selection(selection)
{
}

void UpLayerNodeAO::Undo()
{
	NodeReorderHelper::DownOneLayer(*m_sub_mgr, m_selection);
}

void UpLayerNodeAO::Redo()
{
	NodeReorderHelper::UpOneLayer(*m_sub_mgr, m_selection);
}

}