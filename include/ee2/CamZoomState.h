#pragma once

#include <ee0/EditOpState.h>
#include <ee0/typedef.h>

namespace ee2
{

class CamZoomState : public ee0::EditOpState
{
public:
	CamZoomState(const std::shared_ptr<pt0::Camera>& camera,
		const ee0::SubjectMgrPtr& sub_mgr);

	virtual bool OnMouseWheelRotation(int x, int y, int direction) override;

    static void Scale(const std::shared_ptr<pt0::Camera>& camera, float scale);

private:
	ee0::SubjectMgrPtr m_sub_mgr;

}; // CamZoomState

}