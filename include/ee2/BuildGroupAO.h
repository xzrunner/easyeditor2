#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class BuildGroupAO : public ee0::AtomicOP
{
public:
	BuildGroupAO(
		const ee0::SubjectMgrPtr& sub_mgr, 
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::SelectionSet<ee0::GameObjWithPos>& selection);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	void CopyFromSelection(std::vector<ee0::GameObjWithPos>& objs) const;

private:
	ee0::SubjectMgrPtr m_sub_mgr;
#ifdef GAME_OBJ_ECS
	ecsx::World&       m_world;
#endif // GAME_OBJ_ECS

	const ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

}; // BuildGroupAO

}