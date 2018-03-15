#pragma once

#include <ee0/EditOP.h>
#include <ee0/typedef.h>

namespace ee0 { class EditOpState; }
namespace pt2 { class Camera; }

namespace ee2
{

class CamControlOP : public ee0::EditOP
{
public:
	static const uint32_t MOUSE_MOVE_FOCUS = 0x00000001;
	static const uint32_t RIGHT_TAP        = 0x00000002;
	static const uint32_t LEFT_TAP         = 0x00000004;

	static const uint32_t DEFAULT_FLAG     = MOUSE_MOVE_FOCUS | LEFT_TAP;

public:
	CamControlOP(pt2::Camera& cam, const ee0::SubjectMgrPtr& sub_mgr,
		uint32_t flag = DEFAULT_FLAG);

	virtual bool OnKeyDown(int key_code) override;
	virtual bool OnKeyUp(int key_code) override;
	virtual bool OnMouseLeftDown(int x, int y) override;
	virtual bool OnMouseLeftUp(int x, int y) override;
	virtual bool OnMouseRightDown(int x, int y) override;
	virtual bool OnMouseRightUp(int x, int y) override;
	virtual bool OnMouseDrag(int x, int y) override;
	virtual bool OnMouseMove(int x, int y) override;
	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	void ChangeEditOpState(const ee0::EditOpStatePtr& state);

protected:
	pt2::Camera&       m_cam;
	ee0::SubjectMgrPtr m_sub_mgr;

private:
	uint32_t m_flag;

	ee0::EditOpStatePtr m_op_state = nullptr;

	ee0::EditOpStatePtr m_zoom_state = nullptr;
	ee0::EditOpStatePtr m_left_pan_state = nullptr;
	ee0::EditOpStatePtr m_right_pan_state = nullptr;

}; // CamControlOP

}