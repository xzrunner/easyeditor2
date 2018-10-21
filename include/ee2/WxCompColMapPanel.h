#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/typedef.h>
#include <ee0/Config.h>

#ifndef GAME_OBJ_ECS
#include <node2/CompColorMap.h>
#else
#include <entity2/CompColorMap.h>
#endif // GAME_OBJ_ECS

namespace ee0 { class WxColorSpinCtrl; }

namespace ee2
{

class WxCompColMapPanel : public ee0::WxCompPanel
{
public:
	WxCompColMapPanel(
		wxWindow* parent,
#ifndef GAME_OBJ_ECS
		n2::CompColorMap& col,
#else
		e2::CompColorMap& col,
#endif // GAME_OBJ_ECS
		const ee0::SubjectMgrPtr& sub_mgr
	);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout(const ee0::SubjectMgrPtr& sub_mgr);

private:
#ifndef GAME_OBJ_ECS
	n2::CompColorMap& m_ccol;
#else
	e2::CompColorMap& m_ccol;
#endif // GAME_OBJ_ECS

	ee0::WxColorSpinCtrl *m_rmap_ctrl, *m_gmap_ctrl, *m_bmap_ctrl;

}; // WxCompColMapPanel

}