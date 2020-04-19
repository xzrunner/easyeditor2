#pragma once

#include <ee0/EditOpState.h>
#include <ee0/typedef.h>

#include <SM_Vector.h>

namespace ee2
{

class DrawSelectRectState : public ee0::EditOpState
{
public:
	DrawSelectRectState(const std::shared_ptr<pt0::Camera>& camera,
        const ee0::SubjectMgrPtr& sub_mgr);

	virtual bool OnMousePress(int x, int y) override;
	virtual bool OnMouseRelease(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;

	virtual bool OnDraw(const ur2::Device& dev, ur2::Context& ctx) const override;
	virtual bool Clear() override;

private:
	ee0::SubjectMgrPtr m_sub_mgr;

	sm::vec2 m_first_pos, m_last_pos;

}; // DrawSelectRectState

}