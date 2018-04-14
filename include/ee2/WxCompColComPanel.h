#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/Config.h>

#ifndef GAME_OBJ_ECS
#include <node2/CompColorCommon.h>
#else
#include <entity2/CompColorCommon.h>
#endif // GAME_OBJ_ECS

namespace ee0 { class WxColorSpinCtrl; }

namespace ee2
{

class WxCompColComPanel : public ee0::WxCompPanel
{
public:
	WxCompColComPanel(
		wxWindow* parent, 
#ifndef GAME_OBJ_ECS
		n2::CompColorCommon& col,
#else
		e2::CompColorCommon& col,
#endif // GAME_OBJ_ECS
		const ee0::SubjectMgrPtr& sub_mgr
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout(const ee0::SubjectMgrPtr& sub_mgr);

private:
#ifndef GAME_OBJ_ECS
	n2::CompColorCommon& m_ccol;
#else
	e2::CompColorCommon& m_ccol;
#endif // GAME_OBJ_ECS

	ee0::WxColorSpinCtrl *m_mul_ctrl, *m_add_ctrl;

}; // WxCompColComPanel

}