#include "ee2/BreakUpAO.h"
#include "ee2/NodeGroupHelper.h"

namespace ee2
{

BreakUpAO::BreakUpAO(const ee0::SubjectMgrPtr& sub_mgr, 
	                 ECS_WORLD_PARAM
	                 const ee0::SelectionSet<ee0::GameObjWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_selection(selection)
{
}

void BreakUpAO::Undo()
{
	// copy, will change selection
	std::vector<ee0::GameObjWithPos> objs;
	CopyFromSelection(objs);

	NodeGroupHelper::BuildGroup(ECS_WORLD_SELF_VAR *m_sub_mgr, objs);
}

void BreakUpAO::Redo()
{
	// copy, will change selection
	std::vector<ee0::GameObjWithPos> objs;
	CopyFromSelection(objs);

	printf("BreakUpAO::Redo count %d\n", objs.size());

	for (auto& obj : objs) {
		NodeGroupHelper::BreakUp(ECS_WORLD_SELF_VAR *m_sub_mgr, obj);
	}
}

void BreakUpAO::CopyFromSelection(std::vector<ee0::GameObjWithPos>& objs) const
{
	objs.reserve(m_selection.Size());
	m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
		objs.push_back(opw);
		return true;
	});
}
}