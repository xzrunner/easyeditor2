#pragma once

#include <ee0/WxCompPanel.h>

namespace n2 { class CompShape; }

class wxTextCtrl;

namespace ee2
{

class WxCompShapePanel : public ee0::WxCompPanel
{
public:
	WxCompShapePanel(wxWindow* parent, n2::CompShape& cshape);

	virtual void RefreshNodeComp() override;

private:
	void InitLayout();

private:
	n2::CompShape& m_cshape;

	wxTextCtrl* m_type;

}; // WxCompShapePanel

}