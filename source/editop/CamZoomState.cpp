#include "ee2/CamZoomState.h"

#include <ee0/SubjectMgr.h>

#include <painting2/RenderCtxStack.h>
#include <painting2/OrthoCamera.h>
#include <painting2/Pseudo3DCamera.h>

#include <sm_c_vector.h>

namespace ee2
{

CamZoomState::CamZoomState(pt2::Camera& cam, ee0::SubjectMgr& sub_mgr)
	: m_cam(cam)
	, m_sub_mgr(sub_mgr)
{
}

bool CamZoomState::OnMouseWheelRotation(int x, int y, int direction)
{
	switch (m_cam.Type())
	{
	case pt2::CAM_ORTHO2D:
		{
			auto ctx = pt2::RenderCtxStack::Instance()->Top();
			if (!ctx) {
				return false;
			}

			int w = ctx->GetScreenWidth(),
				h = ctx->GetScreenHeight();
			float scale = 1;
			//if (GetKeyState(WXK_CONTROL)) {
			//	scale = direction > 0 ? 1 / 1.01f : 1.01f;
			//} else {
				scale = direction > 0 ? 1 / 1.1f : 1.1f;
			//}
			const float cx = static_cast<float>(x),
				        cy = static_cast<float>(h - y);
			auto& cam = dynamic_cast<pt2::OrthoCamera&>(m_cam);
			cam.Scale(scale, cx, cy, w, h);
		}
		break;
	case pt2::CAM_PSEUDO3D:
		{
			auto& cam = dynamic_cast<pt2::Pseudo3DCamera&>(m_cam);
			const sm_vec3* pos = cam.GetPos();
			float dz = direction < 0 ? pos->z * 0.1f : - pos->z * 0.1f;
			cam.Translate(sm::vec3(0, 0, dz));
		}
		break;
	}

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);

	return false;
}

}