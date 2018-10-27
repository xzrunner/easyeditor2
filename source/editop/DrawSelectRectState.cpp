#include "ee2/DrawSelectRectState.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>
#include <ee0/CameraHelper.h>
#include <ee0/color_config.h>

#include <tessellation/Painter.h>
#include <painting0/Camera.h>
#include <painting2/RenderSystem.h>
#include <painting2/OrthoCamera.h>

namespace ee2
{

DrawSelectRectState::DrawSelectRectState(const std::shared_ptr<pt0::Camera>& camera,
	                                     const ee0::SubjectMgrPtr& sub_mgr)
	: ee0::EditOpState(camera)
	, m_sub_mgr(sub_mgr)
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();
}

bool DrawSelectRectState::OnMousePress(int x, int y)
{
	m_first_pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);

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
		m_last_pos = ee0::CameraHelper::TransPosScreenToProject(*m_camera, x, y);
		m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
	}

	return false;
}

bool DrawSelectRectState::OnDraw() const
{
	if (!m_first_pos.IsValid() || !m_last_pos.IsValid()) {
		return false;
	}

	float line_width = 2;
	if (m_camera->TypeID() == pt0::GetCamTypeID<pt2::OrthoCamera>()) {
		line_width *= std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera)->GetScale();
	}

	tess::Painter pt;
	if (m_last_pos.x > m_first_pos.x)
	{
		pt.AddRectFilled(m_first_pos, m_last_pos, ee0::LIGHT_GREEN.ToABGR());
		pt.AddRect(m_first_pos, m_last_pos, ee0::SELECT_RED.ToABGR(), line_width);
	}
	else
	{
		pt.AddRectFilled(m_first_pos, m_last_pos, ee0::LIGHT_BLUE.ToABGR());
		pt.AddRect(m_first_pos, m_last_pos, ee0::SELECT_RED.ToABGR(), line_width);
	}
	pt2::RenderSystem::DrawPainter(pt);

	return false;
}

bool DrawSelectRectState::Clear()
{
	m_first_pos.MakeInvalid();
	m_last_pos.MakeInvalid();

	return false;
}

}