#include "ee2/BuildGroupAO.h"
#include "ee2/NodeGroupHelper.h"

namespace ee2
{

BuildGroupAO::BuildGroupAO(const ee0::SubjectMgrPtr& sub_mgr, 
#ifdef GAME_OBJ_ECS
	                       ecsx::World& world,
#endif // GAME_OBJ_ECS
	                       const ee0::SelectionSet<ee0::GameObjWithPos>& selection)
	: m_sub_mgr(sub_mgr)
#ifdef GAME_OBJ_ECS
	, m_world(world)
#endif // GAME_OBJ_ECS
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
#ifndef GAME_OBJ_ECS
		NodeGroupHelper::BreakUp(*m_sub_mgr, obj);
#else
		NodeGroupHelper::BreakUp(m_world, *m_sub_mgr, obj);
#endif // GAME_OBJ_ECS
	}
}

void BuildGroupAO::Redo()
{
	// copy, will change selection
	std::vector<ee0::GameObjWithPos> objs;
	CopyFromSelection(objs);

#ifndef GAME_OBJ_ECS
	NodeGroupHelper::BuildGroup(*m_sub_mgr, objs);
#else
	NodeGroupHelper::BuildGroup(m_world, *m_sub_mgr, objs);
#endif // GAME_OBJ_ECS
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