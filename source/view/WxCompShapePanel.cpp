#include "ee2/WxCompShapePanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node0/SceneNode.h>
#include <node2/CompShape.h>
#include <geoshape/Shape2D.h>

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompShapePanel::WxCompShapePanel(wxWindow* parent, n2::CompShape& cshape)
	: ee0::WxCompPanel(parent, "Shape2D")
	, m_cshape(cshape)
{
	InitLayout();
	Expand();
}

void WxCompShapePanel::RefreshNodeComp()
{
	auto& shape = m_cshape.GetShape();
	assert(shape);

	auto type_name = shape->get_type().get_name();
	m_type->SetValue(type_name.to_string());
}

void WxCompShapePanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	auto& shape = m_cshape.GetShape();
	assert(shape);

	// type
	{
		auto type_name = shape->get_type().get_name();
		m_type = new wxTextCtrl(win, wxID_ANY, type_name.to_string(),
			wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
		pane_sizer->Add(m_type);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

}