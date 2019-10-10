#ifndef PIFAN_WINDOW_HPP
#define PIFAN_WINDOW_HPP

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class PiFanWindow : public Gtk::Window {
    public:
        PiFanWindow();
        virtual ~PiFanWindow();
        
    protected:
        // Signal handlers:
        void on_button_clicked(Glib::ustring data);
        
        // Child widgets
        Gtk::Box m_box1;
        Gtk::Button m_button1, m_button2;
};

#endif // PIFAN-WINDOW_HPP
