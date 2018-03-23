#include "ee2/WxCompTextPanel.h"

#include <ee0/SubjectMgr.h>
#include <ee0/WxColorGradientDlg.h>

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/radiobox.h>
#include <wx/spinctrl.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/clrpicker.h>

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

	m_width->SetValue(tb.width);
	m_height->SetValue(tb.height);

	m_font_type->SetSelection(tb.font_type);
	m_font_size->SetValue(tb.font_size);
	if (text.tb.font_color.items.size() == 1) {
		m_font_color->SetColour(ToWxColor(text.tb.font_color.items[0].col));
	}

	m_has_edge->SetValue(tb.has_edge);
	m_edge_size->SetValue(std::to_string(tb.edge_size));
	if (text.tb.edge_color.items.size() == 1) {
		m_edge_color->SetColour(ToWxColor(text.tb.edge_color.items[0].col));
	}

	m_align_h->SetValue(std::to_string(tb.align_hori));
	m_align_v->SetValue(std::to_string(tb.align_vert));

	m_space_h->SetValue(std::to_string(tb.space_hori));
	m_space_v->SetValue(std::to_string(tb.space_vert));

	m_overflow->SetValue(tb.overflow);
	m_richtext->SetValue(tb.richtext);
}

void WxCompTextPanel::InitLayout()
{
	wxWindow* win = GetPane();

	wxSizer* pane_sizer = new wxBoxSizer(wxVERTICAL);

	auto& text = m_ctext.GetText();

	static const wxSize INPUT_SIZE(65, 19);

	// text
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "String");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(m_text = new wxTextCtrl(win, wxID_ANY, text.text, 
			wxDefaultPosition, wxSize(200, 60), wxTE_MULTILINE));
		Connect(m_text->GetId(), wxEVT_COMMAND_TEXT_UPDATED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}
	// size
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Label");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Width ")));
		sizer->Add(m_width = new wxSpinCtrl(win, wxID_ANY, std::to_string(text.tb.width),
			wxDefaultPosition, INPUT_SIZE, wxSP_ARROW_KEYS, 1, 1024, text.tb.width));
		Connect(m_width->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxCompTextPanel::SpinEventHandler));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Height ")));
		sizer->Add(m_height = new wxSpinCtrl(win, wxID_ANY, std::to_string(text.tb.height),
			wxDefaultPosition, INPUT_SIZE, wxSP_ARROW_KEYS, 1, 1024, text.tb.width));
		Connect(m_height->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
			wxSpinEventHandler(WxCompTextPanel::SpinEventHandler));

		pane_sizer->Add(sizer);
	}
	// font
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Font");
		wxSizer* vert_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			wxArrayString choices;
			choices.Add("todo");
			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Type ")));
			sizer->Add(m_font_type = new wxChoice(win, wxID_ANY, wxDefaultPosition, wxDefaultSize, choices));
			Connect(m_font_type->GetId(), wxEVT_COMMAND_CHOICE_SELECTED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Size ")));
			sizer->Add(m_font_size = new wxSpinCtrl(win, wxID_ANY, std::to_string(text.tb.font_size),
				wxDefaultPosition, INPUT_SIZE, wxSP_ARROW_KEYS, 1, 128, text.tb.font_size));
			Connect(m_font_size->GetId(), wxEVT_COMMAND_SPINCTRL_UPDATED,
				wxSpinEventHandler(WxCompTextPanel::SpinEventHandler));

			vert_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(m_font_color = new wxColourPickerCtrl(win, wxID_ANY, ToWxColor(text.tb.font_color.items[0].col)));
			Connect(m_font_color->GetId(), wxEVT_COLOURPICKER_CHANGED,
				wxColourPickerEventHandler(WxCompTextPanel::ColourPickerEventHandler));

			sizer->Add(m_font_color_gradient = new wxButton(win, wxID_ANY, "gradient..."));
			Connect(m_font_color_gradient->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			vert_sizer->Add(sizer);
		}
		pane_sizer->Add(vert_sizer);
	}
	// edge
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Edge");
		wxSizer* vert_sizer = new wxStaticBoxSizer(bounding, wxVERTICAL);
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(m_has_edge = new wxCheckBox(win, wxID_ANY, "Edge"));
			Connect(m_has_edge->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Size ")));
			sizer->Add(m_edge_size = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.edge_size),
				wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
			Connect(m_edge_size->GetId(), wxEVT_COMMAND_TEXT_ENTER,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			vert_sizer->Add(sizer);
		}
		{
			wxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

			sizer->Add(m_edge_color = new wxColourPickerCtrl(win, wxID_ANY, ToWxColor(text.tb.edge_color.items[0].col)));
			Connect(m_edge_color->GetId(), wxEVT_COLOURPICKER_CHANGED,
				wxColourPickerEventHandler(WxCompTextPanel::ColourPickerEventHandler));

			sizer->Add(m_edge_color_gradient = new wxButton(win, wxID_ANY, "gradient..."));
			Connect(m_edge_color_gradient->GetId(), wxEVT_COMMAND_BUTTON_CLICKED,
				wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

			vert_sizer->Add(sizer);
		}
		pane_sizer->Add(vert_sizer);
	}
	// align
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Align");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Hori ")));
		sizer->Add(m_align_h = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.align_hori),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_align_h->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Vert ")));
		sizer->Add(m_align_v = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.align_vert),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_align_v->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}
	// space
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Space");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("Hori ")));
		sizer->Add(m_space_h = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.space_hori),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_space_h->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		sizer->Add(new wxStaticText(win, wxID_ANY, wxT("  Vert ")));
		sizer->Add(m_space_v = new wxTextCtrl(win, wxID_ANY, std::to_string(text.tb.space_vert),
			wxDefaultPosition, INPUT_SIZE, wxTE_PROCESS_ENTER));
		Connect(m_space_v->GetId(), wxEVT_COMMAND_TEXT_ENTER,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}
	// other
	{
		wxStaticBox* bounding = new wxStaticBox(win, wxID_ANY, "Others");
		wxSizer* sizer = new wxStaticBoxSizer(bounding, wxHORIZONTAL);

		sizer->Add(m_overflow = new wxCheckBox(win, wxID_ANY, "Overflow"));
		Connect(m_overflow->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		sizer->Add(m_richtext = new wxCheckBox(win, wxID_ANY, "Richtext"));
		Connect(m_richtext->GetId(), wxEVT_COMMAND_CHECKBOX_CLICKED,
			wxCommandEventHandler(WxCompTextPanel::CommandEventHandler));

		pane_sizer->Add(sizer);
	}

	win->SetSizer(pane_sizer);
	pane_sizer->SetSizeHints(win);
}

void WxCompTextPanel::CommandEventHandler(wxCommandEvent& event)
{
	pt2::Text& text = m_ctext.GetText();
	auto& tb = text.tb;

	int id = event.GetId();
	if (id == m_text->GetId()) {
		text.text = m_text->GetValue().ToStdString();
	} else if (id == m_font_type->GetId()) {
		tb.font_type = m_font_type->GetSelection();
	} else if (id == m_font_color_gradient->GetId()) {
		ee0::WxColorGradientDlg dlg(this, m_ctext.GetText().tb.font_color);
		if (dlg.ShowModal() == wxID_OK) {
			tb.font_color = dlg.GetColor();
		}
	} else if (id == m_has_edge->GetId()) {
		tb.has_edge = m_has_edge->GetValue();
	} else if (id == m_edge_size->GetId()) {
		tb.edge_size = std::stof(m_edge_size->GetValue().ToStdString());
	}  else if (id == m_edge_color_gradient->GetId()) {
		ee0::WxColorGradientDlg dlg(this, m_ctext.GetText().tb.edge_color);
		if (dlg.ShowModal() == wxID_OK) {
			tb.edge_color = dlg.GetColor();
		}
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
	} else if (id == m_overflow->GetId()) {
		tb.overflow = m_overflow->GetValue();
	} else if (id == m_richtext->GetId()) {
		tb.richtext = m_richtext->GetValue();
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxCompTextPanel::SpinEventHandler(wxSpinEvent& event)
{
	pt2::Text& text = m_ctext.GetText();
	auto& tb = text.tb;

	int id = event.GetId();
	if (id == m_width->GetId()) {
		tb.width = m_width->GetValue();
	} else if (id == m_height->GetId()) {
		tb.height = m_height->GetValue();
	} else if (id == m_font_size->GetId()) {
		tb.font_size = m_font_size->GetValue();
	}

	m_sub_mgr->NotifyObservers(ee0::MSG_SET_CANVAS_DIRTY);
}

void WxCompTextPanel::ColourPickerEventHandler(wxColourPickerEvent& event)
{
	pt2::Text& text = m_ctext.GetText();
	auto& tb = text.tb;

	int id = event.GetId();
	if (id == m_font_color->GetId()) 
	{
		auto col = m_font_color->GetColour();
		tb.font_color.items.resize(1);
		tb.font_color.items[0].col.FromABGR(col.GetRGBA());
	} 
	else if (id == m_edge_color->GetId()) 
	{
		auto col = m_edge_color->GetColour();
		tb.edge_color.items.resize(1);
		tb.edge_color.items[0].col.FromABGR(col.GetRGBA());
	}
}

wxColour WxCompTextPanel::ToWxColor(const pt2::Color& col)
{
	return wxColour(col.r, col.g, col.b, col.a);
}

}