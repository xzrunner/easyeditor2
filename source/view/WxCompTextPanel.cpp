#include "ee2/WxCompTextPanel.h"

#include <ee0/SubjectMgr.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee2
{

WxCompTextPanel::WxCompTextPanel(wxWindow* parent, n2::CompText& ctext,
	                         ee0::SubjectMgr& sub_mgr)
	: ee0::WxCompPanel(parent, "Text")
	, m_ctext(ctext)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompTextPanel::RefreshNodeComp()
{
	m_text->SetValue(m_ctext.GetText().text);
}

void WxCompTextPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	auto& text = m_ctext.GetText();

	// text
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Text ")));

		sizer->Add(m_text = new wxTextCtrl(win, wxID_ANY, text.text, 
			wxDefaultPosition, wxSize(200, 19)));
		Connect(m_text->GetId(), wxEVT_COMMAND_TEXT_UPDATED, 
			wxCommandEventHandler(WxCompTextPanel::UpdateTextValue));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompTextPanel::UpdateTextValue(wxCommandEvent& event)
{
	if (event.GetId() == m_text->GetId())
	{
		m_ctext.GetText().text = m_text->GetValue().ToStdString();
	}

	m_sub_mgr.NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}