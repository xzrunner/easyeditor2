#include "ee2/DrawSelectRectState.h"

#include <ee0/SubjectMgr.h>
#include <ee0/CameraHelper.h>
#include <ee0/color_config.h>

#include <painting2/PrimitiveDraw.h>

namespace ee2
{

DrawSelectRectState::DrawSelectRectState(pt2::Camera& cam, const ee0::SubjectMgrPtr& sub_mgr)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();
}

bool DrawSelectRectState::OnMousePress(int x, int y)
{
	m_first_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);

	return false;
}

bool DrawSelectRectState::OnMouseRelease(int x, int y)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

bool DrawSelectRectState::OnMouseDrag(int x, int y)
{
	if (m_first_pos.IsValid())
	{
		m_last_pos = ee0::CameraHelper::TransPosScreenToProject(m_cam, x, y);
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}

	return false;
}

bool DrawSelectRectState::OnDraw() const
{
	if (!m_first_pos.IsValid() || !m_last_pos.IsValid()) {
		return false;
	}

	pt2::PrimitiveDraw::LineWidth(2);
	if (m_last_pos.x > m_first_pos.x)
	{
		pt2::PrimitiveDraw::SetColor(ee0::LIGHT_GREEN);
		pt2::PrimitiveDraw::Rect(nullptr, m_first_pos, m_last_pos, true);
		pt2::PrimitiveDraw::SetColor(ee0::SELECT_RED);
		pt2::PrimitiveDraw::Rect(nullptr, m_first_pos, m_last_pos, false);
	}
	else
	{
		pt2::PrimitiveDraw::SetColor(ee0::LIGHT_BLUE);
		pt2::PrimitiveDraw::Rect(nullptr, m_first_pos, m_last_pos, true);
		pt2::PrimitiveDraw::SetColor(ee0::SELECT_RED);
		pt2::PrimitiveDraw::Rect(nullptr, m_first_pos, m_last_pos, false);
	}

	return false;
}

bool DrawSelectRectState::Clear()
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	return false;
}

}