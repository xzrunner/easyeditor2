#include "ee2/DeleteNodeAO.h"

#include <ee0/MsgHelper.h>

namespace ee2
{

DeleteNodeAO::DeleteNodeAO(ee0::SubjectMgr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes)
	: m_sub_mgr(sub_mgr)
	, m_nodes(nodes)
{
}

void DeleteNodeAO::Undo()
{
	for (auto& node : m_nodes) {
		ee0::MsgHelper::InsertNode(m_sub_mgr, node);
	}
}

void DeleteNodeAO::Redo()
{
	for (auto& node : m_nodes) {
		ee0::MsgHelper::DeleteNode(m_sub_mgr, node);
	}
}

}