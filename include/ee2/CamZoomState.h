#pragma once

#include <ee0/EditOpState.h>
#include <ee0/typedef.h>

namespace pt2 { class Camera; }

namespace ee2
{

class CamZoomState : public ee0::EditOpState
{
public:
	CamZoomState(pt2::Camera& cam, const ee0::SubjectMgrPtr& sub_mgr);

	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

private:
	pt2::Camera&       m_cam;
	ee0::SubjectMgrPtr m_sub_mgr;

}; // CamZoomState

}