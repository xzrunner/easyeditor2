#include "ee2/CamControlOP.h"
#include "ee2/CamZoomState.h"
#include "ee2/CamTranslateState.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <painting0/Camera.h>

#include <wx/defs.h>

namespace ee2
{

CamControlOP::CamControlOP(const std::shared_ptr<pt0::Camera>& camera,
	                       const ee0::SubjectMgrPtr& sub_mgr, uint32_t flag)
	: ee0::EditOP(camera)
	, m_sub_mgr(sub_mgr)
	, m_flag(flag)
{
	m_zoom_state       = std::make_shared<CamZoomState>(camera, sub_mgr);
	m_left_pan_state   = std::make_shared<CamTranslateState>(camera, sub_mgr);
	m_right_pan_state  = std::make_shared<CamTranslateState>(camera, sub_mgr);
    m_middle_pan_state = std::make_shared<CamTranslateState>(camera, sub_mgr);
	m_op_state = m_zoom_state;
}

bool CamControlOP::OnKeyDown(int key_code)
{
	switch (key_code)
	{
	case WXK_SPACE:
		if (m_flag & LEFT_TAP) {
			ChangeEditOpState(m_left_pan_state);
		}
		break;
	case WXK_ESCAPE:
		m_camera->Reset();
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
		break;
    case WXK_LEFT:
        CamTranslateState::Translate(m_camera, sm::vec2(-10, 0));
        m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
        break;
    case WXK_RIGHT:
        CamTranslateState::Translate(m_camera, sm::vec2(10, 0));
        m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
        break;
    case WXK_DOWN:
        CamTranslateState::Translate(m_camera, sm::vec2(0, -10));
        m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
        break;
    case WXK_UP:
        CamTranslateState::Translate(m_camera, sm::vec2(0, 10));
        m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
        break;
    case WXK_PAGEDOWN:
        CamZoomState::Scale(m_camera, 1.1f);
        m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
        break;
    case WXK_PAGEUP:
        CamZoomState::Scale(m_camera, 0.9f);
        m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
        break;
	}

	return false;
}

bool CamControlOP::OnKeyUp(int key_code)
{
	if ((m_flag & LEFT_TAP) && key_code == WXK_SPACE) {
		ChangeEditOpState(m_zoom_state);
	}
	return false;
}

bool CamControlOP::OnMouseLeftDown(int x, int y)
{
	return m_op_state->OnMousePress(x, y);
}

bool CamControlOP::OnMouseLeftUp(int x, int y)
{
	return m_op_state->OnMouseRelease(x, y);
}

bool CamControlOP::OnMouseMiddleDown(int x, int y)
{
	if (m_flag & MIDDLE_TAP) {
		ChangeEditOpState(m_middle_pan_state);
	}
	return m_op_state->OnMousePress(x, y);
}

bool CamControlOP::OnMouseMiddleUp(int x, int y)
{
	if (m_flag & MIDDLE_TAP) {
		ChangeEditOpState(m_zoom_state);
	}
	return m_op_state->OnMouseRelease(x, y);
}

bool CamControlOP::OnMouseRightDown(int x, int y)
{
	if (m_flag & RIGHT_TAP) {
		ChangeEditOpState(m_right_pan_state);
	}
	return m_op_state->OnMousePress(x, y);
}

bool CamControlOP::OnMouseRightUp(int x, int y)
{
	if (m_flag & RIGHT_TAP) {
		ChangeEditOpState(m_zoom_state);
	}
	return m_op_state->OnMouseRelease(x, y);
}

bool CamControlOP::OnMouseDrag(int x, int y)
{
	return m_op_state->OnMouseDrag(x, y);
}

bool CamControlOP::OnMouseMove(int x, int y)
{
	if (m_flag & MOUSE_MOVE_FOCUS) {
//		m_stage->SetFocus();
	}

	return false;
}

bool CamControlOP::OnMouseWheelRotation(int x, int y, int direction)
{
	return m_op_state->OnMouseWheelRotation(x, y, direction);
}

}