#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

ECS_WORLD_DECL

namespace ee2
{

class ScaleNodeAO : public ee0::AtomicOP
{
public:
	ScaleNodeAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
		const ee0::GameObj& obj, const sm::vec2& new_scale, const sm::vec2& old_scale);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	ee0::GameObj m_obj;

	sm::vec2 m_old_scale, m_new_scale;

}; // ScaleNodeAO 

}