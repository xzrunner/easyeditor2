#include "ee2/BuildGroupAO.h"
#include "ee2/NodeGroupHelper.h"

namespace ee2
{

BuildGroupAO::BuildGroupAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
	                       const ee0::SelectionSet<ee0::GameObjWithPos>& selection)
	: m_sub_mgr(sub_mgr)
	ECS_WORLD_SELF_ASSIGN
	, m_selection(selection)
{
}

void BuildGroupAO::Undo()
{
	// copy, will change selection
	std::vector<ee0::GameObjWithPos> objs;
	CopyFromSelection(objs);

	for (auto& obj : objs)
	{
		NodeGroupHelper::BreakUp(ECS_WORLD_SELF_VAR *m_sub_mgr, obj);
	}
}

void BuildGroupAO::Redo()
{
	// copy, will change selection
	std::vector<ee0::GameObjWithPos> objs;
	CopyFromSelection(objs);

	NodeGroupHelper::BuildGroup(ECS_WORLD_SELF_VAR *m_sub_mgr, objs);
}

void BuildGroupAO::CopyFromSelection(std::vector<ee0::GameObjWithPos>& objs) const
{
	objs.reserve(m_selection.Size());
	m_selection.Traverse([&](const ee0::GameObjWithPos& opw)->bool {
		objs.push_back(opw);
		return true;
	});
}

}