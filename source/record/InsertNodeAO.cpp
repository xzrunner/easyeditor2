#include "ee2/InsertNodeAO.h"

#include <ee0/MsgHelper.h>

namespace ee2
{

InsertNodeAO::InsertNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const ee0::GameObj& obj)
	: m_sub_mgr(sub_mgr)
{
	m_objs.push_back(obj);
}

InsertNodeAO::InsertNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const std::vector<ee0::GameObj>& objs)
	: m_sub_mgr(sub_mgr)
	, m_objs(objs)
{
}

void InsertNodeAO::Undo()
{
	for (auto& obj : m_objs) {
		ee0::MsgHelper::DeleteNode(*m_sub_mgr, obj);
	}
}

void InsertNodeAO::Redo()
{
	for (auto& obj : m_objs) {
		ee0::MsgHelper::InsertNode(*m_sub_mgr, obj);
	}
}

std::string InsertNodeAO::ToString() const
{
	if (m_objs.size() == 0) {
		return "";
	} else if (m_objs.size() == 1) {
		return "insert 1 game obj";
	} else {
		return "insert " + std::to_string(m_objs.size()) + " game objs";
	}
}

}