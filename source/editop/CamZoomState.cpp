#include "ee2/CamZoomState.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <painting2/OrthoCamera.h>
#include <painting2/Pseudo3DCamera.h>
#include <painting2/Blackboard.h>
#include <painting2/RenderContext.h>
#include <painting2/WindowContext.h>

#include <sm_c_vector.h>

namespace ee2
{

CamZoomState::CamZoomState(const std::shared_ptr<pt0::Camera>& camera,
	                       const ee0::SubjectMgrPtr& sub_mgr)
	: ee0::EditOpState(camera)
	, m_sub_mgr(sub_mgr)
{
}

bool CamZoomState::OnMouseWheelRotation(int x, int y, int direction)
{
	auto type = m_camera->TypeID();
	if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
	{
		auto& wc = pt2::Blackboard::Instance()->GetWindowContext();
		if (!wc) {
			return false;
		}

		int w = wc->GetScreenWidth(),
			h = wc->GetScreenHeight();
		float scale = 1;
		//if (GetKeyState(WXK_CONTROL)) {
		//	scale = direction > 0 ? 1 / 1.01f : 1.01f;
		//} else {
			scale = direction > 0 ? 1 / 1.1f : 1.1f;
		//}
		const float cx = static_cast<float>(x),
				    cy = static_cast<float>(h - y);
		auto o_cam = std::dynamic_pointer_cast<pt2::OrthoCamera>(m_camera);
		o_cam->Scale(scale, static_cast<int>(cx), static_cast<int>(cy), w, h);
	}
	else if (type == pt0::GetCamTypeID<pt2::Pseudo3DCamera>())
	{
		auto p_cam = std::dynamic_pointer_cast<pt2::Pseudo3DCamera>(m_camera);
		const sm_vec3* pos = p_cam->GetPos();
		float dz = direction < 0 ? pos->z * 0.1f : -pos->z * 0.1f;
		p_cam->Translate(sm::vec3(0, 0, dz));
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

void CamZoomState::Scale(const std::shared_ptr<pt0::Camera>& camera, float scale)
{
	auto type = camera->TypeID();
	if (type == pt0::GetCamTypeID<pt2::OrthoCamera>())
	{
		auto o_cam = std::dynamic_pointer_cast<pt2::OrthoCamera>(camera);
		o_cam->Set(o_cam->GetPosition(), o_cam->GetScale() * scale);
	}
}

}