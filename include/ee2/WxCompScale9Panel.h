#pragma once

#include <ee0/WxCompPanel.h>
#include <ee0/Config.h>
#include <ee0/GameObj.h>

ECS_WORLD_DECL

class wxTextCtrl;

namespace ee2
{

class WxCompScale9Panel : public ee0::WxCompPanel
{
public:
	WxCompScale9Panel(wxWindow* parent, ECS_WORLD_PARAM const ee0::GameObj& obj);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateSpinValue(wxSpinEvent& event);

private:
	ECS_WORLD_SELF_DEF
	const ee0::GameObj& m_obj;

	wxSpinCtrl* m_width;
	wxSpinCtrl* m_height;

}; // WxCompScale9Panel

}