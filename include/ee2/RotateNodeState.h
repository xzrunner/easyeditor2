#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

namespace pt2 { class Camera; }
namespace ee0 { class EditRecord; }
#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class RotateNodeState : public ee0::EditOpState
{
public:
	RotateNodeState(
		pt2::Camera& cam, 
		ee0::EditRecord& record, 
		const ee0::SubjectMgrPtr& sub_mgr, 
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		ee0::SelectionSet<ee0::GameObjWithPos>& selection,
		const sm::vec2& first_pos);

	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

private:
	void Rotate(const sm::vec2& dst);

private:
	pt2::Camera& m_cam;

	ee0::EditRecord&   m_record;
	ee0::SubjectMgrPtr m_sub_mgr;
#ifdef GAME_OBJ_ECS
	ecsx::World&       m_world;
#endif // GAME_OBJ_ECS

	ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	float m_angle;

}; // RotateNodeState

}