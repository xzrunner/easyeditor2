#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

ECS_WORLD_DECL

namespace ee2
{

class ShearNodeAO : public ee0::AtomicOP
{
public:
	ShearNodeAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
		const ee0::GameObj& obj, const sm::vec2& new_shear, const sm::vec2& old_shear);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual std::string ToString() const { return "shear game obj"; }

private:
	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	ee0::GameObj m_obj;

	sm::vec2 m_old_shear, m_new_shear;

}; // ShearNodeAO

}