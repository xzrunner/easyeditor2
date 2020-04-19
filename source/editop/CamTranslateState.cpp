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

CamTranslateState::CamTranslateState(const std::shared_ptr<pt0::Camera>& camera,
	                                 const ee0::SubjectMgrPtr& sub_mgr)
	: ee0::EditOpState(camera)
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
	auto type = m_camera->TypeID();
	if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
	{
        Translate(m_camera, sm::vec2(static_cast<float>(offx), static_cast<float>(offy)));
	}
	else if (type == pt0::GetCamTypeID<pt2::Pseudo3DCamera>())
	{
		auto p_cam = std::dynamic_pointer_cast<pt2::Pseudo3DCamera>(m_camera);
        const int screen_w = static_cast<int>(p_cam->GetSize().x);
        const int screen_h = static_cast<int>(p_cam->GetSize().y);
		sm::vec2 last = p_cam->TransPosScreenToProject(m_last_pos.x, m_last_pos.y, screen_w, screen_h);
		sm::vec2 curr = p_cam->TransPosScreenToProject(x, y, screen_w, screen_h);
		p_cam->Translate(sm::vec3(curr.x - last.x, curr.y - last.y, 0));
	}

	m_last_pos.Set(x, y);

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

void CamTranslateState::Translate(const std::shared_ptr<pt0::Camera>& camera,
                                  const sm::vec2& offset)
{
    auto type = camera->TypeID();
    if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
    {
        auto o_cam = std::dynamic_pointer_cast<pt2::OrthoCamera>(camera);
        o_cam->Translate(offset);
    }
}

}