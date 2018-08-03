#include "ee2/CamTranslateState.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <painting2/OrthoCamera.h>
#include <painting2/Pseudo3DCamera.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>

namespace ee2
{

CamTranslateState::CamTranslateState(pt0::Camera& cam, const ee0::SubjectMgrPtr& sub_mgr)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
	m_last_pos.MakeInvalid();
}

bool CamTranslateState::OnMousePress(int x, int y)
{
	m_last_pos.Set(x, y);
	return false;
}

bool CamTranslateState::OnMouseRelease(int x, int y)
{
	m_last_pos.MakeInvalid();
	return false;
}

bool CamTranslateState::OnMouseDrag(int x, int y)
{
	if (!m_last_pos.IsValid()) {
		return false;
	}

	auto offx = m_last_pos.x - x;
	auto offy = y - m_last_pos.y;
	auto type = m_cam.TypeID();
	if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
	{
		auto& cam = dynamic_cast<pt2::OrthoCamera&>(m_cam);
		cam.Translate(sm::vec2(static_cast<float>(offx), static_cast<float>(offy)));
	}
	else if (type == pt0::GetCamTypeID<pt2::Pseudo3DCamera>())
	{
		auto& cam = dynamic_cast<pt2::Pseudo3DCamera&>(m_cam);
		auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
		if (!wc) {
			return false;
		}
		int w = wc->GetScreenWidth(),
			h = wc->GetScreenHeight();
		sm::vec2 last = cam.TransPosScreenToProject(m_last_pos.x, m_last_pos.y, w, h);
		sm::vec2 curr = cam.TransPosScreenToProject(x, y, w, h);
		cam.Translate(sm::vec3(curr.x - last.x, curr.y - last.y, 0));
	}

	m_last_pos.Set(x, y);

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

}