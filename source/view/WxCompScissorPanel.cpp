#include "ee2/WxCompScissorPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/MessageID.h>

#include <node2/CompScissor.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

namespace ee2
{

WxCompScissorPanel::WxCompScissorPanel(wxWindow* parent, 
	                                   n2::CompScissor& cscissor, 
	                                   const ee0::SubjectMgrPtr& sub_mgr)
	: ee0::WxCompPanel(parent, "Scissor")
	, m_cscissor(cscissor)
	, m_sub_mgr(sub_mgr)
{
	InitLayout();
	Expand();
}

void WxCompScissorPanel::RefreshNodeComp()
{
	auto& rect = m_cscissor.GetRect();

	m_xmin->SetValue(std::to_string(rect.xmin));
	m_ymin->SetValue(std::to_string(rect.ymin));
	m_xmax->SetValue(std::to_string(rect.xmax));
	m_ymax->SetValue(std::to_string(rect.xmax));
}

void WxCompScissorPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	auto& rect = m_cscissor.GetRect();

	static const wxSize INPUT_SIZE(65, 19);

	// min
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("xmin ")));
		sizer->Add(m_xmin = new wxTextCtrl(win, wxID_ANY, std::to_string(rect.xmin),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  ymin")));
		sizer->Add(m_ymin = new wxTextCtrl(win, wxID_ANY, std::to_string(rect.ymin),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));

		Connect(m_xmin->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompScissorPanel::EnterTextValue));
		Connect(m_ymin->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompScissorPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}
	// max
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("xmax ")));
		sizer->Add(m_xmax = new wxTextCtrl(win, wxID_ANY, std::to_string(rect.xmax),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  ymax")));
		sizer->Add(m_ymax = new wxTextCtrl(win, wxID_ANY, std::to_string(rect.ymax),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));

		Connect(m_xmax->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompScissorPanel::EnterTextValue));
		Connect(m_ymax->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompScissorPanel::EnterTextValue));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompScissorPanel::EnterTextValue(wxCommandEvent& event)
{
	sm::rect rect;

	double d;
	m_xmin->GetValue().ToDouble(&d);
	rect.xmin = d;
	m_ymin->GetValue().ToDouble(&d);
	rect.ymin = d;
	m_xmax->GetValue().ToDouble(&d);
	rect.xmax = d;
	m_ymax->GetValue().ToDouble(&d);
	rect.ymax = d;

	m_cscissor.SetRect(rect);

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

}