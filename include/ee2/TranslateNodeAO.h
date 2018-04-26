#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/SelectionSet.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

#include <vector>

ECS_WORLD_DECL

namespace ee2
{

class TranslateNodeAO : public ee0::AtomicOP
{
public:
	TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
		const ee0::SelectionSet<ee0::GameObjWithPos>& selection, const sm::vec2& offset);
	TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
		const ee0::GameObj& obj, const sm::vec2& offset);
	TranslateNodeAO(const ee0::SubjectMgrPtr& sub_mgr, ECS_WORLD_PARAM
		const std::vector<ee0::GameObj>& objs, const sm::vec2& offset);

	virtual void Undo() override;
	virtual void Redo() override;

	virtual std::string ToString() const;

private:
	ee0::SubjectMgrPtr m_sub_mgr;
	ECS_WORLD_SELF_DEF

	std::vector<ee0::GameObj> m_objs;

	sm::vec2 m_offset;

}; // TranslateNodeAO 

}