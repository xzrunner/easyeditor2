#pragma once

#include <ee0/EditOpState.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

namespace ee0 { class EditRecord; }
namespace pt2 { class Camera; }
#ifdef GAME_OBJ_ECS
namespace ecsx { class World; }
#endif // GAME_OBJ_ECS

namespace ee2
{

class TranslateNodeState : public ee0::EditOpState
{
public:
	TranslateNodeState(
		pt2::Camera& cam, 
		ee0::EditRecord& record, 
		const ee0::SubjectMgrPtr& sub_mgr, 
#ifdef GAME_OBJ_ECS
		ecsx::World& world,
#endif // GAME_OBJ_ECS
		const ee0::SelectionSet<ee0::GameObjWithPos>& selection,
		const sm::vec2& first_pos);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual	bool OnMouseDrag(int x, int y) override;

	virtual bool OnDirectionKeyDown(int type) override;

private:
	void Translate(const sm::vec2& offset);

private:
	pt2::Camera& m_cam;

	ee0::EditRecord&   m_record;
	ee0::SubjectMgrPtr m_sub_mgr;
#ifdef GAME_OBJ_ECS
	ecsx::World&       m_world;
#endif // GAME_OBJ_ECS

	const ee0::SelectionSet<ee0::GameObjWithPos>& m_selection;

	sm::vec2 m_first_pos, m_last_pos;

	bool m_dirty;
	
}; // TranslateState

}