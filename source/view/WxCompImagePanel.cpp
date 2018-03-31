#include "ee2/WxCompImagePanel.h"

#include <facade/ResPool.h>
#include <facade/Image.h>
#include <facade/Texture.h>

#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/filedlg.h>

namespace ee2
{

WxCompImagePanel::WxCompImagePanel(wxWindow* parent, n2::CompImage& cimage)
	: ee0::WxCompPanel(parent, "Image")
	, m_cimage(cimage)
{
	InitLayout();
	Expand();
}

void WxCompImagePanel::RefreshNodeComp()
{
	m_filepath->SetValue(m_cimage.GetFilepath());
}

void WxCompImagePanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	// filepath
	{
		wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

		sizer->Add(m_filepath = new wxTextCtrl(win, wxID_ANY, m_cimage.GetFilepath(),
			wxDefaultPosition, wxSize(180, -1), wxTE_READONLY));

		sizer->AddSpacer(5);

		wxButton* btn = new wxButton(win, wxID_ANY, wxT("..."), wxDefaultPosition, wxSize(25, 25));
		sizer->Add(btn);
		Connect(btn->GetId(), wxEVT_COMMAND_BUTTON_CLICKED, 
			wxCommandEventHandler(WxCompImagePanel::OnSetFilepath));

		pane_sizer->Add(sizer);
	}
	
	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompImagePanel::OnSetFilepath(wxCommandEvent& event)
{
	std::string filter = "*.png;*.jpg;*.bmp;*.pvr;*.pkm";
	wxFileDialog dlg(this, wxT("Choose image"), wxEmptyString, filter);
	if (dlg.ShowModal() == wxID_OK)
	{
		auto& path = dlg.GetPath();
		auto img = facade::ResPool::Instance().Fetch<facade::Image>(path.ToStdString());
		m_cimage.SetFilepath(path.ToStdString());
		m_cimage.SetTexture(img->GetTexture());
	}
}

}