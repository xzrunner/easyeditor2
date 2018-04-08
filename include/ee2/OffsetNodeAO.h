#pragma once

#include <ee0/AtomicOP.h>
#include <ee0/typedef.h>
#include <ee0/GameObj.h>

#include <SM_Vector.h>

namespace ee2
{

class OffsetNodeAO : public ee0::AtomicOP
{
public:
	OffsetNodeAO(const ee0::SubjectMgrPtr& sub_mgr, const ee0::GameObj& obj,
		const sm::vec2& new_offset, const sm::vec2& old_offset);

	virtual void Undo() override;
	virtual void Redo() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	ee0::GameObj m_obj;

	sm::vec2 m_new_offset, m_old_offset;

}; // OffsetNodeAO

}