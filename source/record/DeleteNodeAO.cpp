#include "ee2/DeleteNodeAO.h"

#include <ee0/MsgHelper.h>

namespace ee2
{

DeleteNodeAO::DeleteNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<ee0::GameObj>& objs)
	: m_sub_mgr(sub_mgr)
	, m_objs(objs)
{
}

void DeleteNodeAO::Undo()
{
	for (auto& obj : m_objs) {
		ee0::MsgHelper::InsertNode(*m_sub_mgr, obj);
	}
}

void DeleteNodeAO::Redo()
{
	for (auto& obj : m_objs) {
		ee0::MsgHelper::DeleteNode(*m_sub_mgr, obj);
	}
}

}