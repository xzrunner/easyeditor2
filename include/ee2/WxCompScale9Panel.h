#pragma once

#include <ee0/WxCompPanel.h>

class wxTextCtrl;

namespace n2 { class CompScale9; }

namespace ee2
{

class WxCompScale9Panel : public ee0::WxCompPanel
{
public:
	WxCompScale9Panel(wxWindow* parent, n2::CompScale9& cscale9);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

	void UpdateSpinValue(wxSpinEvent& event);

private:
	n2::CompScale9& m_cscale9;

	wxSpinCtrl* m_width;
	wxSpinCtrl* m_height;

}; // WxCompScale9Panel

}