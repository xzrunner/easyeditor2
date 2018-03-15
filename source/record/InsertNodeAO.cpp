#include "ee2/InsertNodeAO.h"

#include <ee0/MsgHelper.h>

namespace ee2
{

InsertNodeAO::InsertNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const n0::SceneNodePtr& node)
	: m_sub_mgr(sub_mgr)
{
	m_nodes.push_back(node);
}

InsertNodeAO::InsertNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<n0::SceneNodePtr>& nodes)
	: m_sub_mgr(sub_mgr)
	, m_nodes(nodes)
{
}

void InsertNodeAO::Undo()
{
	for (auto& node : m_nodes) {
		ee0::MsgHelper::DeleteNode(*m_sub_mgr, node);
	}
}

void InsertNodeAO::Redo()
{
	for (auto& node : m_nodes) {
		ee0::MsgHelper::InsertNode(*m_sub_mgr, node);
	}
}

}