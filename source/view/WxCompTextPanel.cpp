#include "ee2/WxCompTextPanel.h"

#include <ee0/SubjectMgr.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee2
{

WxCompTextPanel::WxCompTextPanel(wxWindow* parent, n2::CompText& ctext,
	                             const ee0::SubjectMgrPtr& sub_mgr)
	: ee0::WxCompPanel(parent, "Text")
	, m_ctext(ctext)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompTextPanel::RefreshNodeComp()
{
	auto& text = m_ctext.GetText();
	auto& tb = text.tb;

	m_text->SetValue(text.text);

	m_width->SetValue(std::to_string(tb.width));
	m_height->SetValue(std::to_string(tb.height));

	m_align_h->SetValue(std::to_string(tb.align_hori));
	m_align_v->SetValue(std::to_string(tb.align_vert));

	m_space_h->SetValue(std::to_string(tb.space_hori));
	m_space_v->SetValue(std::to_string(tb.space_vert));
}

void WxCompTextPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	auto& text = m_ctext.GetText();

	static const wxSize INPUT_SIZE(65, 19);

	// text
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Text ")));

		sizer->Add(m_text = new wxTextCtrl(win, wxID_ANY, text.text, 
			wxDefaultPosition, wxSize(200, 19), wxTE_PROCESS_ENTER));
		Connect(m_text->GetId(), wxEVT_COMMAND_TEXT_ENTER, 
			wxCommandEventHandler(WxCompTextPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// size
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Width ")));
		sizer->Add(m_width = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.width),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_width->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::EnterTextValue));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Height ")));
		sizer->Add(m_height = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.height),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_height->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// align
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Align Hori")));
		sizer->Add(m_align_h = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.align_hori),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_align_h->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::EnterTextValue));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Align Vert ")));
		sizer->Add(m_align_v = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.align_vert),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_align_v->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// space
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Space Hori")));
		sizer->Add(m_space_h = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.space_hori),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_space_h->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::EnterTextValue));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Space Vert ")));
		sizer->Add(m_space_v = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.space_vert),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_space_v->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// font
	{

	}
	// edge
	{

	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompTextPanel::EnterTextValue(wxCommandEvent& event)
{
	pt2::Text& text = m_ctext.GetText();
	auto& tb = text.tb;

	int id = event.GetId();
	if (id == m_text->GetId()) {
		text.text = m_text->GetValue().ToStdString();
	} else if (id == m_width->GetId()) {
		tb.width = std::stoi(m_width->GetValue().ToStdString());
	} else if (id == m_height->GetId()) {
		tb.height = std::stoi(m_height->GetValue().ToStdString());
	} else if (id == m_align_h->GetId()) {
		tb.align_hori = static_cast<pt2::Textbox::HoriAlign>(
			std::stoi(m_align_h->GetValue().ToStdString()));
	} else if (id == m_align_v->GetId()) {
		tb.align_vert = static_cast<pt2::Textbox::VertAlign>(
			std::stoi(m_align_v->GetValue().ToStdString()));
	} else if (id == m_space_h->GetId()) {
		tb.space_hori = std::stof(m_space_h->GetValue().ToStdString());
	} else if (id == m_space_v->GetId()) {
		tb.space_vert = std::stof(m_space_v->GetValue().ToStdString());
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}