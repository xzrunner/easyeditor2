#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

#include <vector>

namespace pt2 { class Camera; }
#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class CopyPasteNodeState : public ee0::EditOpState
{
public:
	CopyPasteNodeState(
		pt2::Camera& cam, 
		const ee0::SubjectMgrPtr& sub_mgr,
#ifdef GAME_OBJ_ECS
		const ecsx::World& world, 
#endif // GAME_OBJ_ECS
		ee0::SelectionSet<ee0::GameObjWithPos>& selection
	);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

private:
	pt2::Camera&       m_cam;
	ee0::SubjectMgrPtr m_sub_mgr;
#ifdef GAME_OBJ_ECS
	const ecsx::World& m_world;
#endif // GAME_OBJ_ECS

	sm::vec2 m_last_pos;

	std::vector<ee0::GameObjWithPos> m_objs;
	
}; // CopyPasteNodeState

}