#include "ee2/WxCompSprite2Panel.h"

#include <gum/SymbolPool.h>

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompSprite2Panel::WxCompSprite2Panel(wxWindow* parent, n2::CompSprite2& csprite2)
	: ee0::WxCompPanel(parent, "Sprite2")
	, m_csprite2(csprite2)
{
	InitLayout();
	Expand();
}

void WxCompSprite2Panel::RefreshNodeComp()
{
	m_filepath->SetValue(m_csprite2.GetFilepath());
}

void WxCompSprite2Panel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, m_csprite2.GetFilepath(),
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(WxCompSprite2Panel::OnSetFilepath));

		pane_sizer->Add(sizer);
	}
	
	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompSprite2Panel::OnSetFilepath(wxCommandEvent& event)
{
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, "*.json");
	if (dlg.ShowModal() == wxID_OK)
	{
		auto& path = dlg.GetPath();
		auto sym = gum::SymbolPool::Instance()->Fetch(path.ToStdString().c_str());
		m_csprite2.SetFilepath(path.ToStdString());
		m_csprite2.SetSymbol(sym);
	}
}

}